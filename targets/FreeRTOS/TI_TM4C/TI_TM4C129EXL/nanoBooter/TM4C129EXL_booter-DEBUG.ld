/*
//
// Copyright (c) 2020 The nanoFramework project contributors
// Portions Copyright (c) 2013-2020 Texas Instruments Incorporated.  All rights reserved.
// See LICENSE file in the project root for full license information.
//
*/

MEMORY
{
    FLASH (RX) : ORIGIN = 0x00000000, LENGTH = 0x00100000
    SRAM (WX)  : ORIGIN = 0x20000000, LENGTH = 0x00040000
}

ENTRY(ResetISR)

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _data = .;
        _ldata = LOADADDR (.data);
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM
}
