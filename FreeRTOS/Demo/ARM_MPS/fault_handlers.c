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

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "irq_vectors.h"

#include "arm/scb.h"
#include "arm/sau.h"

#ifndef configRESET_TLS_BLOCK
    #define configRESET_TLS_BLOCK() \
    _impure_ptr = _GLOBAL_REENT
#endif /* configRESET_TLS_BLOCK */

asm
(
    ".macro HANDLE_FAULT pLogFunction;"
    #if ( __ARM_ARCH_8M_BASE__ || __ARM_ARCH_6M__ )

    "    mov r0, lr;"
    "    movs r4, #4;"
    "    tst r0, r4;"                        /* test bit 2 of lr to determine stack in use */
    "    bne 1f;"                            /* Jump to label 1 if NE ( bit not set ) */

    "0:  mrs r0, msp;"                         /* lr bit 2 indicates MSP was active, so geet MSP */
    "    b 2f;"                                /* Jump to label 2 */

    "1:  mrs r0, psp;"                        /* lr bit 2 indicates PSP was active, so get PSP */

    "2:  mov r1, r0;"                          /* Copy sp to r1 */
    "    mov r2, #32;"
    "    add r1, r2;"
    "    stmia r1!, {r4-r7};"                  /* Push low registers r4-r7 which are not pushed automatically exception entry */
    "    mov r4, r8;"                          /* Copy contents of high registers r8-r11 to low registers r4-r7 */
    "    mov r5, r9;"
    "    mov r6, r10;"
    "    mov r7, r11;"

    "    stmia r1!, {r4-r7};"                  /* Push previous contents of high registers to stack */
    "    mov r4, lr;"
    "    stmia r1!, {r4};"                     /* Push LR contents */

    #else /* ( __ARM_ARCH_8M_BASE__ || __ARM_ARCH_6M__ ) */

        "tst lr, #4;"                          /* test bit 2 of lr to determine stack in use */
        "ite eq;"
        "mrseq r0, msp;"
        "mrsne r0, psp;"

        "adds r1, r0, #32;"                    /* Increment r1 = r0 + 32 to get to correct location of start of frame */
        "stmia r1!, {r4-r7};"                  /* Push low registers r4-r7 which are not pushed automatically exception entry */
        "stmia r1!, {r8-r11};"                 /* Push previous contents of high registers to stack */
        "stmia r1!, {lr};"                     /* Push LR contents */

        /* Clear faultmask */
        "mov r3, 0x00;"
        "msr faultmask, r3;"

    #endif

    "blx \\pLogFunction;"

    ".endm;"
);

typedef struct __attribute__( ( packed ) )
{
    unsigned long R0;
    unsigned long R1;
    unsigned long R2;
    unsigned long R3;
    unsigned long R12;
    unsigned long LR;
    unsigned long PC;
    unsigned long xPSR;
    unsigned long R4;
    unsigned long R5;
    unsigned long R6;
    unsigned long R7;
    unsigned long R8;
    unsigned long R9;
    unsigned long R10;
    unsigned long R11;
    unsigned long FAULTLR;
} FaultStack_t;

static void vPrintFaultStack( FaultStack_t * pxFaultStack )
{
    printf( "  R0:  0x%08lX   ",     pxFaultStack->R0 );
    printf( "  R1:  0x%08lX   ",     pxFaultStack->R1 );
    printf( "  R2:  0x%08lX   ",     pxFaultStack->R2 );
    printf( "  R3:  0x%08lX\n",      pxFaultStack->R3 );
    printf( "  R4:  0x%08lX   ",     pxFaultStack->R4 );
    printf( "  R5:  0x%08lX   ",     pxFaultStack->R5 );
    printf( "  R6:  0x%08lX   ",     pxFaultStack->R6 );
    printf( "  R7:  0x%08lX\n",      pxFaultStack->R7 );
    printf( "  R8:  0x%08lX   ",     pxFaultStack->R8 );
    printf( "  R9:  0x%08lX   ",     pxFaultStack->R9 );
    printf( " R10:  0x%08lX   ",     pxFaultStack->R10 );
    printf( " R11:  0x%08lX\n",      pxFaultStack->R11 );
    printf( " R12:  0x%08lX   ",     pxFaultStack->R12 );
    printf( "  LR:  0x%08lX   ",     pxFaultStack->LR );
    printf( "  PC:  0x%08lX   ",     pxFaultStack->PC );
    printf( "xPSR:  0x%08lX\n\n",    pxFaultStack->xPSR );
    printf( "FAULTLR:  0x%08lX\n\n",    pxFaultStack->FAULTLR );
}

static void vPrintSCBRegs( void )
{
    printf( "  CFSR: 0x%08lX: [ MMFSR: 0x%02lX, BFSR: 0x%02lX, UFSR: 0x%02lX ]\n",
            SCB_CFSR,
            SCB_MMFSR,
            SCB_BFSR,
            SCB_UFSR );

    printf( " SHCSR: 0x%08lX\n", SCB_SHCSR );
    printf( "  HFSR: 0x%08lX\n", SCB_HFSR );
    printf( "  DFSR: 0x%08lX\n", SCB_DFSR );

    printf( " MMFSR: 0x%08lX\t", SCB_MMFSR );
    printf( " MMFAR: 0x%08lX\n", SCB_MMFAR );
    printf( "  BFAR: 0x%08lX\n", SCB_BFAR );
    printf( "  AFSR: 0x%08lX\n", SCB_AFSR );
}

void vEnableFaultHandlers( void )
{
    #if __ARM_ARCH_7M__ || __ARM_ARCH_8M_MAIN__
        SCB_SHCSR |= ( SHCSR_SECUREFAULTENA | SHCSR_USGFAULTENA | SHCSR_BUSFAULTENA | SHCSR_MEMFAULTENA );

        #if __CMSE_PRESENT
            SCB_SHCSR |= SHCSR_SECUREFAULTENA;
        #endif /* __CMSE_PRESENT */

        #if __MPU_PRESENT
            SCB_SHCSR |= SHCSR_MEMFAULTENA;
        #endif /* __MPU_PRESENT */

        /* Enable Non-Secure BusFault, HardFault, and NMI */
        // SCB_AIRCR |= ( AIRCR_BFHFNMINS );

        // /* Set to no group priority bits, 8 subpriority bits */
        // SCB_AIRCR |= AIRCR_PRIGROUP_SUBPRIORITY_8;

        // SCB_SHPR1 = ( ( 0x07 << SCB_SHPR1_PRI_7_SHIFT ) |
        //               ( 0x06 << SCB_SHPR1_PRI_6_SHIFT ) |
        //               ( 0x05 << SCB_SHPR1_PRI_5_SHIFT ) |
        //               ( 0x04 << SCB_SHPR1_PRI_4_SHIFT ) );

        // SCB_SHPR2 |= ( 0x0B << SCB_SHPR2_PRI_11_SHIFT );

        // SCB_SHPR3 |= ( ( 0x0F << SCB_SHPR3_PRI_15_SHIFT ) |
        //                ( 0x0E << SCB_SHPR3_PRI_14_SHIFT ) |
        //                ( 0x0C << SCB_SHPR3_PRI_12_SHIFT ) );

    #endif /* __ARM_ARCH_7M__ || __ARM_ARCH_8M_MAIN__ */
}

void vLogDefault( FaultStack_t * pxFaultStack )
{
    configRESET_TLS_BLOCK();

    printf( "******************************* Default Handler ******************************\n\n" );

    vPrintFaultStack( pxFaultStack );
    vPrintSCBRegs();

    exit( 1 );
}

void vLogHardFault( FaultStack_t * pxFaultStack )
{
    configRESET_TLS_BLOCK();
    printf( "***************************** Hard Fault Handler *****************************\n\n" );
    vPrintFaultStack( pxFaultStack );

    printf( "HFSR: 0x%02lX\n", SCB_HFSR );

    if( SCB_HFSR & HFSR_DEBUGEVT )
    {
        printf( "HFSR[ DEBUGEVT ]: Debug Event\n" );
    }
    if( SCB_HFSR & HFSR_FORCED )
    {
        printf( "HFSR[ FORCED ]: Escalated from lower priority fault\n" );
        vPrintSCBRegs();
    }
    if( SCB_HFSR & HFSR_VECTTBL )
    {
        printf( "HFSR[ VECTTBL ]: Vector Table Read Error\n" );
    }
    exit( 1 );
}

void vLogMemManageFault( FaultStack_t * pxFaultStack )
{
    configRESET_TLS_BLOCK();
    printf( "*********************** Memory Protection Fault Handler **********************\n\n" );

    vPrintFaultStack( pxFaultStack );

    printf( "MMFSR: 0x%08lX\n", SCB_MMFSR );

    if( SCB_MMFSR & MMFSR_MLSPERR )
    {
        printf( "MMFSR[ MLSPERR ]: Floating Point Lazy State Preservation Error\n" );
    }

    if( SCB_MMFSR & MMFSR_MSTKERR )
    {
        printf( "MMFSR[ MSTKERR ]: Error during Exception Entry Stacking\n" );
    }

    if( SCB_MMFSR & MMFSR_MUNSTKERR )
    {
        printf( "MMFSR[ MUNSTKERR ]: Error during Exception Return Unstacking\n" );
    }

    if( SCB_MMFSR & MMFSR_DACCVOL )
    {
        printf( "MMFSR[ DACCVOL ]: Data Access Violation\n" );
    }

    if( SCB_MMFSR & MMFSR_IACCVOL )
    {
        printf( "MMFSR[ IACCVOL ]: Instruction Access Violation\n" );
    }

    if( SCB_MMFSR & MMFSR_MMARVALID )
    {
        printf( "MMFSR[ MMARVALID ]: Memory Management Fault Address Valid\n" );
        printf( "MMFAR: 0x%08lX\n", SCB_MMFAR );
    }

    exit( 1 );
}

void vLogBusFault( FaultStack_t * pxFaultStack )
{
    configRESET_TLS_BLOCK();

    printf( "****************************** Bus Fault Handler *****************************\n\n" );

    vPrintFaultStack( pxFaultStack );

    printf( "BFSR: 0x%08lX\n", SCB_BFSR );

    if( SCB_BFSR & BFSR_IBUSERR )
    {
        printf( "BFSR[ IBUSERR ]: BusFault during Instruction Prefetch\n" );
    }

    if( SCB_BFSR & BFSR_PRECISERR )
    {
        printf( "BFSR[ PRECISERR ]: Precise Data Access Error\n" );
    }

    if( SCB_BFSR & BFSR_IMPRECISERR )
    {
        printf( "BFSR[ IMPRECISERR ]: Imprecise Data Access Error\n" );
    }

    if( SCB_BFSR & BFSR_UNSTKERR )
    {
        printf( "BFSR[ UNSTKERR ]: BusFault ocurred during unstack operation when returning from an exception handler\n" );
    }

    if( SCB_BFSR & BFSR_STKERR )
    {
        printf( "BFSR[ STKERR ]: BusFault ocurred during stack operation when entering an exception handler\n" );
    }

    if( SCB_BFSR & BFSR_STKERR )
    {
        printf( "BFSR[ LSPERR ]: BusFault ocurred during Floating Point Lazy State Preservation\n" );
    }

    if( SCB_BFSR & BFSR_BFARVALID )
    {
        printf( "BFSR[ BFARVALID ]: BFAR is Valid\n" );
        printf( "BFAR: 0x%08lX\n", SCB_BFAR );
    }

    // while( 1 )
    // {
    //     asm volatile( "nop;" );
    // }

    exit( 1 );
}

void vLogUsageFault( FaultStack_t * pxFaultStack )
{
    configRESET_TLS_BLOCK();

    printf( "***************************** Usage Fault Handler ****************************\n\n" );

    vPrintFaultStack( pxFaultStack );

    printf( "UFSR: 0x%08lX\n", SCB_UFSR );

    if( SCB_UFSR & UFSR_DIVBYZERO )
    {
        printf( "UFSR[ DIVBYZERO ]: Divide By Zero\n" );
    }
    if( SCB_UFSR & UFSR_UNALIGNED )
    {
        printf( "UFSR[ UNALIGNED ]: Unaligned Access\n" );
    }
    if( SCB_UFSR & UFSR_STKOF )
    {
        printf( "UFSR[ STKOF ]: Stack Overflow\n" );
    }
    if( SCB_UFSR & UFSR_NOCP )
    {
        printf( "UFSR[ NOCP ]: No Coprocessor\n" );
    }

    if( SCB_UFSR & UFSR_INVPC )
    {
        printf( "UFSR[ INVPC ]: Invalid Program Counter (PC)\n" );
    }

    if( SCB_UFSR & UFSR_INVSTATE )
    {
        printf( "UFSR[ INVSTATE ]: EPSR.T or EPSR.IT validity\n" );
    }

    if( SCB_UFSR & UFSR_UNDEFINSTR )
    {
        printf( "UFSR[ UNDEFINSTR ]: Undefined Instruction\n" );
    }

    exit( 1 );
}

void vLogSecureFault( FaultStack_t * pxFaultStack )
{
    configRESET_TLS_BLOCK();

    printf( "**************************** Secure Fault Handler ****************************\n\n" );

    vPrintFaultStack( pxFaultStack );

    printf( "SFSR: 0x%08lX\n", SAU_SFSR );

    if( SAU_SFSR & SFSR_LSERR )
    {
        printf( "SFSR[ LSERR ]: Lazy State Error\n" );
    }

    if( SAU_SFSR & SFSR_LSPERR )
    {
        printf( "SFSR[ LSPERR ]: Lazy State Preservation Error\n" );
    }
    if( SAU_SFSR & SFSR_INVTRAN )
    {
        printf( "SFSR[ INVTRAN ]: Invalid Transition Error\n" );
    }
    if( SAU_SFSR & SFSR_AUVIOL )
    {
        printf( "SFSR[ AUVIOL ]: Attribution Unit Violation Error\n" );
    }
    if( SAU_SFSR & SFSR_INVER )
    {
        printf( "SFSR[ INVER ]: Invalid Exception Return Error\n" );
    }
    if( SAU_SFSR & SFSR_INVIS )
    {
        printf( "SFSR[ INVIS ]: Invalid Integrity Signature Error\n" );
    }
    if( SAU_SFSR & SFSR_INVEP )
    {
        printf( "SFSR[ INVEP ]: Invalid Entry Point Error\n" );
    }

    if( SAU_SFSR & SFSR_SFARVALID )
    {
        printf( "SFAR: 0x%08lX\n", SAU_SFAR );
    }

    exit( 1 );
}


void vLogDebugMonitor( FaultStack_t * pxFaultStack )
{
    configRESET_TLS_BLOCK();

    printf( "**************************** DebugMonitor Handler ****************************\n" );

    vPrintFaultStack( pxFaultStack );

    printf( "DFSR: 0x%08lX\n", SCB_DFSR );

    if( SCB_DFSR & DFSR_EXTERNAL )
    {
        printf( "DFSR[ EXTERNAL ]: External Event\n" );
    }

    if( SCB_DFSR & DFSR_VCATCH )
    {
        printf( "DFSR[ VCATCH ]: Vector Catch Event\n" );
    }

    if( SCB_DFSR & DFSR_DWTTRAP )
    {
        printf( "DFSR[ DWTTRAP ]: Watchpoint Event\n" );
    }

    if( SCB_DFSR & DFSR_BKPT )
    {
        printf( "DFSR[ BKPT ]: Breakpoint Event\n" );
    }

    if( SCB_DFSR & DFSR_HALTED )
    {
        printf( "DFSR[ HALTED ]: Halt or Step Event\n" );
    }

    exit( 1 );
}

__attribute__( ( naked, aligned( 8 ) ) )
void Default_Handler( void )
{
    asm volatile( "HANDLE_FAULT vLogDefault;" );
}

#define STRING(X) #X
#define STRINGIFY(X) STRING(X)


__attribute__( ( naked, aligned( 8 ) ) )
void HardFault_Handler( void )
{
    // #if __ARM_ARCH_7M__ || __ARM_ARCH_8M_MAIN__
        // asm volatile
        // (
        //     /* Determine Stack pointer in use => r0 */
        //     "tst lr, #4;"
        //     "ite eq;"
        //     "mrseq r0, msp;"
        //     "mrsne r0, psp;"

        //     /* Determine if HardFault was caused by a breakpoint */
        //     "ldr r1, ="STRINGIFY( SCB_HFSR_ADDR )";"
        //     "ldr r2, [ r1 ];"
        //     "lsls r2, r2, 1;"

        //     "bcs skip_fault;"

        //     "HANDLE_FAULT vLogHardFault;"

        // "skip_fault:;"
        //     /* Clear fault status */
        //     "str r2, [ r1 ];"

        //     /* Fault is due to debug breakpoint. Increment return address and move on. */
        //     "ldr r1, [ r0, #24 ];"
        //     "adds r1, #2;"
        //     "str r1, [ r0, #24 ];"
        //     "bx lr;"
        // );
    // #else
        asm volatile
        (
            "HANDLE_FAULT vLogHardFault;"
        );
    // #endif
}

__attribute__( ( naked, aligned( 8 ) ) )
void MemManageFault_Handler( void )
{
    asm volatile( "HANDLE_FAULT vLogMemManageFault" );
}

__attribute__( ( naked, aligned( 8 ) ) )
void BusFault_Handler( void )
{
    asm volatile( "HANDLE_FAULT vLogBusFault" );
}

__attribute__( ( naked, aligned( 8 ) ) )
void UsageFault_Handler( void )
{
    asm volatile( "HANDLE_FAULT vLogUsageFault" );
}

__attribute__( ( naked, aligned( 8 ) ) )
void SecureFault_Handler( void )
{
    asm volatile( "HANDLE_FAULT vLogSecureFault" );
}

__attribute__( ( naked, aligned( 8 ) ) )
void DebugMonitor_Handler( void )
{
    asm volatile( "HANDLE_FAULT vLogDebugMonitor" );
}

