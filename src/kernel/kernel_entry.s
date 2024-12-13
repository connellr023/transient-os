#include "memory/paging.hpp"

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
    // Enable AArch64 in EL1 by setting bits RW and SWIC to 1 in the
    // Hypervisor Configuration Register (see p. D10-2492 and D10-2503 in
    // the ARM Architecture Reference Manual). Since all other bits are 0,
    // most instructions are not trapped, and the Physical SError, IRQ, and
    // FIQ routings are set so that these exceptions are not taken to EL2,
    // but are instead handled at EL1.
    mov         x0, (1<<31)// Enable AArch64
    orr         x0, x0, (1<<1)// SWIO is hardwired on the Pi
    msr         hcr_el2, x0

    // Change execution level to EL1:
    //
    // Set the Saved Program Status Register so that when entering EL1, the
    // DAIF bits are set to 1111 (exceptions are masked) and the M[3:2] bits
    // are set to 01 (EL1) and the M[0] bit is set to 0 (SP is always SP0)
    // (see p. C5-386-387 in the ARM Architecture Reference Manual).
    mov         x2, 0x3C4
    msr         spsr_el2, x2

    adr         x2, el1_entry
    msr         elr_el2, x2

    // Start stack pointer for kernel initialization stack right below code
    adrp        x1, _start
    add         x1, x1, :lo12:_start
    msr         sp_el0, x1
    mov         sp, x1

    // After first context switch, we will switch to using SP_EL0 in the low memory region
    mov         x0, #LOW_MEMORY
    msr         sp_el1, x0

    // Select SP_EL0 for EL1
    msr         spsel, xzr

    eret

el1_entry:
    // Enter main kernel initialization function
    bl          main
    b           hang
