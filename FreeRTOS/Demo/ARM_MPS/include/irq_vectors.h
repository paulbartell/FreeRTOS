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

#ifndef IRQ_VECTORS_H
#define IRQ_VECTORS_H

#include "stddef.h"

/* Type Definitions */
typedef void ( IRQHandler_t ) ( void );

typedef struct
{
    size_t * pEndOfStack;
    IRQHandler_t * Reset;
    IRQHandler_t * NMI;
    IRQHandler_t * HardFault;
    IRQHandler_t * MemManageFault;
    IRQHandler_t * BusFault;
    IRQHandler_t * UsageFault;
    IRQHandler_t * SecureFault;
    IRQHandler_t * reserved8;
    IRQHandler_t * reserved9;
    IRQHandler_t * reserved10;
    IRQHandler_t * SVC;
    IRQHandler_t * DebugMonitor;
    IRQHandler_t * reserved13;
    IRQHandler_t * PendSV;
    IRQHandler_t * SysTick;
} CoreVectorTable_t;

extern IRQHandler_t Reset_Handler;
extern IRQHandler_t Default_Handler;
extern IRQHandler_t HardFault_Handler;
extern IRQHandler_t MemManageFault_Handler;
extern IRQHandler_t BusFault_Handler;
extern IRQHandler_t UsageFault_Handler;
extern IRQHandler_t DebugMonitor_Handler;
extern IRQHandler_t SecureFault_Handler;

#if( ( __ARM_ARCH_6M__ == 1 ) || ( __ARM_ARCH_7M__ == 1 ) || ( __ARM_ARCH_7EM__ == 1 ) )
    extern IRQHandler_t vPortSVCHandler;
    extern IRQHandler_t xPortPendSVHandler;
    extern IRQHandler_t xPortSysTickHandler;
#elif __ARM_ARCH_8M__ == 1
    extern IRQHandler_t SVC_Handler;
    extern IRQHandler_t PendSV_Handler;
    extern IRQHandler_t SysTick_Handler;
#endif /* __ARM_ARCH_8M__ */

extern const CoreVectorTable_t core_irq_vectors_rom;
extern CoreVectorTable_t core_irq_vectors;

#endif /* IRQ_VECTORS_H */
