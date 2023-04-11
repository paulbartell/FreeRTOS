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

#ifndef ARM_SCB_H
#define ARM_SCB_H

#define SCB_BASE_ADDR               0xE000ED00UL

#define SCB_CPUID_ADDR              ( SCB_BASE_ADDR + 0x00UL )
#define SCB_ICSR_ADDR               ( SCB_BASE_ADDR + 0x04UL )
#define SCB_VTOR_ADDR               ( SCB_BASE_ADDR + 0x08UL )
#define SCB_AIRCR_ADDR              ( SCB_BASE_ADDR + 0x0CUL )
#define SCB_SCR_ADDR                ( SCB_BASE_ADDR + 0x10UL )
#define SCB_CCR_ADDR                ( SCB_BASE_ADDR + 0x14UL )
#define SCB_SHPR1_ADDR              ( SCB_BASE_ADDR + 0x18UL )
#define SCB_SHPR2_ADDR              ( SCB_BASE_ADDR + 0x1CUL )
#define SCB_SHPR3_ADDR              ( SCB_BASE_ADDR + 0x20UL )
#define SCB_SHCSR_ADDR              ( SCB_BASE_ADDR + 0x24UL )
#define SCB_CFSR_ADDR               ( SCB_BASE_ADDR + 0x28UL )
#define SCB_HFSR_ADDR               ( SCB_BASE_ADDR + 0x2CUL )
#define SCB_DFSR_ADDR               ( SCB_BASE_ADDR + 0x30UL )
#define SCB_MMFAR_ADDR              ( SCB_BASE_ADDR + 0x34UL )
#define SCB_BFAR_ADDR               ( SCB_BASE_ADDR + 0x38UL )
#define SCB_AFSR_ADDR               ( SCB_BASE_ADDR + 0x3CUL )
#define SCB_CLIDR_ADDR              ( SCB_BASE_ADDR + 0x78UL )
#define SCB_CTR_ADDR                ( SCB_BASE_ADDR + 0x7CUL )
#define SCB_CCSID_ADDRR             ( SCB_BASE_ADDR + 0x80UL )
#define SCB_CSSEL_ADDRR             ( SCB_BASE_ADDR + 0x84UL )
#define SCB_CPACR_ADDR              ( SCB_BASE_ADDR + 0x88UL )
#define SCB_NSACR_ADDR              ( SCB_BASE_ADDR + 0x8CUL )

#define SCB_CPUID                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_CPUID_ADDR ) )
#define SCB_ICSR                    ( * ( ( unsigned long * ) ( uintptr_t ) SCB_ICSR_ADDR ) )
#define SCB_VTOR                    ( * ( ( unsigned long * ) ( uintptr_t ) SCB_VTOR_ADDR ) )
#define SCB_AIRCR                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_AIRCR_ADDR ) )
#define SCB_SCR                     ( * ( ( unsigned long * ) ( uintptr_t ) SCB_SCR_ADDR ) )
#define SCB_CCR                     ( * ( ( unsigned long * ) ( uintptr_t ) SCB_CCR_ADDR ) )
#define SCB_SHPR1                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_SHPR1_ADDR ) )
#define SCB_SHPR2                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_SHPR2_ADDR ) )
#define SCB_SHPR3                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_SHPR3_ADDR ) )
#define SCB_SHCSR                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_SHCSR_ADDR ) )
#define SCB_CFSR                    ( * ( ( unsigned long * ) ( uintptr_t ) SCB_CFSR_ADDR ) )
#define SCB_HFSR                    ( * ( ( unsigned long * ) ( uintptr_t ) SCB_HFSR_ADDR ) )
#define SCB_DFSR                    ( * ( ( unsigned long * ) ( uintptr_t ) SCB_DFSR_ADDR ) )
#define SCB_MMFAR                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_MMFAR_ADDR ) )
#define SCB_BFAR                    ( * ( ( unsigned long * ) ( uintptr_t ) SCB_BFAR_ADDR ) )
#define SCB_AFSR                    ( * ( ( unsigned long * ) ( uintptr_t ) SCB_AFSR_ADDR ) )
#define SCB_CLIDR                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_CLIDR_ADDR ) )
#define SCB_CTR                     ( * ( ( unsigned long * ) ( uintptr_t ) SCB_CTR_ADDR ) )
#define SCB_CCSID                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_CCSID__ADDR ) )
#define SCB_CSSEL                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_CSSEL__ADDR ) )
#define SCB_CPACR                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_CPACR_ADDR ) )
#define SCB_NSACR                   ( * ( ( unsigned long * ) ( uintptr_t ) SCB_NSACR_ADDR ) )

#define AIRCR_VECTKEY_SHIFT             ( 16U )
#define AIRCR_VECTKEY_MASK              ( 0xFFFFUL << AIRCR_VECTKEY_SHIFT )
#define AIRCR_VECTKEY_WRITE             ( 0x05FAUL << AIRCR_VECTKEY_SHIFT )
#define AIRCR_VECTKEYSTAT_MASK          ( AIRCR_VECTKEY_MASK )
#define AIRCR_ENDIANNESS                ( 1U << 15U )
#define AIRCR_ENDIANNESS_BIG            ( AIRCR_ENDIANNESS )
#define AIRCR_ENDIANNESS_LITLE          ( 0x00000000UL )
#define AIRCR_PRIS                      ( 1U << 14U )
#define AIRCR_BFHFNMINS                 ( 1U << 13U )
#define AIRCR_PRIGROUP_SHIFT            ( 8U )
#define AIRCR_PRIGROUP_MASK             ( 0x7UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_PRIGROUP_SUBPRIORITY_1    ( 0x0UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_PRIGROUP_SUBPRIORITY_2    ( 0x1UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_PRIGROUP_SUBPRIORITY_3    ( 0x2UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_PRIGROUP_SUBPRIORITY_4    ( 0x3UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_PRIGROUP_SUBPRIORITY_5    ( 0x4UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_PRIGROUP_SUBPRIORITY_6    ( 0x5UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_PRIGROUP_SUBPRIORITY_7    ( 0x6UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_PRIGROUP_SUBPRIORITY_8    ( 0x7UL << AIRCR_PRIGROUP_SHIFT )
#define AIRCR_SYSRESETREQS              ( 1U << 3U )
#define AIRCR_SYSRESETREQ               ( 1U << 2U )
#define AIRCR_VECTCLRACTIVE             ( 1U << 1U )

#define SCB_SHPR1_PRI_7_SHIFT           ( 24U )
#define SCB_SHPR1_PRI_7_MASK            ( 0xFFUL << SCB_SHPR1_PRI_7_SHIFT )
#define SCB_SHPR1_PRI_6_SHIFT           ( 16U )
#define SCB_SHPR1_PRI_6_MASK            ( 0xFFUL << SCB_SHPR1_PRI_6_SHIFT )
#define SCB_SHPR1_PRI_5_SHIFT           ( 8U )
#define SCB_SHPR1_PRI_5_MASK            ( 0xFFUL << SCB_SHPR1_PRI_5_SHIFT )
#define SCB_SHPR1_PRI_4_SHIFT           ( 0U )
#define SCB_SHPR1_PRI_4_MASK            ( 0xFFUL << SCB_SHPR1_PRI_4_SHIFT )

#define SCB_SHPR2_PRI_11_SHIFT           ( 24U )
#define SCB_SHPR2_PRI_11_MASK            ( 0xFFUL << SCB_SHPR2_PRI_11_SHIFT )

#define SCB_SHPR3_PRI_15_SHIFT           ( 24U )
#define SCB_SHPR3_PRI_15_MASK            ( 0xFFUL << SCB_SHPR3_PRI_15_SHIFT )
#define SCB_SHPR3_PRI_14_SHIFT           ( 16U )
#define SCB_SHPR3_PRI_14_MASK            ( 0xFFUL << SCB_SHPR3_PRI_14_SHIFT )
#define SCB_SHPR3_PRI_12_SHIFT           ( 0U )
#define SCB_SHPR3_PRI_12_MASK            ( 0xFFUL << SCB_SHPR3_PRI_12_SHIFT )

#define CFSR_MMFSR_MASK         ( 0x000000FFUL )
#define CFSR_MMFSR_SHIFT        ( 0U )
#define CFSR_BFSR_MASK          ( 0x0000FF00UL )
#define CFSR_BFSR_SHIFT         ( 8U )
#define CFSR_UFSR_MASK          ( 0xFFFF0000UL )
#define CFSR_UFSR_SHIFT         ( 16U )

#define SCB_MMFSR               ( ( SCB_CFSR & CFSR_MMFSR_MASK ) >> CFSR_MMFSR_SHIFT )
#define SCB_UFSR                ( ( SCB_CFSR & CFSR_UFSR_MASK ) >> CFSR_UFSR_SHIFT )
#define SCB_BFSR                ( ( SCB_CFSR & CFSR_BFSR_MASK ) >> CFSR_BFSR_SHIFT )

#define HFSR_DEBUGEVT           ( 1U << 31U )
#define HFSR_VECTTBL            ( 1U << 1U )
#define HFSR_FORCED             ( 1U << 30U )

#define MMFSR_MMARVALID         ( 1U << 7U )
#define MMFSR_MLSPERR           ( 1U << 5U )
#define MMFSR_MSTKERR           ( 1U << 4U )
#define MMFSR_MUNSTKERR         ( 1U << 3U )
#define MMFSR_DACCVOL           ( 1U << 1U )
#define MMFSR_IACCVOL           ( 1U << 0U )

#define BFSR_BFARVALID          ( 1U << 7U )
#define BFSR_LSPERR             ( 1U << 5U )
#define BFSR_STKERR             ( 1U << 4U )
#define BFSR_UNSTKERR           ( 1U << 3U )
#define BFSR_IMPRECISERR        ( 1U << 2U )
#define BFSR_PRECISERR          ( 1U << 1U )
#define BFSR_IBUSERR            ( 1U << 0U )

#define UFSR_DIVBYZERO          ( 1U << 9U )
#define UFSR_UNALIGNED          ( 1U << 8U )
#define UFSR_STKOF              ( 1U << 4U )
#define UFSR_NOCP               ( 1U << 3U )
#define UFSR_INVPC              ( 1U << 2U )
#define UFSR_INVSTATE           ( 1U << 1U )
#define UFSR_UNDEFINSTR         ( 1U << 0U )

#define DFSR_EXTERNAL           ( 1U << 4U )
#define DFSR_VCATCH             ( 1U << 3U )
#define DFSR_DWTTRAP            ( 1U << 2U )
#define DFSR_BKPT               ( 1U << 1U )
#define DFSR_HALTED             ( 1U << 0U )

#define SHCSR_HARDFAULTPENDED   ( 1U << 21U )
#define SHCSR_SECUREFAULTPENDED ( 1U << 20U )
#define SHCSR_SECUREFAULTENA    ( 1U << 19U )
#define SHCSR_USGFAULTENA       ( 1U << 18U )
#define SHCSR_BUSFAULTENA       ( 1U << 17U )
#define SHCSR_MEMFAULTENA       ( 1U << 16U )
#define SHCSR_SVCALLPENDED      ( 1U << 15U )
#define SHCSR_BUSFAULTPENDED    ( 1U << 14U )
#define SHCSR_MEMFAULTPENDED    ( 1U << 12U )
#define SHCSR_USGFAULTPENDED    ( 1U << 12U )
#define SHCSR_SYSTICKACT        ( 1U << 11U )
#define SHCSR_PENDSVACT         ( 1U << 10U )
#define SHCSR_MONITORACT        ( 1U << 8U )
#define SHCSR_SVCALLACT         ( 1U << 7U )
#define SHCSR_NMIACT            ( 1U << 5U )
#define SHCSR_SECUREFAULTACT    ( 1U << 4U )
#define SHCSR_USGFAULTACT       ( 1U << 3U )
#define SHCSR_HARDFAULTACT      ( 1U << 2U )
#define SHCSR_BUSFAULTACT       ( 1U << 1U )
#define SHCSR_MEMFAULTACT       ( 1U << 0U )

#endif /* ARM_SCB_H */