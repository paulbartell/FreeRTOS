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

#ifndef ARCH_ARMV8_M_MAIN_H
#define ARCH_ARMV8_M_MAIN_H

#define __ARM_ARCH_6M__                         0U
#define __ARM_ARCH_7M__                         0U
#define __ARM_ARCH_7EM__                        0U
#define __ARM_ARCH_8M_BASE__                    0U

#ifndef __ARM_ARCH_8M__
    #define __ARM_ARCH_8M__                     1U
#endif /* __ARM_ARCH_8M__ */

#ifndef __ARM_ARCH_8M_MAIN__
    #define __ARM_ARCH_8M_MAIN__                1U
#endif /* __ARM_ARCH_8M_MAIN__ */

/*
 * ARMv8M Main Required Features
 */
#define __SYSTICK_PRESENT                       1U
#define __FAULTMASK_BASEPRI_PRESENT             1U
#define __NVIC_PRESENT                          1U
#ifndef __NVIC_EXTI_MAX
    #define __NVIC_EXTI_MAX                     496U
#endif /* __NVIC_EXTI_MAX */

/*
 * ARMv8M Main Optional Features
 */

/* Optional: Vector Table Offset Register */
#ifndef __VTOR_PRESENT
    #define __VTOR_PRESENT                      0U
#endif /* __VTOR_PRESENT */

/* Optional: Memory Protection Unit */
#ifndef __MPU_PRESENT
    #define __MPU_PRESENT                       0U
#elif( __MPU_PRESENT == 1U )
    #define __MPU_TYPE_PMSAV6                   0U
    #define __MPU_TYPE_PMSAV7                   0U

    #define __MPU_TYPE_PMSAV8                   1U
    #define __MPU_HAVE_BACKGROUND_REGION        1U

    /* Alias Registers required for ARMv8M Main Extension */
    #define __MPU_HAVE_ALIAS_REGS               1U

    #ifndef __MPU_MIN_REGIONS
        #define __MPU_MIN_REGIONS               0U
    #endif /* __MPU_MIN_REGIONS */

    #ifndef __MPU_MAX_REGIONS
        #define __MPU_MAX_REGIONS               0U
    #endif /* __MPU_MAX_REGIONS */

    #ifndef __MPU_NUM_REGIONS
        #define __MPU_NUM_REGIONS               __MPU_MIN_REGIONS
    #endif /* __MPU_NUM_REGIONS */
#endif /* __MPU_PRESENT */

/* Optional: Floating Point Unit */
#ifndef __FPU_PRESENT
    #define __FPU_PRESENT                       0U
#elif( __FPU_PRESENT == 1U )
    #ifndef __FPU_D16
        #define __FPU_D16                       1U
    #endif /* __FPU_D16 */

    #ifndef __FPU_D32
        #define __FPU_D32                       0U
    #endif /* __FPU_D32 */

    #ifndef __FPU_PRECISION_SINGLE
        #define __FPU_PRECISION_SINGLE          1U
    #endif /* __FPU_PRECISION_SINGLE */

    #ifndef __FPU_PRECISION_DOUBLE
        #define __FPU_PRECISION_DOUBLE          0U
    #endif /* __FPU_PRECISION_DOUBLE */

    #ifndef __FPU_PRECISION_HALF
        #define __FPU_PRECISION_HALF            0U
    #endif
#endif

/* Optional: Instruction Tightly Coupled Memory */
#ifndef __ITCM_PRESENT
    #define __ITCM_PRESENT                      0U
#endif /* __ITCM_PRESENT */

/* Optional: Data Tightly Coupled Memory */
#ifndef __DTCM_PRESENT
    #define __DTCM_PRESENT                      0U
#endif /* __DTCM_PRESENT */

/* Optional: ARMv7EM / ARMv8M Instruction Cache */
#ifndef __ICACHE_PRESENT
    #define __ICACHE_PRESENT                    0U
#endif /* __ICACHE_PRESENT */

/* Optional: ARMv7EM / ARMv8M Data Cache */
#ifndef __DCACHE_PRESENT
    #define __DCACHE_PRESENT                    0U
#endif /* __DCACHE_PRESENT */

/* Optional: ARMv8-M Cortex-M Security Extensions */
#ifndef __CMSE_PRESENT
    #define __CMSE_PRESENT                      0U
#endif /* __CMSE_PRESENT */

#define __MVE_PRESENT                           0U

#include "arch_cortex_m.h"

#endif /* ! defined( ARCH_ARMV8_M_MAIN_H ) */
