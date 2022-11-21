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

#include "logging_levels.h"

#define LIBRARY_LOG_NAME     "MbedTLS_P11"
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
#include "mbedtls/ecdsa.h"
#include "pk_wrap.h"

#include "core_pkcs11_config.h"
#include "core_pkcs11.h"
#include "mbedtls_pk_pkcs11.h"

/*-----------------------------------------------------------*/

CK_RV xPKCS11_initMbedtlsPkContext( mbedtls_pk_context * pxMbedtlsPkCtx,
                                    CK_SESSION_HANDLE xSessionHandle,
                                    CK_OBJECT_HANDLE xPkHandle )
{
    CK_RV xResult = CKR_OK;

    CK_KEY_TYPE xKeyType = CKK_VENDOR_DEFINED;
    CK_FUNCTION_LIST_PTR pxFunctionList = NULL;

    if( pxMbedtlsPkCtx == NULL )
    {
        xResult = CKR_ARGUMENTS_BAD;
    }
    else if( xSessionHandle == CK_INVALID_HANDLE )
    {
        xResult = CKR_SESSION_HANDLE_INVALID;
    }
    else if( xPkHandle == CK_INVALID_HANDLE )
    {
        xResult = CKR_KEY_HANDLE_INVALID;
    }
    else if( ( C_GetFunctionList( &pxFunctionList ) != CKR_OK ) ||
             ( pxFunctionList == NULL ) ||
             ( pxFunctionList->C_GetAttributeValue == NULL ) )
    {
        xResult = CKR_FUNCTION_FAILED;
    }
    /* Determine key type */
    else
    {
        CK_ATTRIBUTE xAttrTemplate =
        {
            .pValue     = &xKeyType,
            .type       = CKA_KEY_TYPE,
            .ulValueLen = sizeof( CK_KEY_TYPE )
        };

        xResult = pxFunctionList->C_GetAttributeValue( xSessionHandle,
                                                       xPkHandle,
                                                       &xAttrTemplate,
                                                       sizeof( xAttrTemplate ) / sizeof( CK_ATTRIBUTE ) );
    }

    if( xResult == CKR_OK )
    {
        xResult = CKR_FUNCTION_FAILED;

        switch( xKeyType )
        {
            case CKK_ECDSA:
                pxMbedtlsPkCtx->pk_ctx = p11_ecdsa_ctx_alloc();

                if( pxMbedtlsPkCtx->pk_ctx != NULL )
                {
                    xResult = p11_ecdsa_ctx_init( pxMbedtlsPkCtx->pk_ctx,
                                                  pxFunctionList, xSessionHandle, xPkHandle );
                }

                if( xResult == CKR_OK )
                {
                    pxMbedtlsPkCtx->pk_info = &mbedtls_pkcs11_pk_ecdsa;
                }
                else
                {
                    p11_ecdsa_ctx_free( pxMbedtlsPkCtx->pk_ctx );
                    pxMbedtlsPkCtx->pk_ctx = NULL;
                    pxMbedtlsPkCtx->pk_info = NULL;
                }

                break;

            case CKK_RSA:
                pxMbedtlsPkCtx->pk_ctx = p11_rsa_ctx_alloc();

                if( pxMbedtlsPkCtx->pk_ctx != NULL )
                {
                    xResult = p11_rsa_ctx_init( pxMbedtlsPkCtx->pk_ctx,
                                                pxFunctionList, xSessionHandle, xPkHandle );
                }

                if( xResult == CKR_OK )
                {
                    pxMbedtlsPkCtx->pk_info = &mbedtls_pkcs11_pk_rsa;
                }
                else
                {
                    p11_rsa_ctx_free( pxMbedtlsPkCtx->pk_ctx );
                    pxMbedtlsPkCtx->pk_ctx = NULL;
                    pxMbedtlsPkCtx->pk_info = NULL;
                }

                break;

            default:
                pxMbedtlsPkCtx->pk_ctx = NULL;
                pxMbedtlsPkCtx->pk_info = NULL;
                break;
        }
    }

    return xResult;
}

/*-----------------------------------------------------------*/

int lPKCS11PkMbedtlsCloseSessionAndFree( mbedtls_pk_context * pxMbedtlsPkCtx )
{
    CK_RV xResult = CKR_OK;
    P11PkCtx_t * pxP11Ctx = NULL;
    CK_FUNCTION_LIST_PTR pxFunctionList = NULL;

    configASSERT( pxMbedtlsPkCtx );

    if( pxMbedtlsPkCtx )
    {
        if( pxMbedtlsPkCtx->pk_info.type == MBEDTLS_PK_ECKEY )
        {
            pxP11Ctx = &( ( ( P11EcDsaCtx_t * ) ( pxMbedtlsPkCtx->pk_ctx ) )->xP11PkCtx );
        }
        else if( pxMbedtlsPkCtx->pk_info.type == MBEDTLS_PK_RSA )
        {
            pxP11Ctx = &( ( ( P11RsaCtx_t * ) ( pxMbedtlsPkCtx->pk_ctx ) )->xP11PkCtx );
        }
        else
        {
            pxP11Ctx = NULL;
            xResult = CKR_FUNCTION_FAILED;
        }
    }
    else
    {
        xResult = CKR_FUNCTION_FAILED;
    }

    if( xResult == CKR_OK )
    {
        xResult = C_GetFunctionList( &pxFunctionList );
    }

    if( xResult == CKR_OK )
    {
        configASSERT( pxFunctionList );
        xResult = pxFunctionList->C_CloseSession( pxP11Ctx->xSessionHandle );
    }

    if( xResult == CKR_OK )
    {
        pxP11Ctx->xSessionHandle = CK_INVALID_HANDLE;
    }

    return( xResult == CKR_OK ? 0 : -1 );
}

/*-----------------------------------------------------------*/

int lPKCS11RandomCallback( void * pvCtx,
                           unsigned char * pucOutput,
                           size_t uxLen )
{
    int lRslt;
    CK_FUNCTION_LIST_PTR pxFunctionList = NULL;
    CK_SESSION_HANDLE * pxSessionHandle = ( CK_SESSION_HANDLE * ) pvCtx;

    if( pucOutput == NULL )
    {
        lRslt = -1;
    }
    else if( pvCtx == NULL )
    {
        lRslt = -1;
        LogError( ( "pvCtx must not be NULL." ) );
    }
    else
    {
        lRslt = ( int ) C_GetFunctionList( &pxFunctionList );
    }

    if( ( lRslt != CKR_OK ) ||
        ( pxFunctionList == NULL ) ||
        ( pxFunctionList->C_GenerateRandom == NULL ) )
    {
        lRslt = -1;
    }
    else
    {
        lRslt = ( int ) pxFunctionList->C_GenerateRandom( *pxSessionHandle, pucOutput, uxLen );
    }

    return lRslt;
}

/*-----------------------------------------------------------*/

