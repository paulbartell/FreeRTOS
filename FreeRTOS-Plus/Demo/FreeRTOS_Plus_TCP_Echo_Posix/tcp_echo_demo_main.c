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

/* #define SdkLog vLoggingPrintf */
#include "FreeRTOSConfig.h"
#include "logging_levels.h"

#define LIBRARY_LOG_NAME     "TCPEchoDemo"
#define LIBRARY_LOG_LEVEL    LOG_INFO

#include "logging_stack.h"

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "FreeRTOSIPConfig.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

extern void vTCPEchoClientTask( void * pvParameters );
extern void vApplicationInitLogging( void );
extern void vApplicationInitIpStack( void );

static void vInitialTask( void * pvParameters )
{
    BaseType_t xResult = pdFALSE;

    vApplicationInitIpStack();

    xResult = xTaskCreate( vTCPEchoClientTask, "TCPEchoClient", 1024U * 8, NULL, 2U, NULL );

    configASSERT( xResult == pdTRUE );

    /* xResult = xTaskCreate( vTCPEchoServerTask, "TCPEchoServeer", 1024U * 8U, NULL, 2U, NULL ); */

    vTaskDelete( NULL );
}

int main( void )
{
    vApplicationInitLogging();

    vLoggingPrintf( "Logging Initialized" );

    xTaskCreate( vInitialTask, "INIT", 1024U, NULL, 4U, NULL );

    vTaskStartScheduler();

    for( ; ; )
    {
        __asm volatile ( "NOP" );
    }
}
