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
    FLASH (xr) : ORIGIN = 0x00000000, LENGTH = 4M
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 4M
}

ENTRY(Reset_Handler)

__HEAP_SIZE = 0x300000;
__STACK_SIZE = 0x4000;

M_VECTOR_RAM_SIZE = (16 + 48) * 4;
_estack = ORIGIN(RAM) + LENGTH(RAM);

VTOR_ALIGN = 128;   /* VTOR.TBLOFF is 25 bits wide -> 128 byte aligned */
ARCH_VECTOR_LEN = ( 4 * 16 );
PLATFORM_VECTOR_OFFSET = ARCH_VECTOR_LEN;

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

    .text :
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

        *(.text)
        /* *(.text*) */

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

        *(.rodata*)

        KEEP(*(.eh_frame*))
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

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

    .stack( ORIGIN( RAM ) + LENGTH( RAM ) - __STACK_SIZE) ( NOLOAD ) :
    {
        . = ALIGN(8);
        __StackLimit = .;

        . = . + __STACK_SIZE;

        . = ALIGN(8);
        __StackTop = .;
    } > RAM

    PROVIDE(__stack = __StackTop);

  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}

