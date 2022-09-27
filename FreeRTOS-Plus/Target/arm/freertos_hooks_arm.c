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
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"

#define __weak__    __attribute__( ( weak ) )

#if configUSE_MALLOC_FAILED_HOOK == 1U

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
    void vApplicationMallocFailedHook( void );
#endif /* configUSE_MALLOC_FAILED_HOOK == 1U */

#if ( configUSE_IDLE_HOOK == 1U )
    void vApplicationIdleHook( void );
#endif /* configUSE_IDLE_HOOK == 1U */

#if ( ( configCHECK_FOR_STACK_OVERFLOW == 1U ) || ( configCHECK_FOR_STACK_OVERFLOW == 2U ) )
    void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                        char * pcTaskName );
#endif /* configCHECK_FOR_STACK_OVERFLOW */

#if ( configUSE_TICK_HOOK == 1U )
    void vApplicationTickHook( void );
#endif /* configUSE_TICK_HOOK == 1U */

#if ( configSUPPORT_STATIC_ALLOCATION == 1U )
    void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                        StackType_t ** ppxIdleTaskStackBuffer,
                                        uint32_t * pulIdleTaskStackSize );

    void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                         StackType_t ** ppxTimerTaskStackBuffer,
                                         uint32_t * pulTimerTaskStackSize );
#endif /* configSUPPORT_STATIC_ALLOCATION == 1U */

/*-----------------------------------------------------------*/

#if ( configUSE_MALLOC_FAILED_HOOK == 1U )
    void __weak__ vApplicationMallocFailedHook( void )
    {
        /*
         * vApplicationMallocFailedHook() will only be called if
         * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
         * function that will get called if a call to pvPortMalloc() fails.
         * pvPortMalloc() is called internally by the kernel whenever a task, queue,
         * timer or semaphore is created.  It is also called by various parts of the
         * demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
         * size of the    heap available to pvPortMalloc() is defined by
         * configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
         * API function can be used to query the size of free heap space that remains
         * (although it does not provide information on how the remaining heap might be
         * fragmented).  See http://www.freertos.org/a00111.html for more
         * information.
         */
        vAssertCalled( __FILE__, __LINE__ );
    }
#endif /* configUSE_MALLOC_FAILED_HOOK == 1U */

/*-----------------------------------------------------------*/

#if ( configUSE_IDLE_HOOK == 1U )
    void __weak__ vApplicationIdleHook( void )
    {
        /*
         * vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
         * to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
         * task.  It is essential that code added to this hook function never attempts
         * to block in any way (for example, call xQueueReceive() with a block time
         * specified, or call vTaskDelay()).  If application tasks make use of the
         * vTaskDelete() API function to delete themselves then it is also important
         * that vApplicationIdleHook() is permitted to return to its calling function,
         * because it is the responsibility of the idle task to clean up memory
         * allocated by the kernel to any task that has since deleted itself.
         */

        usleep( 500U );
    }
#endif /* configUSE_IDLE_HOOK */

/*-----------------------------------------------------------*/

#if ( ( configCHECK_FOR_STACK_OVERFLOW == 1U ) || ( configCHECK_FOR_STACK_OVERFLOW == 2U ) )
    void __weak__ vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                                 char * pcTaskName )
    {
        ( void ) pcTaskName;
        ( void ) pxTask;

        /*
         * Run time stack overflow checking is performed if
         * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
         * function is called if a stack overflow is detected.  This function is
         * provided as an example only as stack overflow checking does not function
         * when running the FreeRTOS POSIX port.
         */
        vAssertCalled( __FILE__, __LINE__ );
    }
#endif /* ( configCHECK_FOR_STACK_OVERFLOW == 1U ) || ( configCHECK_FOR_STACK_OVERFLOW == 2U ) */

/*-----------------------------------------------------------*/

#if ( configUSE_TICK_HOOK == 1U )
    void __weak__ vApplicationTickHook( void )
    {
        /*
         * This function will be called by each tick interrupt if
         * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
         * added here, but the tick hook is called from an interrupt context, so
         * code must not attempt to block, and only the interrupt safe FreeRTOS API
         * functions can be used (those that end in FromISR()).
         */
    }
#endif /* configUSE_TICK_HOOK == 1U */

/*-----------------------------------------------------------*/

#if ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1U )
    void __weak__ vApplicationDaemonTaskStartupHook( void )
    {
        /*
         * This function will be called once only, when the daemon task starts to
         * execute (sometimes called the timer task). This is useful if the
         * application includes initialisation code that would benefit from executing
         * after the scheduler has been started.
         */
    }
#endif /* configUSE_DAEMON_TASK_STARTUP_HOOK == 1U */

/*-----------------------------------------------------------*/
#if ( configSUPPORT_STATIC_ALLOCATION == 1U )

/*
 * configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task.
 */
    void __weak__ vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                                  StackType_t ** ppxTimerTaskStackBuffer,
                                                  uint32_t * pulTimerTaskStackSize )
    {
        /*
         * If the buffers to be provided to the Timer task are declared inside this
         * function then they must be declared static - otherwise they will be allocated on
         * the stack and so not exists after this function exits.
         */
        static StaticTask_t xTimerTaskTCB;
        static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

        /*
         * Pass out a pointer to the StaticTask_t structure in which the Timer
         * task's state will be stored.
         */
        *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

        /* Pass out the array that will be used as the Timer task's stack. */
        *ppxTimerTaskStackBuffer = uxTimerTaskStack;

        /*
         * Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
         * Note that, as the array is necessarily of type StackType_t,
         * configMINIMAL_STACK_SIZE is specified in words, not bytes.
         */
        *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION == 1U */

/*-----------------------------------------------------------*/

#if ( configSUPPORT_STATIC_ALLOCATION == 1U )

/*
 * configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task.
 */
    void __weak__ vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                                 StackType_t ** ppxIdleTaskStackBuffer,
                                                 uint32_t * pulIdleTaskStackSize )
    {
        /*
         * If the buffers to be provided to the Idle task are declared inside this
         * function then they must be declared static - otherwise they will be allocated on
         * the stack and so not exists after this function exits.
         */
        static StaticTask_t xIdleTaskTCB;
        static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

        /*
         * Pass out a pointer to the StaticTask_t structure in which the Idle task's
         * state will be stored.
         */
        *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

        /* Pass out the array that will be used as the Idle task's stack. */
        *ppxIdleTaskStackBuffer = uxIdleTaskStack;

        /*
         * Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
         * Note that, as the array is necessarily of type StackType_t,
         * configMINIMAL_STACK_SIZE is specified in words, not bytes.
         */
        *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION == 1U  */

/*-----------------------------------------------------------*/
