MEMORY {
    RAM_ITCM  (xrw) : ORIGIN = 0x00000000, LENGTH = 0x00004000
    FLASH_SYS (xr ) : ORIGIN = 0x00100000, LENGTH = 0x0000edc0
    FLASH_ITCM(xr ) : ORIGIN = 0x00200000, LENGTH = 0x00100000
    FLASH_AXIM(xr ) : ORIGIN = 0x08000000, LENGTH = 0x00100000
    SRAM_DTCM (xrw) : ORIGIN = 0x20000000, LENGTH = 0x00010000
    SRAM1     (xrw) : ORIGIN = 0x20010000, LENGTH = 0x0003c000
    SRAM2     (xrw) : ORIGIN = 0x2004c000, LENGTH = 0x00004000
}

SECTIONS {
    .text : {
        KEEP(*(.entry))
    } > SRAM_DTCM

    .data : {
        _sdata = . ;
        *(.dta)
        *(.data.*)
        _edata = . ;
         . = ALIGN(4);
    } > SRAM_DTCM 

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > SRAM_DTCM 

    .ARM.extab : {
        __extab_start = .;
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        __extab_end = .;
    } > SRAM_DTCM 

    .bss : {
        _sbss = .;
        *(.bss)
        _ebss = .;
         . = ALIGN(4);
    } > SRAM_DTCM 

    __stack_top = ORIGIN(SRAM_DTCM) + LENGTH(SRAM_DTCM) - 1;
    __stack_size = 4K;
}

