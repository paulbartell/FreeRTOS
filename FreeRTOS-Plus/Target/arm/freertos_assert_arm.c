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

/* C Standard Library */
// #include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------*/

#if defined( configASSERT )
    void vAssertCalled( const char * const pcFileName,
                        unsigned long ulLine )
    {
        /* Copy the parameters to local volatile variables, just for debugging */
        volatile char * pcFile = ( volatile char * ) pcFileName;
        volatile uint32_t ulLineNumber = ulLine;

        /*
         * Called if an assertion passed to configASSERT() fails.  See
         * http://www.freertos.org/a00110.html#configASSERT for more information.
         */

        printf("vAssertCalled( %s, %lu )\n", pcFileName, ulLine );

        taskENTER_CRITICAL();
        {
            #if defined(__has_builtin)
                #if __has_builtin(__builtin_debugtrap)
                    __builtin_debugtrap();
                #elif __has_builtin(__builtin_trap)
                    __builtin_trap();
                #endif
            #endif
        }
        taskEXIT_CRITICAL();
    }
#endif /* defined( configASSERT ) */

/*-----------------------------------------------------------*/
