/*
 * Copyright (C) 2024 Integrated Detector Electronics AS
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains
 * the property of Integrated Detector Electronics AS and its suppliers,
 * if any. The intellectual and technical concepts contained
 * herein are proprietary to Integrated Detector Electronics AS
 * and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Integrated Detector Electronics AS.
 *
 */

/**
 * \date   IDEAS/15.01.2021/aeols
 * \brief  Linker script for DHU EM
 */

INCLUDE ../Common/Common_RMU/Memory_layout.ld

MEMORY
{
    image_area (rw)     : ORIGIN = ORIGIN(app_load_area), LENGTH = LENGTH(app_load_area)
    data_sram_area (rw) : ORIGIN = ORIGIN(app_data_area), LENGTH = LENGTH(app_data_area)
}


STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x3000;
HEAP_SIZE = DEFINED(HEAP_SIZE) ? HEAP_SIZE : DEFINED(__heap_size__) ? __heap_size__ : 0x1000;

ENTRY(ISR_reset);
ENTRY(interrupt_vectors);

/* Section Definitions */
SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        _sfixed = .;
        KEEP(*(.vectors .vectors.*))
        KEEP(*(.ISR .ISR.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)

        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        _efixed = .;            /* End of text section */
    } > utility_load_area

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > utility_load_area
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4);
    _etext = .;

    .relocate : AT (_etext)
    {
        . = ALIGN(4);
        _srelocate = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        . = ALIGN(4);
        _erelocate = .;
        _fill_grade_ = ((SIZEOF(.text)+SIZEOF(.ARM.exidx)+SIZEOF(.relocate)) * 1000) / LENGTH(utility_load_area);
        ASSERT((_fill_grade_ < 1000), "<> Error: the internal flash is full <>" );
    } > app_data_area

    /* section used for the global application object data */
    .application_data (NOLOAD) :
    {
        . = ALIGN(4);
        _sappl = . ;
        *(.application_data)
        . = ALIGN(4);
        _eappl = . ;
    } > data_tcm_area

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = . ;
        _szero = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = . ;
        _ezero = .;
    } > app_data_area

    /* heap section */
    .heap (NOLOAD):
    {
        . = ALIGN(8);
         _sheap = .;
        . = . + HEAP_SIZE;
        . = ALIGN(8);
        _eheap = .;
    } > app_data_area

    /* stack section */
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        _sstack = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _estack = .;
    } > app_data_area

    . = ALIGN(4);
    _end = . ;

    /* .management_repos section is used for the persistent Management Repository */
    .management_repos (NOLOAD) :
    {
        . = ALIGN(4);
        *(.management_repos)
    } > management_repos

    /* .bookkeeping_area section is used for the persistent Bookkeeping */
    .bookkeeping_area (NOLOAD) :
    {
        . = ALIGN(4);
        *(.bookkeeping_area)
    } > bookkeeping_area

    /* .volatile_file section is used for the volatile file */
    .volatile_file (NOLOAD) :
    {
        . = ALIGN(4);
        *(.volatile_file)
    } > volatile_file


/* NOTE THESE ARE PLACEHOLDERS IF MOVING TM/TC QUEUES SHOULD BE NEEDED */
    /* .volatile_tc_queue section is used for the Volatile TCs */
    .volatile_tc_queue (NOLOAD) :
    {
        . = ALIGN(8);
        *(.volatile_tc_queue)
    } > volatile_tc_queue

    /* .volatile_tm_queue section is used for the Volatile TMs */
    .volatile_tm_queue (NOLOAD) :
    {
        . = ALIGN(8);
        *(.volatile_tm_queue)
    } > volatile_tm_queue

    /* .filestore_repos section is used for the persistent Filestore Repository */
    .filestore_repos (NOLOAD) :
    {
        . = ALIGN(4);
        *(.filestore_repos)
    } > filestore_repos

    /* .tm_queue_repos section is used for the persistent telemetry queue */
    .tm_queue_repos (NOLOAD) :
    {
        . = ALIGN(4);
        *(.tm_queue_repos)
    } > tm_queue_repos

    __bss_start__ = _sbss;
    __bss_end__ = _ebss;

    __ram_end__ = ORIGIN(app_data_area) + LENGTH(app_data_area) - 4;

    ld_text_size    = SIZEOF(.text);
    ld_exidx_size   = SIZEOF(.ARM.exidx);
    ld_vector_start = _sfixed;
    ld_data_source  = _etext;
    ld_data_start   = _srelocate;
    ld_data_size    = SIZEOF(.relocate);
    ld_appl_start   = _sappl;
    ld_appl_size    = SIZEOF(.application_data);
    ld_bss_start    = _sbss;
    ld_bss_size     = SIZEOF(.bss);
    ld_heap_start   = _sheap;
    ld_heap_end     = _eheap;
    ld_heap_size    = SIZEOF(.heap);
    ld_stack_start  = _sstack;
    ld_stack_end    = _estack;
    ld_stack_size   = SIZEOF(.stack);
    ld_text_size    = SIZEOF(.text);
    ld_exidx_size   = SIZEOF(.ARM.exidx);

/*  deprecated script section ?
    ld_script_start = _ld_script_start_;
    ld_script_end   = _ld_script_end_;
*/
    ld_instr_tcm_addr = ORIGIN(instr_tcm_area);
    ld_instr_tcm_size = LENGTH(instr_tcm_area);

    ld_flash_addr = ORIGIN(utility_load_area);
    ld_flash_size = LENGTH(utility_load_area);

    ld_data_tcm_addr = ORIGIN(data_tcm_area);
    ld_data_tcm_size = LENGTH(data_tcm_area);

/*  Must be defined at target level given the image built (bootloader_area, app_load_area, utility_load_area) */
    ld_image_load_area_addr = ORIGIN(utility_load_area);
    ld_image_load_area_size = LENGTH(utility_load_area);

    ld_data_load_area_addr = ORIGIN(app_data_area);
    ld_data_load_area_size = LENGTH(app_data_area);

    ld_bootloader_addr = ORIGIN(bootloader_area);
    ld_bootloader_size = LENGTH(bootloader_area);

    ld_management_repos_addr = ORIGIN(management_repos);
    ld_management_repos_size = LENGTH(management_repos);

    ld_bookkeeping_area_addr = ORIGIN(bookkeeping_area);
    ld_bookkeeping_area_size = LENGTH(bookkeeping_area);

    ld_tm_queue_repos_addr = ORIGIN(tm_queue_repos);
    ld_tm_queue_repos_size = LENGTH(tm_queue_repos);

    ld_filestore_volatile_addr = ORIGIN(volatile_file);
    ld_filestore_volatile_size = LENGTH(volatile_file);

    ld_filestore_repos_addr = ORIGIN(filestore_repos);
    ld_filestore_repos_size = LENGTH(filestore_repos);

    ld_ext_mram1_start = ORIGIN(bootloader_area);
    ld_ext_mram1_end = ORIGIN(mram1_reserved);

    ld_ext_mram2_start = ORIGIN(filestore_repos);
    ld_ext_mram2_end = ORIGIN(mram2_reserved);

    ld_ext_sram_start = ORIGIN(volatile_file);
    ld_ext_sram_end = ORIGIN(sram_reserved);
}
