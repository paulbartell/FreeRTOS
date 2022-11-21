/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/**
 * @file mbedtls_pk_pkcs11_rsa.c
 * @brief mbedtls_pk implementation for pkcs11 RSA keys.
 *        Exports a mbedtls_pk_info_t type for performing
 */


#include "logging_levels.h"

#define LIBRARY_LOG_NAME     "MbedTLS_P11_RSA"
#define LIBRARY_LOG_LEVEL    LOG_ERROR

#include "logging_stack.h"


#include <string.h>

/* Mbedtls Includes */
#define MBEDTLS_ALLOW_PRIVATE_ACCESS

#include "mbedtls/pk.h"
#include "mbedtls/asn1.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/platform.h"
#include "mbedtls/asn1write.h"
#include "pk_wrap.h"

#include "core_pkcs11_config.h"
#include "core_pkcs11.h"

/*-----------------------------------------------------------*/


typedef struct P11RsaCtx
{
    P11PkCtx_t xP11PkCtx;
    mbedtls_rsa_context xMbedRsaCtx;
} P11RsaCtx_t;

/*-----------------------------------------------------------*/

/**
 * @brief
 *
 * @param pvCtx
 * @return size_t
 */
static size_t p11_rsa_get_bitlen( const void * pvCtx );

/**
 * @brief
 *
 * @param xType
 * @return int
 */
static int p11_rsa_can_do( mbedtls_pk_type_t xType );

/**
 * @brief
 *
 * @param pvCtx
 * @param xMdAlg
 * @param pucHash
 * @param xHashLen
 * @param pucSig
 * @param xSigLen
 * @return int
 */
static int p11_rsa_verify( void * pvCtx,
                           mbedtls_md_type_t xMdAlg,
                           const unsigned char * pucHash,
                           size_t xHashLen,
                           const unsigned char * pucSig,
                           size_t xSigLen );

/**
 * @brief
 *
 * @param ctx
 * @param md_alg
 * @param hash
 * @param hash_len
 * @param sig
 * @param sig_size
 * @param sig_len
 * @param f_rng
 * @param p_rng
 * @return int
 */
static int p11_rsa_sign( void * ctx,
                         mbedtls_md_type_t md_alg,
                         const unsigned char * hash,
                         size_t hash_len,
                         unsigned char * sig,
                         size_t sig_size,
                         size_t * sig_len,
                         int ( * f_rng )( void *, unsigned char *, size_t ),
                         void * p_rng );

/**
 * @brief
 *
 * @param pvPub
 * @param pvPrv
 * @param lFRng
 * @param pvPRng
 * @return int
 */
static int p11_rsa_check_pair( const void * pvPub,
                               const void * pvPrv,
                               int ( * lFRng )( void *, unsigned char *, size_t ),
                               void * pvPRng );

/**
 * @brief
 *
 * @return void*
 */
static void * p11_rsa_ctx_alloc( void );



/**
 * @brief
 *
 * @param pvCtx
 */
static void p11_rsa_ctx_free( void * pvCtx );

/**
 * @brief
 *
 * @param pvCtx
 * @param pxItems
 */
static void p11_rsa_debug( const void * pvCtx,
                           mbedtls_pk_debug_item * pxItems );

/*-----------------------------------------------------------*/

/**
 * @brief mbedtls_pk_info_t struct for PKCS#11 RSA operations.
 */
mbedtls_pk_info_t mbedtls_pkcs11_pk_rsa =
{
    .type               = MBEDTLS_PK_RSA,
    .name               = "PKCS#11",
    .get_bitlen         = p11_rsa_get_bitlen,
    .can_do             = p11_rsa_can_do,
    .verify_func        = p11_rsa_verify,
    .sign_func          = p11_rsa_sign,
    #if defined( MBEDTLS_ECDSA_C ) && defined( MBEDTLS_ECP_RESTARTABLE )
        .verify_rs_func = NULL,
        .sign_rs_func   = NULL,
    #endif /* MBEDTLS_ECDSA_C && MBEDTLS_ECP_RESTARTABLE */
    .decrypt_func       = NULL,
    .encrypt_func       = NULL,
    .check_pair_func    = p11_rsa_check_pair,
    .ctx_alloc_func     = p11_rsa_ctx_alloc,
    .ctx_free_func      = p11_rsa_ctx_free,
    #if defined( MBEDTLS_ECDSA_C ) && defined( MBEDTLS_ECP_RESTARTABLE )
        .rs_alloc_func  = NULL,
        .rs_free_func   = NULL,
    #endif /* MBEDTLS_ECDSA_C && MBEDTLS_ECP_RESTARTABLE */
    .debug_func         = p11_rsa_debug,
};

/*-----------------------------------------------------------*/

static void * p11_rsa_ctx_alloc( void )
{
    void * pvCtx = NULL;

    pvCtx = mbedtls_calloc( 1, sizeof( P11RsaCtx_t ) );

    if( pvCtx != NULL )
    {
        P11RsaCtx_t * pxP11Rsa = ( P11RsaCtx_t * ) pvCtx;

        /* Initialize other fields */
        pxP11Rsa->xP11PkCtx.pxFunctionList = NULL;
        pxP11Rsa->xP11PkCtx.xSessionHandle = CK_INVALID_HANDLE;
        pxP11Rsa->xP11PkCtx.xPkHandle = CK_INVALID_HANDLE;

        mbedtls_rsa_init( &( pxP11Rsa->xMbedRsaCtx ) );
    }

    return pvCtx;
}

/*-----------------------------------------------------------*/

static CK_RV p11_rsa_ctx_init( void * pvCtx,
                               CK_FUNCTION_LIST_PTR pxFunctionList,
                               CK_SESSION_HANDLE xSessionHandle,
                               CK_OBJECT_HANDLE xPkHandle )
{
    CK_RV xResult = CKR_OK;
    P11RsaCtx_t * pxP11RsaCtx = ( P11RsaCtx_t * ) pvCtx;
    mbedtls_rsa_context * pxMbedRsaCtx = NULL;

    configASSERT( pxFunctionList != NULL );
    configASSERT( xSessionHandle != CK_INVALID_HANDLE );
    configASSERT( xPkHandle != CK_INVALID_HANDLE );

    if( pxP11RsaCtx != NULL )
    {
        pxMbedRsaCtx = &( pxP11RsaCtx->xMbedRsaCtx );
    }
    else
    {
        xResult = CKR_FUNCTION_FAILED;
    }

    /* TODO: corePKCS11 does not allow exporting RSA public attributes; */
    /* CK_ATTRIBUTE pxAttrs[ 2 ] = */
    /* { */
    /*     { .type = CKA_MODULUS, .ulValueLen = 0, .pValue = NULL }, */
    /*     { .type = CKA_PUBLIC_EXPONENT,  .ulValueLen = 0, .pValue = NULL }, */
    /*     { .type = CKA_PRIME_1,  .ulValueLen = 0, .pValue = NULL }, */
    /*     { .type = CKA_PRIME_2,  .ulValueLen = 0, .pValue = NULL }, */
    /*     { .type = CKA_EXPONENT_1,  .ulValueLen = 0, .pValue = NULL }, */
    /*     { .type = CKA_EXPONENT_2,  .ulValueLen = 0, .pValue = NULL }, */
    /*     { .type = CKA_COEFFICIENT,  .ulValueLen = 0, .pValue = NULL }, */
    /* }; */

    ( void ) pxMbedRsaCtx;

    if( xResult == CKR_OK )
    {
        pxP11RsaCtx->xP11PkCtx.pxFunctionList = pxFunctionList;
        pxP11RsaCtx->xP11PkCtx.xSessionHandle = xSessionHandle;
        pxP11RsaCtx->xP11PkCtx.xPkHandle = xPkHandle;
    }

    return xResult;
}

/*-----------------------------------------------------------*/

static size_t p11_rsa_get_bitlen( const void * pvCtx )
{
    P11RsaCtx_t * pxRsaCtx = ( P11RsaCtx_t * ) pvCtx;

    configASSERT( mbedtls_rsa_info.get_bitlen );

    return mbedtls_rsa_info.get_bitlen( &( pxRsaCtx->xMbedRsaCtx ) );
}

/*-----------------------------------------------------------*/

static int p11_rsa_can_do( mbedtls_pk_type_t xType )
{
    return( xType == MBEDTLS_PK_RSA );
}

/*-----------------------------------------------------------*/

static int p11_rsa_verify( void * pvCtx,
                           mbedtls_md_type_t xMdAlg,
                           const unsigned char * pucHash,
                           size_t xHashLen,
                           const unsigned char * pucSig,
                           size_t xSigLen )
{
    P11RsaCtx_t * pxRsaCtx = ( P11RsaCtx_t * ) pvCtx;

    configASSERT( mbedtls_rsa_info.verify_func );

    return mbedtls_rsa_info.verify_func( &( pxRsaCtx->xMbedRsaCtx ),
                                         xMdAlg,
                                         pucHash, xHashLen,
                                         pucSig, xSigLen );
}

/*-----------------------------------------------------------*/

static int p11_rsa_sign( void * pvCtx,
                         mbedtls_md_type_t xMdAlg,
                         const unsigned char * pucHash,
                         size_t xHashLen,
                         unsigned char * pucSig,
                         size_t xSigBufferSize,
                         size_t * pxSigLen,
                         int ( * plRng )( void *, unsigned char *, size_t ),
                         void * pvRng )
{
    CK_RV xResult = CKR_OK;
    int32_t lFinalResult = 0;

    const P11RsaCtx_t * pxP11RsaCtx = NULL;
    const P11PkCtx_t * pxP11Ctx = NULL;

    CK_BYTE pxToBeSigned[ 256 ];

    CK_MECHANISM xMech =
    {
        .mechanism      = CKM_RSA_PKCS,
        .pParameter     = NULL,
        .ulParameterLen = 0
    };

    /* Unused parameters. */
    ( void ) ( plRng );
    ( void ) ( pvRng );

    configASSERT( pucSig != NULL );
    configASSERT( xSigBufferSize > 0 );
    configASSERT( pxSigLen != NULL );
    configASSERT( pucHash != NULL );
    configASSERT( xHashLen > 0 );

    configASSERT( xMdAlg == MBEDTLS_MD_SHA256 );
    configASSERT( xHashLen <= sizeof( pxToBeSigned ) );

    /* Sanity check buffer length. */
    if( xHashLen > sizeof( pxToBeSigned ) )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }
    else if( pvCtx != NULL )
    {
        pxP11RsaCtx = ( P11RsaCtx_t * ) pvCtx;
        pxP11Ctx = &( pxP11RsaCtx->xP11PkCtx );
    }
    else
    {
        xResult = CKR_FUNCTION_FAILED;
    }

    if( xResult == CKR_OK )
    {
        xResult = vAppendSHA256AlgorithmIdentifierSequence( ( uint8_t * ) pucHash, pxToBeSigned );
    }

    if( CKR_OK == xResult )
    {
        /* Use the PKCS#11 module to sign. */
        xResult = pxP11Ctx->pxFunctionList->C_SignInit( pxP11Ctx->xSessionHandle,
                                                        &xMech,
                                                        pxP11Ctx->xPkHandle );
    }

    if( CKR_OK == xResult )
    {
        CK_ULONG ulSigLen = sizeof( pxToBeSigned );

        xResult = pxP11Ctx->pxFunctionList->C_Sign( pxP11Ctx->xSessionHandle,
                                                    pxToBeSigned,
                                                    pkcs11RSA_SIGNATURE_INPUT_LENGTH,
                                                    pucSig,
                                                    &ulSigLen );

        *pxSigLen = ( size_t ) ulSigLen;
    }

    if( xResult != CKR_OK )
    {
        LogError( ( "Failed to sign message using PKCS #11 with error code %02X.", xResult ) );
        lFinalResult = -1;
    }

    return lFinalResult;
}

/*-----------------------------------------------------------*/

static int p11_rsa_check_pair( const void * pvPub,
                               const void * pvPrv,
                               int ( * lFRng )( void *, unsigned char *, size_t ),
                               void * pvPRng )
{
    P11RsaCtx_t * pxP11RsaCtx = ( P11RsaCtx_t * ) pvPrv;

    configASSERT( mbedtls_rsa_info.check_pair_func );

    return mbedtls_rsa_info.check_pair_func( pvPub, &( pxP11RsaCtx->xMbedRsaCtx ),
                                             lFRng, pvPRng );
}

/*-----------------------------------------------------------*/

static void p11_rsa_ctx_free( void * pvCtx )
{
    if( pvCtx != NULL )
    {
        P11RsaCtx_t * pxP11Rsa = ( P11RsaCtx_t * ) pvCtx;

        mbedtls_rsa_free( &( pxP11Rsa->xMbedRsaCtx ) );

        mbedtls_free( pvCtx );
    }
}

/*-----------------------------------------------------------*/

static void p11_rsa_debug( const void * pvCtx,
                           mbedtls_pk_debug_item * pxItems )
{
    P11RsaCtx_t * pxP11RsaCtx = ( P11RsaCtx_t * ) pvCtx;

    configASSERT( mbedtls_rsa_info.debug_func );

    mbedtls_rsa_info.debug_func( &( pxP11RsaCtx->xMbedRsaCtx ), pxItems );
}

/*-----------------------------------------------------------*/
