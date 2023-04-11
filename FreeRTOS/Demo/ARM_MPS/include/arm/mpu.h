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

#ifndef ARM_MPU_H
#define ARM_MPU_H

#define MPU_BASE_ADDR                   0xE000ED90UL

/*
 * Registers common between ARMv6M, ARMv7M, and ARMv8M MPU
 */
#define MPU_TYPE_OFFSET                 0x00UL
#define MPU_CTRL_OFFSET                 0x04UL
#define MPU_RNR_OFFSET                  0x08UL

#define MPU_TYPE_ADDR                   ( ( unsigned long ) MPU_BASE_ADDR + MPU_TYPE_OFFSET )
#define MPU_CTRL_ADDR                   ( ( unsigned long ) MPU_BASE_ADDR + MPU_CTRL_OFFSET )
#define MPU_RNR_ADDR                    ( ( unsigned long ) MPU_BASE_ADDR + MPU_RNR_OFFSET )

#define MPU_TYPE                        ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_TYPE_ADDR ) )
#define MPU_CTRL                        ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_CTRL_ADDR ) )
#define MPU_RNR                         ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RNR_ADDR ) )

/*
 * ARMv6M amd ARMv7M MPU Registers
 */
#if __MPU_TYPE_PMSAV7 || __MPU_TYPE_PMSAV6
    #define MPU_RBAR_OFFSET             0x0CUL
    #define MPU_RASR_OFFSET             0x10UL

    #define MPU_RBAR_ADDR               ( ( unsigned long ) MPU_BASE_ADDR + MPU_RBAR_OFFSET )
    #define MPU_RASR_ADDR               ( ( unsigned long ) MPU_BASE_ADDR + MPU_RASR_OFFSET )

    #define MPU_RBAR                    ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RBAR_ADDR ) )
    #define MPU_RASR                    ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RASR_ADDR ) )

    #if __MPU_HAVE_ALIAS_REGS
        #define MPU_RBAR_A1_OFFSET      0x14UL
        #define MPU_RASR_A1_OFFSET      0x18UL

        #define MPU_RBAR_A2_OFFSET      0x1CUL
        #define MPU_RASR_A2_OFFSET      0x20UL

        #define MPU_RBAR_A3_OFFSET      0x24UL
        #define MPU_RASR_A3_OFFSET      0x28UL

        #define MPU_RBAR_A1_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RBAR_A1_OFFSET )
        #define MPU_RASR_A1_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RASR_A1_OFFSET )

        #define MPU_RBAR_A2_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RBAR_A2_OFFSET )
        #define MPU_RASR_A2_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RASR_A2_OFFSET )

        #define MPU_RBAR_A3_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RBAR_A3_OFFSET )
        #define MPU_RASR_A3_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RASR_A3_OFFSET )


        #define MPU_RBAR_A1             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RBAR_A1_ADDR ) )
        #define MPU_RASR_A1             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RASR_A1_ADDR ) )

        #define MPU_RBAR_A2             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RBAR_A2_ADDR ) )
        #define MPU_RASR_A2             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RASR_A2_ADDR ) )

        #define MPU_RBAR_A3             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RBAR_A3_ADDR ) )
        #define MPU_RASR_A3             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RASR_A3_ADDR ) )
    #endif /* __MPU_HAVE_ALIAS_REGS */

#elif __MPU_TYPE_PMSAV8
    #define MPU_RBAR_OFFSET             0x0CUL
    #define MPU_RLAR_OFFSET             0x10UL
    #define MPU_MAIR0_OFFSET            0x30UL
    #define MPU_MAIR1_OFFSET            0x34UL

    #define MPU_RASR_ADDR               ( ( unsigned long ) MPU_BASE_ADDR + MPU_RASR_OFFSET )
    #define MPU_RLAR_ADDR               ( ( unsigned long ) MPU_BASE_ADDR + MPU_RLAR_OFFSET )
    #define MPU_MAIR0_ADDR              ( ( unsigned long ) MPU_BASE_ADDR + MPU_MAIR0_OFFSET )
    #define MPU_MAIR1_ADDR              ( ( unsigned long ) MPU_BASE_ADDR + MPU_MAIR1_OFFSET )

    #define MPU_RBAR                    ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RBAR_ADDR ) )
    #define MPU_MAIR0                   ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_MAIR0_ADDR ) )
    #define MPU_MAIR1                   ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_MAIR1_ADDR ) )

    #if __MPU_HAVE_ALIAS_REGS
        #define MPU_RBAR_A1_OFFSET      0x14UL
        #define MPU_RLAR_A1_OFFSET      0x18UL

        #define MPU_RBAR_A2_OFFSET      0x1CUL
        #define MPU_RLAR_A2_OFFSET      0x20UL

        #define MPU_RBAR_A3_OFFSET      0x24UL
        #define MPU_RLAR_A3_OFFSET      0x28UL

        #define MPU_RBAR_A1_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RBAR_A1_OFFSET )
        #define MPU_RLAR_A1_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RLAR_A1_OFFSET )

        #define MPU_RBAR_A2_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RBAR_A2_OFFSET )
        #define MPU_RLAR_A2_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RLAR_A2_OFFSET )

        #define MPU_RBAR_A3_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RBAR_A3_OFFSET )
        #define MPU_RLAR_A3_ADDR        ( ( unsigned long ) MPU_BASE_ADDR + MPU_RLAR_A3_OFFSET )

        #define MPU_RBAR_A1             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RBAR_A1_ADDR ) )
        #define MPU_RLAR_A1             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RLAR_A1_ADDR ) )

        #define MPU_RBAR_A2             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RBAR_A2_ADDR ) )
        #define MPU_RLAR_A2             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RLAR_A2_ADDR ) )

        #define MPU_RBAR_A3             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RBAR_A3_ADDR ) )
        #define MPU_RLAR_A3             ( * ( ( volatile unsigned long * ) ( uintptr_t ) MPU_RLAR_A3_ADDR ) )
    #endif /* __MPU_HAVE_ALIAS_REGS */
#endif /* __MPU_TYPE_PMSAV8 */
#endif /* ARM_MPU_H */