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

#ifndef ARM_CORTEX_M_DEFAULTS_H
#define ARM_CORTEX_M_DEFAULTS_H

#ifndef __ARM_ARCH_6M__
    #define __ARM_ARCH_6M__                     0U
#endif /* __ARM_ARCH_6M__ */

#ifndef __ARM_ARCH_7M__
    #define __ARM_ARCH_7M__                     0U
#endif /* __ARM_ARCH_7M__ */

#ifndef __ARM_ARCH_7EM__
    #define __ARM_ARCH_7EM__                    0U
#endif /* __ARM_ARCH_7EM__ */

#ifndef __ARM_ARCH_8M__
    #define __ARM_ARCH_8M__                     0U
#endif /* __ARM_ARCH_8M__ */

#ifndef __ARM_ARCH_8M_BASE__
    #define __ARM_ARCH_8M_BASE__                0U
#endif /* __ARM_ARCH_8M_BASE__ */

#ifndef __ARM_ARCH_8M_MAIN__
    #define __ARM_ARCH_8M_MAIN__                0U
#endif /* __ARM_ARCH_8M_MAIN__ */

#ifndef __SYSTICK_PRESENT
    #define __SYSTICK_PRESENT                   1U
#endif /* __SYSTICK_PRESENT */

/*
 * ARM Cortex-M Required Features:
 */

#ifndef __NVIC_PRESENT
    #define __NVIC_PRESENT                      1U
#elif( __NVIC_PRESENT == 0U )
    #error __NVIC_PRESENT is required for all Cortex-M MCUs.
#endif

#ifndef __NVIC_PRIO_BITS
    #error __NVIC_PRIO_BITS must be defined
#elif __NVIC_PRIO_BITS > 8U
    #error __NVIC_PRIO_BITS must be <= 8
#endif /* __NVIC_PRIO_BITS */

#ifndef __NVIC_EXTI_MAX
    #define __NVIC_EXTI_MAX                     496U
#elif __NVIC_EXTI_MAX > 496U
    #error __NVIC_EXTI_MAX must be less than 496U for all Cortex-M MCUs.
#endif /* __NVIC_EXTI_MAX */

#ifndef __NVIC_EXTI_NUM
    #error __NVIC_EXTI_NUM must be defined
#elif __NVIC_EXTI_NUM > __NVIC_EXTI_MAX
    #error __NVIC_EXTI_NUM may not exceed __NVIC_EXTI_MAX.
#endif

/*
 * ARM Cortex-M Optional Features:
 */

/* Optional: Vector Table Offset Register */
#ifndef __VTOR_PRESENT
    #define __VTOR_PRESENT                      0U
#endif /* __VTOR_PRESENT */

/* Optional: Instruction Tightly Coupled Memory */
#ifndef __ITCM_PRESENT
    #define __ITCM_PRESENT                      0U
#endif /* __DTCM_PRESENT */

/* Optional: Data Tightly Coupled Memory */
#ifndef __DTCM_PRESENT
    #define __DTCM_PRESENT                      0U
#endif /* __DTCM_PRESENT */

/* Optional MPU */
#ifndef __MPU_PRESENT
    #define __MPU_PRESENT                       0U
#elif( __MPU_PRESENT == 1U )
    #ifndef __MPU_TYPE_PMSAV6
        #define __MPU_TYPE_PMSAV6               0U
    #endif /* __MPU_TYPE_PMSAV6 */

    #ifndef __MPU_TYPE_PMSAV7
        #define __MPU_TYPE_PMSAV7               0U
    #endif /* __MPU_TYPE_PMSAV7 */

    #ifndef __MPU_TYPE_PMSAV8
        #define __MPU_TYPE_PMSAV8               0U
    #endif /* __MPU_TYPE_PMSAV8 */

    #ifndef __MPU_HAVE_BACKGROUND_REGION
        #define __MPU_HAVE_BACKGROUND_REGION    1U
    #endif /* __MPU_HAVE_BACKGROUND_REGION */

    #ifndef __MPU_HAVE_ALIAS_REGS
        #define __MPU_HAVE_ALIAS_REGS           0U
    #endif /* __MPU_HAVE_ALIAS_REGS */

    #ifndef __MPU_MIN_REGIONS
        #define __MPU_MIN_REGIONS               0U
    #endif /* __MPU_MIN_REGIONS */

    #ifndef __MPU_MAX_REGIONS
        #define __MPU_MAX_REGIONS               256U
    #elif ( __MPU_MAX_REGIONS > 256 )
        #error __MPU_MAX_REGIONS must be less than 256.
    #endif /* __MPU_MAX_REGIONS */

    #ifndef __MPU_NUM_REGIONS
        #error "__MPU_NUM_REGIONS not defined"
    #elif( __MPU_NUM_REGIONS < __MPU_MIN_REGIONS )
        #error "__MPU_NUM_REGIONS must be >= __MPU_MIN_REGIONS"
    #elif( __MPU_NUM_REGIONS > __MPU_MAX_REGIONS )
        #error "__MPU_NUM_REGIONS must be <= __MPU_MAX_REGIONS"
    #endif /* __MPU_NUM_REGIONS */
#endif /* __MPU_PRESENT */

/* Optional FPU */
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

/* Optional ARMv7E-M / ARMv8-M DSP Extension */
#ifndef __DSP_PRESENT
    #define __DSP_PRESENT                       0U
#endif

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

/*
 * Optional: ARMv8-M Helium / M-Profile Vector Extension
 * __MVE_PRESENT implies that integer MVE is enabled.
 */
#ifndef __MVE_PRESENT
    #define __MVE_PRESENT                       0U
#elif( __MVE_PRESENT == 1U )
    /* Optional Floating Point MVE, requires an FPU */
    #ifndef __MVE_FP_PRESENT
        #define __MVE_FP_PRESENT                ( __FPU_PRESENT )
    #endif /* __MVE_FP_PRESENT */

    #if( ( __MVE_FP_PRESENT == 1 ) && ( __FPU_PRESENT == 0U ) )
        #error "__MVE_FP_PRESENT ( Vector Floating Point Extensions ) requires __FPU_PRESENT ( an FPU )"
    #endif /* __MVE_FP_PRESENT == 1 && __FPU_PRESENT == 0U */
#endif /* __MVE_PRESENT */

#endif /* ! defined( ARM_CORTEX_M_DEFAULTS_H ) */
