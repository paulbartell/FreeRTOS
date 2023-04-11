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

#ifndef ARM_SAU_H
#define ARM_SAU_H

#define SAU_BASE_ADDR           0xE000EDD0UL

#define SAU_CTRL_ADDR           ( SAU_BASE_ADDR + 0x00UL )
#define SAU_TYPE_ADDR           ( SAU_BASE_ADDR + 0x04UL )
#define SAU_RNR_ADDR            ( SAU_BASE_ADDR + 0x08UL )
#define SAU_RBAR_ADDR           ( SAU_BASE_ADDR + 0x0CUL )
#define SAU_RLAR_ADDR           ( SAU_BASE_ADDR + 0x10UL )
#define SAU_SFSR_ADDR           ( SAU_BASE_ADDR + 0x14UL )
#define SAU_SFAR_ADDR           ( SAU_BASE_ADDR + 0x18UL )

#define SAU_CTRL                ( * ( ( unsigned long * ) ( uintptr_t ) SAU_CTRL_ADDR ) )
#define SAU_TYPE                ( * ( ( unsigned long * ) ( uintptr_t ) SAU_TYPE_ADDR ) )
#define SAU_RNR                 ( * ( ( unsigned long * ) ( uintptr_t ) SAU_RNR_ADDR  ) )
#define SAU_RBAR                ( * ( ( unsigned long * ) ( uintptr_t ) SAU_RBAR_ADDR ) )
#define SAU_RLAR                ( * ( ( unsigned long * ) ( uintptr_t ) SAU_RLAR_ADDR ) )
#define SAU_SFSR                ( * ( ( unsigned long * ) ( uintptr_t ) SAU_SFSR_ADDR ) )
#define SAU_SFAR                ( * ( ( unsigned long * ) ( uintptr_t ) SAU_SFAR_ADDR ) )

#define SFSR_LSERR              ( 1U << 7U )
#define SFSR_SFARVALID          ( 1U << 6U )
#define SFSR_LSPERR             ( 1U << 5U )
#define SFSR_INVTRAN            ( 1U << 4U )
#define SFSR_AUVIOL             ( 1U << 3U )
#define SFSR_INVER              ( 1U << 2U )
#define SFSR_INVIS              ( 1U << 1U )
#define SFSR_INVEP              ( 1U << 0U )

#endif /* ARM_SAU_H */