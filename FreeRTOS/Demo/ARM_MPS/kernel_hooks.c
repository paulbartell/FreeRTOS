/*
 * FreeRTOS V202212.00
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

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#define __WEAK__ __attribute__ ( ( weak ) )

/*-----------------------------------------------------------*/

void __WEAK__ vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
     * free memory available in the FreeRTOS heap.  pvPortMalloc() is called
     * internally by FreeRTOS API functions that create tasks, queues, software
     * timers, and semaphores.  The size of the FreeRTOS heap is set by the
     * configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    taskDISABLE_INTERRUPTS();

    printf("Call to malloc failed\n");

    for( ; ; )
    {
    }
}

/*-----------------------------------------------------------*/

void __WEAK__ vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                             char * pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
     * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     * function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();

    printf( "Stack Overflow in task: %s\n", pcTaskName );
    fflush( NULL );

    exit( 1 );
}

/*-----------------------------------------------------------*/

void __WEAK__ vAssertCalled( const char * pcFileName,
                             int line )
{
    taskDISABLE_INTERRUPTS();
    printf( "Assertion failed at %s: %d\n", pcFileName, line );
    fflush( NULL );

    exit( 1 );
}

/*-----------------------------------------------------------*/

void __WEAK__ vApplicationIdleHook( void )
{
}

/*-----------------------------------------------------------*/

void __WEAK__ vApplicationTickHook( void )
{
}

/*-----------------------------------------------------------*/

void __WEAK__ vLoggingPrintf( const char * pcFormat,
                              ... )
{
    va_list arg;

    va_start( arg, pcFormat );
    vprintf( pcFormat, arg );
    va_end( arg );
}

/*-----------------------------------------------------------*/

void __WEAK__ vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                             StackType_t ** ppxIdleTaskStackBuffer,
                                             uint32_t * pulIdleTaskStackSize )
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/*-----------------------------------------------------------*/

void __WEAK__ vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                              StackType_t ** ppxTimerTaskStackBuffer,
                                              uint32_t * pulTimerTaskStackSize )
{
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];
    static StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
