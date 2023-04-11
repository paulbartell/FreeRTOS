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

#ifndef ARM_CORTEX_M4_H
#define ARM_CORTEX_M4_H

#ifndef __ARM_ARCH_7M__
    #define __ARM_ARCH_7M__                 1U
#endif /* __ARM_ARCH_7M__ */

#ifndef __ARM_ARCH_7EM__
    #define __ARM_ARCH_7EM__                1U
#endif /* __ARM_ARCH_7EM__ */

/*
 * Cortex-M4 Required Features
 */
#define __SYSTICK_PRESENT                   1U
#define __DSP_PRESENT                       1U

#ifndef __NVIC_EXTI_MAX
    #define __NVIC_EXTI_MAX                 240U
#elif( __NVIC_EXTI_MAX > 240U )
    #error __NVIC_EXTI_MAX may not exceed 240 for Cortex-M4
#endif

/*
 * Cortex-M4 Optional Features
 */

/* Optional: Single-Precision Floating Point Unit */
#ifndef __FPU_PRESENT
    #define __FPU_PRESENT                   0U
#else
    #define __FPU_D16                       1U
    #define __FPU_D32                       0U
    #define __FPU_PRECISION_SINGLE          1U
    #define __FPU_PRECISION_DOUBLE          0U
    #define __FPU_PRECISION_HALF            0U
#endif

/* Optional: Memory Protection Unit */
#ifndef __MPU_PRESENT
    #define __MPU_PRESENT                   0U
#else
    #define __MPU_TYPE_PMSAV7               1U
    #define __MPU_HAVE_ALIAS_REGS           1U
    #define __MPU_HAVE_BACKGROUND_REGION    1U
    #define __MPU_MIN_REGIONS               8U
    #define __MPU_MAX_REGIONS               8U
    #define __MPU_NUM_REGIONS               8U
#endif

#ifndef __VTOR_PRESENT
    #define __VTOR_PRESENT                  1U
#endif /* __VTOR_PRESENT */

/* ARMv7EM features not supported by Cortex-M4 */
#define __ITCM_PRESENT                      0U
#define __DTCM_PRESENT                      0U
#define __ICACHE_PRESENT                    0U
#define __DCACHE_PRESENT                    0U

#include "arch_armv7m.h"

#endif /* ! defined( ARM_CORTEX_M4_H ) */
