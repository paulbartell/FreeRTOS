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

#ifndef SOC_MPS2_AN500_CM7_H
#define SOC_MPS2_AN500_CM7_H

#ifndef __ARM_ARCH_7EM__
    #define __ARM_ARCH_7EM__    1U
#endif /* __ARM_ARCH_7EM__ */

#define __FPU_PRESENT           1U
#define __MPU_PRESENT           1U
#define __MPU_NUM_REGIONS       16U
#define __ICACHE_PRESENT        1U
#define __DCACHE_PRESENT        1U
#define __DTCM_PRESENT          1U
#define __VTOR_PRESENT          1U
#define __SYSTICK_PRESENT       1U
// #define __NVIC_PRIO_BITS        3U
#define __NVIC_PRIO_BITS        8U
#define __NVIC_EXTI_NUM         32U

#include "core_cortex_m7.h"

#endif /* ! defined( SOC_MPS2_AN500_CM7_H ) */
