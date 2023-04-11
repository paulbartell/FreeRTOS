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

#ifndef ARCH_ARMV6_M_H
#define ARCH_ARMV6_M_H

#define __ARM_ARCH_7M__                         0U
#define __ARM_ARCH_7EM__                        0U
#define __ARM_ARCH_8M__                         0U
#define __ARM_ARCH_8M_BASE__                    0U
#define __ARM_ARCH_8M_MAIN__                    0U

#ifndef __ARM_ARCH_6M__
    #define __ARM_ARCH_6M__                     1U
#endif /* __ARM_ARCH_6M__ */

/*
 * ARMv6M Required Features
 */
#define __NVIC_PRESENT                          1U
#define __NVIC_PRIO_BITS                        2U
#ifndef __NVIC_EXTI_MAX
    #define __NVIC_EXTI_MAX                     32U
#elif( __NVIC_EXTI_MAX  > 32U )
    #error __NVIC_EXTI_MAX may not exceed 32U for ARMv6M.
#endif

#ifndef __NVIC_EXTI_NUM
    #define __NVIC_EXTI_NUM                     __NVIC_EXTI_MAX
#endif /* __NVIC_EXTI_NUM */

/*
 * ARMv6M Optional Features
 */

/* Optional: SysTick Timer */
#ifndef __SYSTICK_PRESENT
    #define __SYSTICK_PRESENT                   1U
#endif /* __SYSTICK_PRESENT */

/* Optiona: Vector Table Offset Register */
#ifndef __VTOR_PRESENT
    #define __VTOR_PRESENT                      0U
#endif /* __VTOR_PRESENT */

/* Optional: Memory Protection Unit  */
#ifndef __MPU_PRESENT
    #define __MPU_PRESENT                       0U
#elif( __MPU_PRESENT == 1U )
    #define __MPU_TYPE_PMSAV7                   0U
    #define __MPU_TYPE_PMSAV8                   0U
    #define __MPU_TYPE_PMSAV6                   1U
    #define __MPU_HAVE_BACKGROUND_REGION        1U
    #define __MPU_HAVE_ALIAS_REGS               0U
#endif /* __MPU_PRESENT */

/* Optional: Instruction Tightly Coupled Memory */
#ifndef __ITCM_PRESENT
    #define __ITCM_PRESENT                      0U
#endif /* __DTCM_PRESENT */

/* Optional: Data Tightly Coupled Memory */
#ifndef __DTCM_PRESENT
    #define __DTCM_PRESENT                      0U
#endif

/* ARMv7M features, Never present in ARMv6M */
#define __FAULTMASK_BASEPRI_PRESENT             0U
#define __DSP_PRESENT                           0U
#define __FPU_PRESENT                           0U
#define __ICACHE_PRESENT                        0U
#define __DCACHE_PRESENT                        0U

/* ARMv8M Features, Never present in ARMv6M */
#define __CMSE_PRESENT                          0U
#define __MVE_PRESENT                           0U
#define __MVE_FP_PRESENT                        0U

#include "arch_cortex_m.h"

#endif /* ! defined( ARCH_ARMV6_M_H ) */
