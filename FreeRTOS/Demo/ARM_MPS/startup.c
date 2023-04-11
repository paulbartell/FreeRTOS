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
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "irq_vectors.h"
#include "arm/systick.h"

#include "FreeRTOS.h"

#ifdef __cplusplus
    extern "C" {
#endif

#define VTOR_BASE           ( 0xE000ED08U )
#define VTOR_MASK           ( 0xFFFFFF80U )
#define VTOR                ( * ( ( size_t * const ) VTOR_BASE ) )

#define VTOR_NS_BASE        ( 0xE002ED08 )
#define VTOR_NS             ( * ( ( size_t * const ) VTOR_NS_BASE ) )


#define PSP_BASE

/* Type Definitions */
typedef struct
{
    size_t * const pSrc;
    size_t * const pDest;
    size_t uxLen;
} CopyTableEntry_t;

typedef struct
{
    size_t * const pDest;
    size_t uxLen;
} ZeroTableEntry;

/* Function Declarations */
extern int main( int argc, char * argv[] );
extern void exit( int exit_code );
extern void initialise_monitor_handles( void );

/* Globals */
extern CopyTableEntry_t const __copy_table_start__;
extern CopyTableEntry_t const __copy_table_end__;

extern ZeroTableEntry const __zero_table_start__;
extern ZeroTableEntry const __zero_table_end__;

extern IRQHandler_t * const __rom_vectors_start__;
extern IRQHandler_t * const __rom_vectors_end__;
extern IRQHandler_t * __ram_vectors_start__;
extern IRQHandler_t * __ram_vectors_end__;

/* For non-newlib semihosting implementations,
   provide an empty stub for initialise_monitor_handles */
__attribute__( ( weak, used ) )
void initialise_monitor_handles( void )
{
}


#if defined(__clang__)
    __attribute__( ( optnone ) )
#elif defined(__GNUC__) || defined(__GNUG__)
    __attribute__( ( optimize( "0" ) ) )
#endif
static void init_data_sections( void )
{
    const CopyTableEntry_t * pCopyTable;
    size_t dataIndex;

    for( pCopyTable = &__copy_table_start__; pCopyTable <= &__copy_table_end__; pCopyTable++ )
    {
        for( dataIndex = 0; dataIndex < pCopyTable->uxLen; dataIndex++ )
        {
            ( pCopyTable->pDest )[ dataIndex ] = ( pCopyTable->pSrc )[ dataIndex ];
        }
    }
}

#if defined(__clang__)
    __attribute__( ( optnone ) )
#elif defined(__GNUC__) || defined(__GNUG__)
    __attribute__( ( optimize( "0" ) ) )
#endif
static void init_bss_sections( void )
{
    const ZeroTableEntry * pZeroTable;
    size_t dataIndex;

    for( pZeroTable = &__zero_table_start__; pZeroTable <= &__zero_table_end__; pZeroTable++ )
    {
        for( dataIndex = 0; dataIndex < pZeroTable->uxLen; dataIndex++ )
        {
            ( pZeroTable->pDest )[ dataIndex ] = 0;
        }
    }
}

// #if defined(__clang__)
//     __attribute__( ( optnone ) )
// #elif defined(__GNUC__) || defined(__GNUG__)
//     __attribute__( ( optimize( "0" ) ) )
// #endif
// static void init_ram_vectors( void )
// {
//     size_t numRomVectors = ( &__rom_vectors_end__ - &__rom_vectors_start__ ) / sizeof( void * );
//     size_t numRamVectors = ( &__ram_vectors_end__ - &__ram_vectors_start__ ) / sizeof( void * );
//     size_t vectorIndex;

//     for( vectorIndex = 0; vectorIndex < numRomVectors; vectorIndex++ )
//     {
//         ( &__ram_vectors_start__ )[ vectorIndex ] = ( &__rom_vectors_start__ )[ vectorIndex ];
//     }

//     for( vectorIndex = numRomVectors; vectorIndex < numRamVectors; vectorIndex++ )
//     {
//         ( &__ram_vectors_start__ )[ vectorIndex ] = NULL;
//     }

//     asm volatile( "cpsid i;" );

//     VTOR = ( ( size_t ) ( void * ) ( IRQHandler_t ** ) __ram_vectors_start__ ) & VTOR_MASK;

//     asm volatile
//     (
//         "dsb 0xF;"
//         "isb 0xF;"
//         "cpsie i;"
//     );
// }

extern void _start( void );
extern void __sinit( struct _reent * s );

#if configUSE_C_RUNTIME_TLS_SUPPORT == 1U
    configTLS_BLOCK_TYPE xTlsHandlerMode;
#endif /* configUSE_C_RUNTIME_TLS_SUPPORT */

extern void vEnableFaultHandlers( void );


__attribute__( ( weak, used ) )
void Reset_Handler( void )
{
    /* set stack pointer */
    asm volatile
    (
        "ldr r0, =__StackTop;"
        "mov sp, r0;"

        #if __ARM_ARCH_8M__
            "msr psp, r0;"
            "msr msp, r0;"

            "ldr r0, =__StackLimit;"
            "msr msplim, r0;"
            "msr psplim, r0;"
        #endif /* __ARM_ARCH_8M__ */
    );

    init_data_sections();
    init_bss_sections();

    // init_ram_vectors();

    #if( _RETARGETABLE_LOCKING == 1 )
        extern void newlib_retarget_locks_init( void );

        newlib_retarget_locks_init();
    #endif /* _RETARGETABLE_LOCKING == 1 */

    vEnableFaultHandlers();

    initialise_monitor_handles();

    /* Disable SYSTICK */
    SYST_CSR = 0x00000004;
    SYST_RVR = 0x00004E1F;
    SYST_CVR = 0x00000000;

    // _start();
    main( 0, NULL );
    exit( 1 );
}

#ifdef __cplusplus
    }
#endif
