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
 * @file mbedtls_pk_pkcs11_ecdsa.c
 * @brief mbedtls_pk implementation for pkcs11 ECDSA keys.
 *        Exports a mbedtls_pk_info_t type.
 */

#include "logging_levels.h"

#define LIBRARY_LOG_NAME     "MbedTLS_P11_ECDSA"
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

typedef struct P11EcDsaCtx
{
    P11PkCtx_t xP11PkCtx;
    mbedtls_ecdsa_context xMbedEcDsaCtx;
} P11EcDsaCtx_t;

/*-----------------------------------------------------------*/

/**
 * @brief Allocates a P11EcDsaCtx_t
 *
 * @return A void pointer to the newly created P11EcDsaCtx_t.
 */
static void * p11_ecdsa_ctx_alloc( void );

/**
 * @brief Frees a P11EcDsaCtx_t
 *
 * @param pvCtx void pointer to the context to be freed.
 */
static void p11_ecdsa_ctx_free( void * pvCtx );

/**
 * @brief Perform an ecdsa sign operation with the given P11EcDsaCtx_t.
 *
 * @param pvCtx Void pointer to the relevant P11EcDsaCtx_t.
 * @param xMdAlg Hashing algorithm used to generate pucHash.
 * @param pucHash Pointer to a buffer containing the has of the data to be signed.
 * @param xHashLen Length of the hash of data to be signed.
 * @param pucSig Pointer to a buffer in which the signature should be stored.
 * @param xSigBufferSize Length of the buffer provided in pucSig.
 * @param pxSigLen Pointer to a size_t in which the length of the genrated signature will be stored.
 * @param plRng Function pointer to the RNG function.
 * @param pvRng Void pointer to the RNG function context.
 * @return 0 on success
 * @return A negative number on failure
 */
static int p11_ecdsa_sign( void * pvCtx,
                           mbedtls_md_type_t xMdAlg,
                           const unsigned char * pucHash,
                           size_t xHashLen,
                           unsigned char * pucSig,
                           size_t xSigBufferSize,
                           size_t * pxSigLen,
                           int ( * plRng )( void *, unsigned char *, size_t ),
                           void * pvRng );

/**
 * @brief Get the bit length of the relevant key.
 *
 * @param pvCtx Void pointer to the relevant P11EcDsaCtx_t.
 * @return size_t Bit length of the key.
 */
static size_t p11_ecdsa_get_bitlen( const void * pvCtx );

/**
 * @brief Returns true if the pk context can perform the given pk operation.
 *
 * @param xType Type of operation.
 * @return int 0 if the key is an ecdsa key.
 */
static int p11_ecdsa_can_do( mbedtls_pk_type_t xType );

/**
 * @brief Perform an ECDSA verify operation with the given pk context.
 *
 * Validates that the signature given in the pucSig and xSigLen arugments
 * matches the hash given in pucHash and xSigLen for the P11EcDsaCtx_t
 * specified in pvCtx.
 *
 * @param pvCtx Void pointer to the relevant P11EcDsaCtx_t
 * @param xMdAlg The hashing algorithm used to generate pucHash
 * @param pucHash Pointer to a buffer containing the hash to validate against
 * @param xHashLen Length of the hash in pucHash
 * @param pucSig Pointer to a buffer containing the signature to validate
 * @param xSigLen Length of the signature givne in pucSig
 * @return 0 on success
 */
static int p11_ecdsa_verify( void * pvCtx,
                             mbedtls_md_type_t xMdAlg,
                             const unsigned char * pucHash,
                             size_t xHashLen,
                             const unsigned char * pucSig,
                             size_t xSigLen );

/**
 * @brief Check if a public-private pair of keys match
 *
 * @param pvPub pointer to an mbedtls_pk
 * @param pvPrv
 * @param lFRng
 * @param pvPRng
 * @return int
 */
static int p11_ecdsa_check_pair( const void * pvPub,
                                 const void * pvPrv,
                                 int ( * lFRng )( void *, unsigned char *, size_t ),
                                 void * pvPRng );

/**
 * @brief
 *
 * @param pvCtx
 * @param pxItems
 */
static void p11_ecdsa_debug( const void * pvCtx,
                             mbedtls_pk_debug_item * pxItems );

/**
 * @brief
 *
 * @param pucSig
 * @param xSigBufferSize
 * @param pxSigLen
 * @return int
 */
static int prvEcdsaSigToASN1InPlace( unsigned char * pucSig,
                                     size_t xSigBufferSize,
                                     size_t * pxSigLen );

/**
 * @brief
 *
 * @param ppucPosition
 * @param pucStart
 * @param pucOctetStr
 * @param xOctetStrLen
 * @return int
 */
static int prvASN1WriteBigIntFromOctetStr( unsigned char ** ppucPosition,
                                           const unsigned char * pucStart,
                                           const unsigned char * pucOctetStr,
                                           size_t xOctetStrLen );

/*-----------------------------------------------------------*/

/**
 * @brief mbedtls_pk_info_t struct for PKCS#11 ECDSA operations.
 */
mbedtls_pk_info_t mbedtls_pkcs11_pk_ecdsa =
{
    .type               = MBEDTLS_PK_ECKEY,
    .name               = "PKCS#11",
    .get_bitlen         = p11_ecdsa_get_bitlen,
    .can_do             = p11_ecdsa_can_do,
    .verify_func        = p11_ecdsa_verify,
    .sign_func          = p11_ecdsa_sign,
    #if defined( MBEDTLS_ECDSA_C ) && defined( MBEDTLS_ECP_RESTARTABLE )
        .verify_rs_func = NULL,
        .sign_rs_func   = NULL,
    #endif /* MBEDTLS_ECDSA_C && MBEDTLS_ECP_RESTARTABLE */
    .decrypt_func       = NULL,
    .encrypt_func       = NULL,
    .check_pair_func    = p11_ecdsa_check_pair,
    .ctx_alloc_func     = p11_ecdsa_ctx_alloc,
    .ctx_free_func      = p11_ecdsa_ctx_free,
    #if defined( MBEDTLS_ECDSA_C ) && defined( MBEDTLS_ECP_RESTARTABLE )
        .rs_alloc_func  = NULL,
        .rs_free_func   = NULL,
    #endif /* MBEDTLS_ECDSA_C && MBEDTLS_ECP_RESTARTABLE */
    .debug_func         = p11_ecdsa_debug,
};

/*-----------------------------------------------------------*/

static void * p11_ecdsa_ctx_alloc( void )
{
    void * pvCtx = NULL;

    pvCtx = mbedtls_calloc( 1, sizeof( P11EcDsaCtx_t ) );

    if( pvCtx != NULL )
    {
        P11EcDsaCtx_t * pxP11EcDsa = ( P11EcDsaCtx_t * ) pvCtx;

        /* Initialize other fields */
        pxP11EcDsa->xP11PkCtx.pxFunctionList = NULL;
        pxP11EcDsa->xP11PkCtx.xSessionHandle = CK_INVALID_HANDLE;
        pxP11EcDsa->xP11PkCtx.xPkHandle = CK_INVALID_HANDLE;

        mbedtls_ecdsa_init( &( pxP11EcDsa->xMbedEcDsaCtx ) );
    }

    return pvCtx;
}

/*-----------------------------------------------------------*/

CK_RV p11_ecdsa_ctx_init( void * pvCtx,
                          CK_FUNCTION_LIST_PTR pxFunctionList,
                          CK_SESSION_HANDLE xSessionHandle,
                          CK_OBJECT_HANDLE xPkHandle )
{
    CK_RV xResult = CKR_OK;
    P11EcDsaCtx_t * pxP11EcDsaCtx = ( P11EcDsaCtx_t * ) pvCtx;
    mbedtls_ecdsa_context * pxMbedEcDsaCtx = NULL;

    configASSERT( pxFunctionList != NULL );
    configASSERT( xSessionHandle != CK_INVALID_HANDLE );
    configASSERT( xPkHandle != CK_INVALID_HANDLE );

    if( pxP11EcDsaCtx != NULL )
    {
        pxMbedEcDsaCtx = &( pxP11EcDsaCtx->xMbedEcDsaCtx );
    }
    else
    {
        xResult = CKR_FUNCTION_FAILED;
    }

    /* Initialize public EC parameter data from attributes */

    CK_ATTRIBUTE pxAttrs[ 2 ] =
    {
        { .type = CKA_EC_PARAMS, .ulValueLen = 0, .pValue = NULL },
        { .type = CKA_EC_POINT,  .ulValueLen = 0, .pValue = NULL }
    };

    /* Determine necessary size */
    xResult = pxFunctionList->C_GetAttributeValue( xSessionHandle,
                                                   xPkHandle,
                                                   pxAttrs,
                                                   sizeof( pxAttrs ) / sizeof( CK_ATTRIBUTE ) );

    if( xResult == CKR_OK )
    {
        if( pxAttrs[ 0 ].ulValueLen > 0 )
        {
            pxAttrs[ 0 ].pValue = pvPortMalloc( pxAttrs[ 0 ].ulValueLen );
        }

        if( pxAttrs[ 1 ].ulValueLen > 0 )
        {
            pxAttrs[ 1 ].pValue = pvPortMalloc( pxAttrs[ 1 ].ulValueLen );
        }

        xResult = pxFunctionList->C_GetAttributeValue( xSessionHandle,
                                                       xPkHandle,
                                                       pxAttrs,
                                                       2 );
    }

    /* Parse EC Group */
    if( xResult == CKR_OK )
    {
        /*TODO: Parse the ECParameters object */
        int lResult = mbedtls_ecp_group_load( &( pxMbedEcDsaCtx->grp ), MBEDTLS_ECP_DP_SECP256R1 );

        if( lResult != 0 )
        {
            xResult = CKR_FUNCTION_FAILED;
        }
    }

    /* Parse ECPoint */
    if( xResult == CKR_OK )
    {
        unsigned char * pucIterator = pxAttrs[ 1 ].pValue;
        size_t uxLen = pxAttrs[ 1 ].ulValueLen;
        int lResult = 0;

        lResult = mbedtls_asn1_get_tag( &pucIterator, &( pucIterator[ uxLen ] ), &uxLen, MBEDTLS_ASN1_OCTET_STRING );

        if( lResult != 0 )
        {
            xResult = CKR_GENERAL_ERROR;
        }
        else
        {
            lResult = mbedtls_ecp_point_read_binary( &( pxMbedEcDsaCtx->grp ),
                                                     &( pxMbedEcDsaCtx->Q ),
                                                     pucIterator,
                                                     uxLen );
        }

        if( lResult != 0 )
        {
            xResult = CKR_GENERAL_ERROR;
        }
    }

    if( pxAttrs[ 0 ].pValue != NULL )
    {
        vPortFree( pxAttrs[ 0 ].pValue );
    }

    if( pxAttrs[ 1 ].pValue != NULL )
    {
        vPortFree( pxAttrs[ 1 ].pValue );
    }

    if( xResult == CKR_OK )
    {
        pxP11EcDsaCtx->xP11PkCtx.pxFunctionList = pxFunctionList;
        pxP11EcDsaCtx->xP11PkCtx.xSessionHandle = xSessionHandle;
        pxP11EcDsaCtx->xP11PkCtx.xPkHandle = xPkHandle;
    }

    return xResult;
}

/*-----------------------------------------------------------*/

static int prvASN1WriteBigIntFromOctetStr( unsigned char ** ppucPosition,
                                           const unsigned char * pucStart,
                                           const unsigned char * pucOctetStr,
                                           size_t uxOctetStrLen )
{
    size_t uxRequiredLen = 0;
    int lReturn = 0;

    /* Check if zero byte is needed at beginning */
    if( pucOctetStr[ 0 ] > 0x7F )
    {
        uxRequiredLen = uxOctetStrLen + 1;
    }
    else
    {
        uxRequiredLen = uxOctetStrLen;
    }

    if( ( *ppucPosition - uxRequiredLen ) >= pucStart )
    {
        *ppucPosition = *ppucPosition - uxOctetStrLen;

        /* Copy octet string */
        ( void ) memcpy( *ppucPosition, pucOctetStr, uxOctetStrLen );

        /* Prepend additional byte if necessary */
        if( pucOctetStr[ 0 ] > 0x7F )
        {
            *ppucPosition = &( ( *ppucPosition )[ -1 ] );
            **ppucPosition = 0;
        }

        lReturn = mbedtls_asn1_write_len( ppucPosition, pucStart, uxRequiredLen );

        if( lReturn > 0 )
        {
            uxRequiredLen += ( size_t ) lReturn;
            lReturn = mbedtls_asn1_write_tag( ppucPosition, pucStart, MBEDTLS_ASN1_INTEGER );
        }

        if( lReturn > 0 )
        {
            lReturn = ( size_t ) lReturn + uxRequiredLen;
        }
    }

    return lReturn;
}

/*-----------------------------------------------------------*/

/*
 * SEQUENCE LENGTH (of entire rest of signature)
 *      INTEGER LENGTH  (of R component)
 *      INTEGER LENGTH  (of S component)
 */
static int prvEcdsaSigToASN1InPlace( unsigned char * pucSig,
                                     size_t xSigBufferSize,
                                     size_t * pxSigLen )
{
    unsigned char pucTempBuf[ MBEDTLS_ECDSA_MAX_LEN ] = { 0 };
    unsigned char * pucPosition = pucTempBuf + sizeof( pucTempBuf );

    size_t uxLen = 0;
    int lReturn = 0;
    size_t xComponentLen;

    configASSERT( pucSig != NULL );
    configASSERT( pxSigLen != NULL );
    configASSERT( xSigBufferSize > *pxSigLen );

    xComponentLen = ( *pxSigLen ) / 2;

    /* Write "S" portion VLT */
    lReturn = prvASN1WriteBigIntFromOctetStr( &pucPosition, pucTempBuf,
                                              &( pucSig[ xComponentLen ] ), xComponentLen );

    /* Write "R" Portion VLT */
    if( lReturn > 0 )
    {
        uxLen += ( size_t ) lReturn;
        lReturn = prvASN1WriteBigIntFromOctetStr( &pucPosition, pucTempBuf,
                                                  pucSig, xComponentLen );
    }

    if( lReturn > 0 )
    {
        uxLen += ( size_t ) lReturn;
        lReturn = mbedtls_asn1_write_len( &pucPosition, pucTempBuf, uxLen );
    }

    if( lReturn > 0 )
    {
        uxLen += ( size_t ) lReturn;
        lReturn = mbedtls_asn1_write_tag( &pucPosition, pucTempBuf,
                                          MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE );
    }

    if( lReturn > 0 )
    {
        uxLen += ( size_t ) lReturn;
    }

    if( ( lReturn > 0 ) && ( uxLen <= xSigBufferSize ) )
    {
        ( void ) memcpy( pucSig, pucPosition, uxLen );
        *pxSigLen = uxLen;
        lReturn = 0;
    }
    else
    {
        lReturn = -1;
    }

    return lReturn;
}

/*-----------------------------------------------------------*/

static int p11_ecdsa_sign( void * pvCtx,
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
    const P11EcDsaCtx_t * pxEcDsaCtx = NULL;
    const P11PkCtx_t * pxP11Ctx = NULL;
    unsigned char pucHashCopy[ MBEDTLS_MD_MAX_SIZE ];

    CK_MECHANISM xMech =
    {
        .mechanism      = CKM_ECDSA,
        .pParameter     = NULL,
        .ulParameterLen = 0
    };

    /* Unused parameters. */
    ( void ) ( xMdAlg );
    ( void ) ( plRng );
    ( void ) ( pvRng );

    configASSERT( pucSig != NULL );
    configASSERT( xSigBufferSize > 0 );
    configASSERT( pxSigLen != NULL );
    configASSERT( pucHash != NULL );
    configASSERT( xHashLen > 0 );

    if( pvCtx != NULL )
    {
        pxEcDsaCtx = ( P11EcDsaCtx_t * ) pvCtx;
        pxP11Ctx = &( pxEcDsaCtx->xP11PkCtx );
    }
    else
    {
        xResult = CKR_FUNCTION_FAILED;
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
        CK_ULONG ulSigLen = xSigBufferSize;

        ( void ) memcpy( pucHashCopy, pucHash, xHashLen );

        xResult = pxP11Ctx->pxFunctionList->C_Sign( pxP11Ctx->xSessionHandle,
                                                    pucHashCopy, xHashLen,
                                                    pucSig, &ulSigLen );

        if( xResult == CKR_OK )
        {
            *pxSigLen = ulSigLen;
        }
    }

    if( xResult != CKR_OK )
    {
        LogError( ( "Failed to sign message using PKCS #11 with error code %02X.", xResult ) );
        lFinalResult = -1;
    }
    else
    {
        lFinalResult = prvEcdsaSigToASN1InPlace( pucSig, xSigBufferSize, pxSigLen );
    }

    return lFinalResult;
}

/*-----------------------------------------------------------*/

static size_t p11_ecdsa_get_bitlen( const void * pvCtx )
{
    P11EcDsaCtx_t * pxEcDsaCtx = ( P11EcDsaCtx_t * ) pvCtx;

    configASSERT( mbedtls_ecdsa_info.get_bitlen );

    return mbedtls_ecdsa_info.get_bitlen( &( pxEcDsaCtx->xMbedEcDsaCtx ) );
}

/*-----------------------------------------------------------*/

static int p11_ecdsa_can_do( mbedtls_pk_type_t xType )
{
    return( xType == MBEDTLS_PK_ECDSA );
}

/*-----------------------------------------------------------*/

static int p11_ecdsa_verify( void * pvCtx,
                             mbedtls_md_type_t xMdAlg,
                             const unsigned char * pucHash,
                             size_t xHashLen,
                             const unsigned char * pucSig,
                             size_t xSigLen )
{
    P11EcDsaCtx_t * pxEcDsaCtx = ( P11EcDsaCtx_t * ) pvCtx;

    configASSERT( mbedtls_ecdsa_info.verify_func );

    return mbedtls_ecdsa_info.verify_func( &( pxEcDsaCtx->xMbedEcDsaCtx ),
                                           xMdAlg,
                                           pucHash, xHashLen,
                                           pucSig, xSigLen );
}

/*-----------------------------------------------------------*/

static int p11_ecdsa_check_pair( const void * pvPub,
                                 const void * pvPrv,
                                 int ( * lFRng )( void *, unsigned char *, size_t ),
                                 void * pvPRng )
{
    mbedtls_ecp_keypair * pxPubKey = ( mbedtls_ecp_keypair * ) pvPub;
    mbedtls_ecp_keypair * pxPrvKey = ( mbedtls_ecp_keypair * ) pvPrv;

    P11EcDsaCtx_t * pxP11PrvKey = ( P11EcDsaCtx_t * ) pvPrv;
    int lResult = 0;

    ( void ) lFRng;
    ( void ) pvPRng;

    if( ( pxPubKey == NULL ) || ( pxPrvKey == NULL ) )
    {
        lResult = -1;
    }

    if( lResult == 0 )
    {
        if( ( pxPubKey->grp.id == MBEDTLS_ECP_DP_NONE ) ||
            ( pxPubKey->grp.id != pxPrvKey->grp.id ) )
        {
            lResult = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        }
    }

    /* Compare public key points */
    if( lResult == 0 )
    {
        lResult = mbedtls_mpi_cmp_mpi( &( pxPubKey->Q.X ), &( pxPrvKey->Q.X ) );
    }

    if( lResult == 0 )
    {
        lResult = mbedtls_mpi_cmp_mpi( &( pxPubKey->Q.Y ), &( pxPrvKey->Q.Y ) );
    }

    if( lResult == 0 )
    {
        lResult = mbedtls_mpi_cmp_mpi( &( pxPubKey->Q.Z ), &( pxPrvKey->Q.Z ) );
    }

    /* Test sign op */
    if( lResult == 0 )
    {
        unsigned char pucTestHash[ 32 ] =
        {
            0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
            0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
            0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
            0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38
        };
        unsigned char pucTestSignature[ MBEDTLS_ECDSA_MAX_SIG_LEN( 256 ) ] = { 0 };
        size_t uxSigLen = 0;
        lResult = p11_ecdsa_sign( ( void * ) ( void * ) pvPrv, MBEDTLS_MD_SHA256,
                                  pucTestHash, sizeof( pucTestHash ),
                                  pucTestSignature, sizeof( pucTestSignature ), &uxSigLen,
                                  NULL, NULL );

        if( lResult == 0 )
        {
            lResult = mbedtls_ecdsa_read_signature( pxPubKey,
                                                    pucTestHash, sizeof( pucTestHash ),
                                                    pucTestSignature, uxSigLen );
        }
    }

    return lResult;
}

/*-----------------------------------------------------------*/

static void p11_ecdsa_debug( const void * pvCtx,
                             mbedtls_pk_debug_item * pxItems )
{
    P11EcDsaCtx_t * pxEcDsaCtx = ( P11EcDsaCtx_t * ) pvCtx;

    configASSERT( mbedtls_ecdsa_info.debug_func );

    mbedtls_ecdsa_info.debug_func( &( pxEcDsaCtx->xMbedEcDsaCtx ), pxItems );
}

/*-----------------------------------------------------------*/

static void p11_ecdsa_ctx_free( void * pvCtx )
{
    if( pvCtx != NULL )
    {
        P11EcDsaCtx_t * pxP11EcDsa = ( P11EcDsaCtx_t * ) pvCtx;

        mbedtls_ecdsa_free( &( pxP11EcDsa->xMbedEcDsaCtx ) );

        mbedtls_free( pvCtx );
    }
}

/*-----------------------------------------------------------*/
