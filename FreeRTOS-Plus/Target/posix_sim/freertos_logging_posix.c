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

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "message_buffer.h"
#include "wait_for_event.h"

#define STDOUT_MAX_LEN      ( 1024U )

/*-----------------------------------------------------------*/

static void * vStdoutThread( void * pvParameters );

/*-----------------------------------------------------------*/

static SemaphoreHandle_t xStdoutMutex = NULL;
static MessageBufferHandle_t xStdoutBuffer = NULL;
static StaticMessageBuffer_t xStdoutBufferStatic = { 0 };
static uint8_t pucStdoutBufferData[ STDOUT_MAX_LEN + 8U ] = { 0 };
static char pcStdoutLineBuffer[ STDOUT_MAX_LEN + 2 ] = { 0 };
static pthread_t xStdoutThread;
struct event * pxStdoutDataReadyEvent = NULL;


/*-----------------------------------------------------------*/

void vPlatformInitLogging( void )
{
    static StaticSemaphore_t xStdoutMutexBuffer;
    int lRslt = 0;

    if( xStdoutMutex == NULL )
    {
        taskENTER_CRITICAL();
        {
            xStdoutMutex = xSemaphoreCreateMutexStatic( &xStdoutMutexBuffer );
            configASSERT( xStdoutMutex != NULL );
            ( void ) xSemaphoreTake( xStdoutMutex, portMAX_DELAY );
        }
        taskEXIT_CRITICAL();
    }

    if( pxStdoutDataReadyEvent == NULL )
    {
        pxStdoutDataReadyEvent = event_create();
        configASSERT( pxStdoutDataReadyEvent != NULL );
    }

    if( xStdoutBuffer == NULL )
    {
        xStdoutBuffer = xMessageBufferCreateStatic( STDOUT_MAX_LEN, pucStdoutBufferData, &xStdoutBufferStatic );
        configASSERT( xStdoutBuffer != NULL );
    }

    lRslt = pthread_create( &xStdoutThread, NULL, vStdoutThread, NULL );
    configASSERT( lRslt == 0U );

    if( xStdoutMutex != NULL )
    {
        ( void ) xSemaphoreGive( xStdoutMutex );
    }
}

/*-----------------------------------------------------------*/

void vPlatformDeInitLogging( void )
{
    vSemaphoreDelete( xStdoutMutex );
}

/*-----------------------------------------------------------*/

void vLoggingPrintf( const char * pcFormat,
                     ... )
{
    va_list arg;
    int lLen = 0;

    if( ( xTaskGetSchedulerState() != taskSCHEDULER_RUNNING ) ||
        ( xSemaphoreTake( xStdoutMutex, portMAX_DELAY ) == pdTRUE ) )
    {
        va_start( arg, pcFormat );
        lLen = vsnprintf( pcStdoutLineBuffer, STDOUT_MAX_LEN + 1, pcFormat, arg );
        va_end( arg );

        /* Strip any CR or LF characters from end */
        while( lLen > 0 &&
               ( pcStdoutLineBuffer[ lLen - 1U ] == '\r' ||
                 pcStdoutLineBuffer[ lLen - 1U ] == '\n' ) )
        {
            lLen--;
        }

        if( lLen > 0 )
        {
            ( void ) xMessageBufferSend( xStdoutBuffer, pcStdoutLineBuffer, ( size_t ) lLen, portMAX_DELAY );
            event_signal( pxStdoutDataReadyEvent );
        }

        ( void ) xSemaphoreGive( xStdoutMutex );
    }
}

/*-----------------------------------------------------------*/

void vLoggingPrintfLine( const char * pcLogName,
                         const char * pcLogLevel,
                         const char * pcFunctionName,
                         size_t uxLineNumber,
                         const char * pcFormat,
                         ... )
{
    va_list arg;
    int lLen = 0;

    if( ( xTaskGetSchedulerState() != taskSCHEDULER_RUNNING ) ||
        ( xSemaphoreTake( xStdoutMutex, portMAX_DELAY ) == pdTRUE ) )
    {
        if( pcLogLevel == NULL )
        {
            lLen = snprintf( pcStdoutLineBuffer, STDOUT_MAX_LEN, "[%s] ", pcLogName );
        }
        else
        {
            lLen = snprintf( pcStdoutLineBuffer, STDOUT_MAX_LEN, "[%s] %s: ", pcLogName, pcLogLevel );
        }

        if( lLen > 0 &&
            lLen < STDOUT_MAX_LEN )
        {
            va_start( arg, pcFormat );
            lLen += vsnprintf( &( pcStdoutLineBuffer[ lLen ] ), STDOUT_MAX_LEN + 1 - lLen, pcFormat, arg );
            va_end( arg );
        }

        /* Strip any CR or LF characters from end */
        while( lLen > 0 &&
               ( pcStdoutLineBuffer[ lLen - 1U ] == '\r' ||
                 pcStdoutLineBuffer[ lLen - 1U ] == '\n' ) )
        {
            lLen--;
        }

        if( ( lLen > 0 ) &&
            ( lLen < STDOUT_MAX_LEN ) &&
            ( pcLogLevel != NULL ) )
        {
            lLen += snprintf( &( pcStdoutLineBuffer[ lLen ] ), STDOUT_MAX_LEN + 1 - lLen, "    (%s:%lu)", pcFunctionName, uxLineNumber );
        }

        if( lLen > 0 )
        {
            ( void ) xMessageBufferSend( xStdoutBuffer, pcStdoutLineBuffer, ( size_t ) lLen, portMAX_DELAY );
            event_signal( pxStdoutDataReadyEvent );
        }

        ( void ) xSemaphoreGive( xStdoutMutex );
    }
}

/*-----------------------------------------------------------*/

static void * vStdoutThread( void * pvParameters )
{
    static char pucLineBuffer[ STDOUT_MAX_LEN + 4U ];
    sigset_t set;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /*
     * disable signals to avoid treating this thread as a FreeRTOS task and putting
     * it to sleep by the scheduler
     */
    sigfillset( &set );
    pthread_sigmask( SIG_SETMASK, &set, NULL );

    for( ; ; )
    {
        event_wait_timed( pxStdoutDataReadyEvent, 1000U );

        while( xMessageBufferIsEmpty( xStdoutBuffer ) == pdFALSE )
        {
            size_t uxLogMsgLen = xMessageBufferReceiveFromISR( xStdoutBuffer,
                                                               pucLineBuffer,
                                                               STDOUT_MAX_LEN,
                                                               &xHigherPriorityTaskWoken );

            if( uxLogMsgLen > 0 )
            {
                pucLineBuffer[ uxLogMsgLen ] = '\n';
                uxLogMsgLen += 1;
                pucLineBuffer[ uxLogMsgLen ] = '\00';
            }

            if( uxLogMsgLen > 0 )
            {
                ( void ) fwrite( pucLineBuffer, 1U, uxLogMsgLen, stdout );
            }

            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    }
    return NULL;
}
