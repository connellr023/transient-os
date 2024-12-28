SECTIONS
{
    /* Start address for the binary */
    . = 0x80000; /* 0xFFFF000000080000 */

    /* Text section (code) */
    .text : {
        KEEP(*(.text.boot)) /* Keep startup code */
        *(.text .text.* .gnu.linkonce.t*) /* Main text section */
    }

    /* Read-only data section */
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }

    . = ALIGN(4096);

    /* Data section */
    PROVIDE(_data = .);
    .data : { *(.data .data.* .gnu.linkonce.d*) }

    /* BSS section (zero-initialized data) */
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }

    /* Define BSS size in bytes */
    __bss_size = (__bss_end - __bss_start);
    
    . = ALIGN(4096);

    /* Define end of binary */
    _end = .;

    /* Discard unused sections */
    /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}

/* Define entry point */
ENTRY(_start)
