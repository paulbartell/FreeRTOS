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

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t xStdioMutex = NULL;

/*-----------------------------------------------------------*/

#if defined( configASSERT )
    void vAssertCalled( const char * const pcFileName,
                        unsigned long ulLine )
    {
        volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
        /* Copy the parameters to local volatile variables, just for debugging */
        volatile char * pcFile = ( volatile char * ) pcFileName;
        volatile uint32_t ulLineNumber = ulLine;

        /*
         * Called if an assertion passed to configASSERT() fails.  See
         * http://www.freertos.org/a00110.html#configASSERT for more information.
         */

        printf( "vAssertCalled( %s, %lu )\n", pcFileName, ulLine );

        taskENTER_CRITICAL();
        {
            /*
             * You can step out of this function to debug the assertion by using
             * the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
             * value.
             */
            while( ulSetToNonZeroInDebuggerToContinue == 0 )
            {
                __asm volatile ( "NOP" );
                __asm volatile ( "NOP" );
            }
        }
        taskEXIT_CRITICAL();
    }
#endif /* defined( configASSERT ) */

/*-----------------------------------------------------------*/

void vApplicationInitLogging( void )
{
    static StaticSemaphore_t xStdioMutexBuffer;

    if( xStdioMutex == NULL )
    {
        xStdioMutex = xSemaphoreCreateMutexStatic( &xStdioMutexBuffer );
    }
}

/*-----------------------------------------------------------*/

void vLoggingPrintf( const char * pcFormat,
                     ... )
{
    va_list arg;

    if( ( xTaskGetSchedulerState() != taskSCHEDULER_RUNNING ) ||
        ( xSemaphoreTake( xStdioMutex, portMAX_DELAY ) == pdTRUE ) )
    {
        va_start( arg, pcFormat );
        vprintf( pcFormat, arg );
        va_end( arg );

        ( void ) xSemaphoreGive( xStdioMutex );
    }
}
