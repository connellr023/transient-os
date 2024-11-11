/*
 * Copyright (C) 2018 bzt (bztsrc//github)
 * Modified by 2024 connellr023//github
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "system/sys_registers.hpp"

.section ".text.boot"

.globl _start
_start:
    // Read cpu id, stop slave cores
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbz     x0, master
    // If cpu id > 0, stop
hang:
    b       hang

master:
    // Set top of stack just before our code (stack grows to a lower address per AAPCS64)
    ldr     x1, =_start
    mov     sp, x1

    // Clear bss section
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size

c: 
    cbz     w2, el3_entry
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, c

el3_entry:
    bl print_current_el

    // Initialize SCTLR_EL2 and HCR_EL2 to save values before entering EL2
    msr sctlr_el2, xzr
    msr hcr_el2, xzr

    // Determine the EL2 execution state
    mrs x0, scr_el3
    orr x0, x0, #(1 << 10) // RW EL2 Execution state is AArch64
    orr x0, x0, #(1 << 0) // NS EL2 Execution state is Non-secure
    msr scr_el3, x0
    mov x0, #0b01001 // DAIF = 0000
    msr spsr_el3, x0 // M[4:0] = 01001 EL2h must match SCR_EL3.RW

    // Determine EL2 entry point
    adr x0, el2_entry
    msr elr_el3, x0

    eret

el2_entry:
    bl print_current_el

    // Initialize SCTLR_EL1 to save values before entering EL1
    msr sctlr_el1, xzr

    // Determine the EL1 execution state
    mrs x0, hcr_el2
    orr x0, x0, #(1 << 31) // RW EL1 Execution state is AArch64
    msr hcr_el2, x0
    mov x0, #0b00101 // DAIF = 0000
    msr spsr_el2, x0 // M[4:0] = 00101 EL1h must match HCR_EL2.RW

    adr x0, el1_entry
    msr elr_el2, x0

    eret

el1_entry:
    bl print_current_el

    // Jump to C code, should not return
    bl      main
    // For failsafe, halt this core too
    b       hang