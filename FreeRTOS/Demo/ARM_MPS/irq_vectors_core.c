/*
 * FreeRTOS
 * Copyright (C) 2023 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#include <stddef.h>
#include "irq_vectors.h"

_Static_assert( sizeof( CoreVectorTable_t ) == 64, "CoreVectorTable_t must be 64 bytes in length." );

/* Global Variables */
extern size_t __StackTop;

__attribute__( ( section( ".rom_vectors" ), used ) )
const CoreVectorTable_t core_irq_vectors_rom  =
{
    &( __StackTop ),
    Reset_Handler,
    Default_Handler,
    HardFault_Handler,
    MemManageFault_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    SecureFault_Handler,
    NULL,
    NULL,
    NULL,
#if __ARM_ARCH_8M__ == 1U
    SVC_Handler,
#else
    vPortSVCHandler,
#endif /* __ARM_ARCH_8M__ == 1U */

    DebugMonitor_Handler,
    NULL,

#if __ARM_ARCH_8M__ == 1U
    PendSV_Handler,
    SysTick_Handler,
#else
    xPortPendSVHandler,
    xPortSysTickHandler
#endif /* __ARM_ARCH_8M__ == 1U */
};

__attribute__( ( section( ".ram_vectors" ), used ) )
CoreVectorTable_t core_irq_vectors_ram;

__attribute__( ( section( ".ram_vectors" ), used ) )
IRQHandler_t * platform_irq_vectors_ram[ 128 ];
