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

#ifndef ARM_SYSTICK_H
#define ARM_SYSTICK_H

#define SYST_BASE_ADDR              0xE000E010UL

#define SYST_CSR_OFFSET             0x00UL
#define SYST_RVR_OFFSET             0x04UL
#define SYST_CVR_OFFSET             0x08UL
#define SYST_CALIB_OFFSET           0x0CUL

#define SYST_CSR_ADDR               ( ( unsigned long ) SYST_BASE_ADDR + SYST_CSR_OFFSET )
#define SYST_RVR_ADDR               ( ( unsigned long ) SYST_BASE_ADDR + SYST_RVR_OFFSET )
#define SYST_CVR_ADDR               ( ( unsigned long ) SYST_BASE_ADDR + SYST_CVR_OFFSET )
#define SYST_CALIB_ADDR             ( ( unsigned long ) SYST_BASE_ADDR + SYST_CALIB_OFFSET )

#define SYST_CSR                    ( * ( ( volatile unsigned long * ) ( uintptr_t ) SYST_CSR_ADDR ) )
#define SYST_RVR                    ( * ( ( volatile unsigned long * ) ( uintptr_t ) SYST_RVR_ADDR ) )
#define SYST_CVR                    ( * ( ( volatile unsigned long * ) ( uintptr_t ) SYST_CVR_ADDR ) )
#define SYST_CALIB                  ( * ( ( volatile unsigned long * ) ( uintptr_t ) SYST_CALIB_ADDR ) )

#endif /* ARM_SYSTICK_H */