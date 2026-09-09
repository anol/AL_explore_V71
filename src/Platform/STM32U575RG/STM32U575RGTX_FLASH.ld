/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Author      : STM32CubeIDE
**
**  Abstract    : Linker script for STM32U575xG Device from STM32U5 series
**                      1024Kbytes FLASH_BANK1
**                      784Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2023 STMicroelectronics.
** All rights reserved.
**
** This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);	/* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x8000;	/* required amount of heap  */
_Min_Stack_Size = 0x2000;	/* required amount of stack */

/* Memories definition */
MEMORY
{
    RAM	(xrw)	   : ORIGIN = 0x20000000,	LENGTH = 768K
    SRAM4	(xrw)	   : ORIGIN = 0x28000000,	LENGTH = 16K
    FLASH_BANK1 (rx) : ORIGIN = 0x08000000,	LENGTH = 512K
    FLASH_BANK2 (rx) : ORIGIN = 0x08080000,	LENGTH = 496K
    Repository (rw)  : ORIGIN = 0x080FC000,	LENGTH = 16K
}

    /* Sections */
SECTIONS
{

    /* The startup code into "FLASH" Rom type memory */
    .isr_vector :
    {
          . = ALIGN(16);
        KEEP(*(.isr_vector)) /* Startup code */
    } >FLASH_BANK1

    /* The program code and other data into "FLASH" Rom type memory */
    .text :
    {
          . = ALIGN(16);
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)
        KEEP (*(.init))
        KEEP (*(.fini))
        _etext = .;        /* define a global symbols at end of code */
    } >FLASH_BANK1

    /* Constant data into "FLASH" Rom type memory */
    .rodata :
    {
          . = ALIGN(16);
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    } >FLASH_BANK1

    .ARM.extab :
    {
          . = ALIGN(16);
      *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH_BANK1

    .ARM :
    {
          . = ALIGN(16);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH_BANK1

    .preinit_array :
    {
          . = ALIGN(16);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH_BANK1

    .init_array :
    {
          . = ALIGN(16);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH_BANK1

    .fini_array :
    {
          . = ALIGN(16);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH_BANK1

    /* Used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /* Initialized data sections into "RAM" Ram type memory */
    .data :
    {
          . = ALIGN(16);
        _sdata = .;        /* create a global symbol at data start */
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        *(.RamFunc)        /* .RamFunc sections */
        *(.RamFunc*)       /* .RamFunc* sections */

        _edata = .;        /* define a global symbol at data end */
    } >RAM AT> FLASH_BANK1

    .Repository (NOLOAD) :
    {
          . = ALIGN(16);
          *(.repository)
    } > Repository

    /* Uninitialized data section into "RAM" Ram type memory */
    .bss :
    {
           . = ALIGN(16);
       /* This is used by the startup in order to initialize the .bss section */
        _sbss = .;         /* define a global symbol at bss start */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        _ebss = .;         /* define a global symbol at bss end */
        __bss_end__ = _ebss;
    } >RAM

    /* User_heap_stack section, used to check that there is enough "RAM" Ram type memory left */
    ._user_heap_stack :
    {
        . = ALIGN(16);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(16);
    } >RAM

    /* Remove information from the compiler libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }

    ld_repository_addr = ORIGIN(Repository);
    ld_repository_size = LENGTH(Repository);

}
