/*
 * FreeRTOS V202212.00
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

MEMORY
{
    FLASH (xr) : ORIGIN = 0x10000000, LENGTH = 2M
    RAM (xrw)  : ORIGIN = 0x38000000, LENGTH = 2M
    /* FLASH (xr) : ORIGIN = 0x10000000, LENGTH = 2M */
    /* RAM (xrw)  : ORIGIN = 0x30000000, LENGTH = 128K */
}

ENTRY(Reset_Handler)

__HEAP_SIZE = 0x180000;
__STACK_SIZE = 0x20000;

M_VECTOR_RAM_SIZE = (16 + 48) * 4;
/* _estack = ORIGIN(RAM) + LENGTH(RAM); */

VTOR_ALIGN = 128;   /* VTOR.TBLOFF is 25 bits wide -> 128 byte aligned */
ARCH_VECTOR_LEN = ( 4 * 16 );
PLATFORM_VECTOR_OFFSET = ARCH_VECTOR_LEN;

__privileged_functions_region_size__  = 32K; /* 32000 bytes */
__privileged_data_region_size__       = 32K; /* 32000 bytes */

__FLASH_segment_start__ = ORIGIN( FLASH );
__FLASH_segment_end__ = __FLASH_segment_start__ + LENGTH( FLASH );

__SRAM_segment_start__ = ORIGIN( RAM );
__SRAM_segment_end__ = __SRAM_segment_start__ + LENGTH( RAM );

__privileged_data_start__ = __SRAM_segment_start__;
__privileged_data_end__ = __SRAM_segment_start__ + __privileged_data_region_size__;

__privileged_functions_start__ = __FLASH_segment_start__;
__privileged_functions_end__ = __FLASH_segment_start__ + __privileged_functions_region_size__;

SECTIONS
{
    /* .rom_vectors ( NOLOAD ):
    {
        . = ALIGN( VTOR_ALIGN );
        __rom_vectors_start__ = .;
        PROVIDE( __rom_vectors_start__ = . );

        core_irq_vectors_rom = ABSOLUTE( ORIGIN( FLASH ) );
        , = ( . + PLATFORM_VECTOR_OFFSET );

        . = ALIGN( ARCH_VECTOR_LEN );

        *(.rom_vectors)
        KEEP(*(.rom_vectors))

        __rom_vectors_end__ = .;
        PROVIDE( __rom_vectors_end__ = . );
        . = ALIGN( 8 );
    } > FLASH */

    /* ASSERT( core_irq_vectors_rom == ORIGIN(FLASH), "core_irq_vectors_rom not placed at origin of ROM." ) */
    /* ASSERT( platform_irq_vectors_rom == ORIGIN(FLASH) + PLATFORM_VECTOR_OFFSET, "platform_irq_vectors_rom not placed at PLATFORM_VECTOR_OFFSET" ) */

    .privileged_functions :
    {
        . = ALIGN( VTOR_ALIGN );
        __rom_vectors_start__ = .;
        PROVIDE( __rom_vectors_start__ = . );

        core_irq_vectors_rom = ABSOLUTE( ORIGIN( FLASH ) );
        , = ( . + PLATFORM_VECTOR_OFFSET );

        . = ALIGN( ARCH_VECTOR_LEN );

        *(.rom_vectors)
        KEEP(*(.rom_vectors))
        __rom_vectors_end__ = .;
        PROVIDE( __rom_vectors_end__ = . );

        . = ALIGN(4);
        *(privileged_functions)
        . = ALIGN(4);
        FILL(0xDEAD);
        /* Ensure that un-privileged code is placed after the region reserved                                                                                                                                         4          * for privileged kernel code. */
        /* Note that dot (.) actually refers to the byte offset from the start
            * of the current section (.privileged_functions in this case). As a
            * result, setting dot (.) to a value sets the size of the section. */
        . = __privileged_functions_region_size__;
    } >FLASH

    .text :
    {
        . = ALIGN(4);
        __syscalls_flash_start__ = .;
        *(freertos_system_calls)
        __syscalls_flash_end__ = .;
        . = ALIGN(4);

        *(.text)
        *(.text*)
        *(.glue_7)         /* glue arm to thumb code. */
        *(.glue_7t)        /* glue thumb to arm code. */
        *(.eh_frame)

        KEEP(*(.init))
        KEEP(*(.fini))


        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        KEEP(*(.eh_frame*))

        . = ALIGN(4);
    } > FLASH


    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)         /* .rodata sections  (constants, strings, etc.). */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.). */
        . = ALIGN(4);

    } > FLASH


    .ARM.extab :
    {
        . = ALIGN(4);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } > FLASH

    .ARM.exidx :
    {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
        . = ALIGN(4);
    } > FLASH

    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;

        LONG( LOADADDR( .data ) )
        LONG( __data_start__ )
        LONG( ( __data_end__ - __data_start__ ) / 4 )

        __copy_table_end__ = .;
    } > FLASH

    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG( __bss_start__ )
        LONG( ( __bss_end__ - __bss_start__ ) / 4 )

        /* Add additional bss sections here
         * LONG (__bss2_start__)
         * LONG ((__bss2_end__ - __bss2_start__) / 4)
         */
        __zero_table_end__ = .;
    } > FLASH

    __etext = ALIGN (4);

    /* .ram_vectors : AT (__etext)
    {
        . = ALIGN( VTOR_ALIGN );

        PROVIDE( __ram_vectors_start__ = . );

        core_irq_vectors_ram = .;

        *(.ram_vectors)
        KEEP(*(.ram_vectors))

        . = ALIGN( ARCH_VECTOR_LEN );
        platform_irq_vectors_ram = . + PLATFORM_VECTOR_OFFSET;

        PROVIDE( __ram_vectors_end__ = . );

        . = ALIGN( 4 );
        KEEP(*(.ram_vectors))
    } > RAM */

    _sidata = LOADADDR(.privileged_data);

    .privileged_data :
    {
        . = ALIGN(4);
        _sdata = .;        /* Create a global symbol at data start. */
        *(privileged_data)
        . = ALIGN(4);
        FILL(0xDEAD);
        /* Ensure that un-privileged data is placed after the region reserved
            * for privileged kernel data. */
        /* Note that dot (.) actually refers to the byte offset from the start
            * * of the current section (.privileged_data in this case). As a result,
            * * setting dot (.) to a value sets the size of the section. */
        . = __privileged_data_region_size__;
    } >RAM AT> FLASH

    .data :
    {
        /* . = ALIGN( VTOR_ALIGN );
        __ram_vectors_start__ = .;
        core_irq_vectors_ram = .;
        . += PLATFORM_VECTOR_OFFSET;


        . = ALIGN( ARCH_VECTOR_LEN );
        platform_irq_vectors_ram = . + PLATFORM_VECTOR_OFFSET;

        __ram_vectors_end__ = .; */

        __data_start__ = .;

        *(vtable)
        *(.data)
        *(.data.*)

        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);

        __data_end__ = .;
        _edata = .;
    } > RAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;

        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);

        __bss_end__ = .;
    } > RAM  AT > RAM

    .heap ( NOLOAD ) :
    {
        . = ALIGN(8);
        __end__ = .;
        PROVIDE ( end = . );

        __heap_start = .;
        PROVIDE( __heap_start = . );

        . = . + __HEAP_SIZE;

        . = ALIGN(8);

        __heap_end = .;
        PROVIDE( __heap_end = . );
        __HeapLimit = .;
    } > RAM

    .stack( ORIGIN( RAM ) + LENGTH( RAM ) - __STACK_SIZE - 8 ) ( NOLOAD ) :
    {
        . = ALIGN(8);
        __StackLimit = .;
        StackLimit = .;
        . = ALIGN(8);

        . = . + __STACK_SIZE;
        . = ALIGN(8);

        __StackTop = .;
        StackTop = .;
        _estack = .;
        __stack = .;
        . = ALIGN(8);
    } > RAM



  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}

