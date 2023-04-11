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

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdio.h>

/*-----------------------------------------------------------*/

__attribute__( ( naked, aligned( 8 ) ) )
static void vCauseHardFault( void )
{
    __asm volatile(
        "ldr r0, =0xFFFFFFFF;"
        "ldr r1, [r0];"
    );
}

/*-----------------------------------------------------------*/

int main( int argc, char * argv[] )
{
    ( void ) argc;
    ( void ) argv;

    printf("Calling vCauseHardFault, address: 0x%08X\n", ( unsigned int )( uintptr_t ) vCauseHardFault );

    vCauseHardFault();

    /* Start the tasks and timer running. */
    vTaskStartScheduler();


    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the Idle and/or
     * timer tasks to be created.  See the memory management section on the
     * FreeRTOS web site for more details on the FreeRTOS heap
     * https://www.FreeRTOS.org/a00111.html. */
    for( ; ; )
    {
    }
}
