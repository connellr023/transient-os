#include "memory/paging.hpp"
#include "sys_registers.hpp"

.section ".text.boot"

.globl _start
_start:
    // Read cpu id, stop slave cores
    mrs         x0, mpidr_el1
    tst         x0, 0x3
    b.eq        master
    // If CPU ID > 0, stop
hang:
    wfe
    b           hang

master:
    // Clear bss section
    ldr         x1, =__bss_start
    ldr         w2, =__bss_size

clear_bss:
    cbz         w2, done_clear
    str         xzr, [x1], #8
    sub         w2, w2, #1
    cbnz        w2, clear_bss

done_clear:
    ldr         x0, =SCTLR_EL1_VALUE
    msr         sctlr_el1, x0

    ldr         x0, =HCR_EL2_VALUE
    msr         hcr_el2, x0

    mov         x2, #SPSR_EL2_VALUE
    msr         spsr_el2, x2

    adr         x2, el1_entry
    msr         elr_el2, x2

    ldr         x0, =_start
    msr         sp_el0, x0

    // Set top of stack for SP_EL1
    sub         x1, x0, #PAGE_SIZE
    msr         sp_el1, x1

    eret

el1_entry:
    // Switch to using SP_EL0
    msr         spsel, xzr
    mov         sp, x0

    // Enter main kernel initialization function
    bl          main
    b           hang
