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

#ifndef ARM_CORTEX_M0_H
#define ARM_CORTEX_M0_H

#ifndef __ARM_ARCH_6M__
    #define __ARM_ARCH_6M__                 1U
#endif /* __ARM_ARCH_6M__ */

/* Optional: SysTick Timer */
#ifndef __SYSTICK_PRESENT
    #define __SYSTICK_PRESENT               1U
#endif /* __SYSTICK_PRESENT */

/* Cortex-M0 Disallowed Features */
#define __FPU_PRESENT                       0U
#define __VTOR_PRESENT                      0U
#define __MPU_PRESENT                       0U

#include "arch_armv6m.h"

#endif /* ! defined( ARM_CORTEX_M0_H ) */
