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
#include "task.h"
#include "semphr.h"
#include "stream_buffer.h"

#define STDOUT_MAX_LEN      ( 1024U )
#define LINE_ENDING         "\n"

/*-----------------------------------------------------------*/

static SemaphoreHandle_t xStdoutMutex = NULL;

/* set aside STDOUT_MAX_LEN + line ending + 1 byte of null terminator */
static uint8_t pcStdoutLineBuffer[ STDOUT_MAX_LEN + sizeof( LINE_ENDING ) ] = { 0 };

/*-----------------------------------------------------------*/

extern void initialise_monitor_handles( void );

/*-----------------------------------------------------------*/

void vPlatformInitLogging( void )
{
    #if configSUPPORT_STATIC_ALLOCATION == 1
        static StaticSemaphore_t xStdoutMutexBuffer;
    #endif /* configSUPPORT_STATIC_ALLOCATION */

    initialise_monitor_handles();

    if( xStdoutMutex == NULL )
    {
        taskENTER_CRITICAL();
        {
            #if configSUPPORT_STATIC_ALLOCATION == 1
                xStdoutMutex = xSemaphoreCreateMutexStatic( &xStdoutMutexBuffer );
            #else
                xStdoutMutex = xSemaphoreCreateMutex();
            #endif

            configASSERT( xStdoutMutex != NULL );
        }
        taskEXIT_CRITICAL();
    }

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

    BaseType_t xSemaphoreTaken = pdFALSE;
    BaseType_t xSchedulerState = xTaskGetSchedulerState();

    if( xSchedulerState == taskSCHEDULER_RUNNING )
    {
        xSemaphoreTaken = xSemaphoreTake( xStdoutMutex, portMAX_DELAY );
    }

    if( ( xSemaphoreTaken == pdTRUE ) ||
        ( xSchedulerState != taskSCHEDULER_RUNNING ) )
    {
        BaseType_t xFoundNewline = pdFALSE;
        va_start( arg, pcFormat );

        lLen = vsnprintf( pcStdoutLineBuffer, STDOUT_MAX_LEN + 1, pcFormat, arg );

        va_end( arg );

        /* Strip any CR or LF characters from end */
        while( lLen > 0 &&
               ( pcStdoutLineBuffer[ lLen - 1U ] == '\r' ||
                 pcStdoutLineBuffer[ lLen - 1U ] == '\n' ) )
        {
            lLen--;
            xFoundNewline = pdTRUE;
        }

        if( xFoundNewline == pdTRUE )
        {
            pcStdoutLineBuffer[ lLen ] = '\n';
            lLen++;
        }

        /* Call write syscall via semihosting */
        write( STDOUT_FILENO, pcStdoutLineBuffer, lLen );

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

        pcStdoutLineBuffer[ lLen ] = '\n';

        lLen++;

        if( lLen > 0 )
        {
            write( STDOUT_FILENO, pcStdoutLineBuffer, lLen );
        }

        ( void ) xSemaphoreGive( xStdoutMutex );
    }
}

/*-----------------------------------------------------------*/
