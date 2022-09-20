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

#define LIBRARY_LOG_NAME     "SNTP Demo"
#define LIBRARY_LOG_LEVEL    LOG_INFO

#include "logging_stack.h"

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "FreeRTOSIPConfig.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#include "demo_config.h"
#include "common_demo_include.h"

extern void vApplicationInitLogging( void );
extern void vApplicationInitIpStack( void );

static void vInitialTask( void * pvParameters )
{
    BaseType_t xResult = pdFALSE;

    vApplicationInitIpStack();

    LogMsg( ( "Waiting for network UP event..." ) );

    while( FreeRTOS_IsNetworkUp() == pdFALSE )
    {
        vTaskDelay( pdMS_TO_TICKS( 1000U ) );
    }

    initializeSystemClock();

    /* Create the SNTP client task that is responsible for synchronizing system time with the time servers
     * periodically. This is created as a high priority task to keep the SNTP client operation unhindered. */
    xResult = xTaskCreate( sntpTask,                 /* Function that implements the task. */
                           "SntpClientTask",         /* Text name for the task - only used for debugging. */
                           democonfigDEMO_STACKSIZE, /* Size of stack (in words, not bytes) to allocate for the task. */
                           NULL,                     /* Task parameter - not used in this case. */
                           configMAX_PRIORITIES - 1, /* Task priority, must be between 0 and configMAX_PRIORITIES - 1. */
                           NULL );

    configASSERT( xResult == pdTRUE );

    /* Create the task that represents an application needing wall-clock time. */
    xResult = xTaskCreate( sampleAppTask,            /* Function that implements the task. */
                           "SampleAppTask",          /* Text name for the task - only used for debugging. */
                           democonfigDEMO_STACKSIZE, /* Size of stack (in words, not bytes) to allocate for the task. */
                           NULL,                     /* Task parameter - not used in this case. */
                           tskIDLE_PRIORITY,         /* Task priority, must be between 0 and configMAX_PRIORITIES - 1. */
                           NULL );                   /* Used to pass out a handle to the created task - not used in this case. */

    configASSERT( xResult == pdTRUE );

    while(1)
    {
        LogInfo( ( "Tick" ) );
        vTaskDelay(1000);
    }


    vTaskDelete( NULL );
}

int main( void )
{
    vApplicationInitLogging();

    xTaskCreate( vInitialTask, "INIT", 1024U, NULL, 4U, NULL );

    vTaskStartScheduler();

    for( ; ; )
    {
        portNOP();
    }
}
