    /*
     * Copyright (C) 2024/2025 connellr023@github
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
     */

#include <kernel/memory/paging.hpp>
#include <kernel/sys/sys_registers.hpp>

.section .text.boot

.globl _start
_start:
    // Read cpu id, stop slave cores
    mrs         x0, mpidr_el1
    tst         x0, 0x3
    b.eq        _master

    // If CPU ID > 0, stop
_hang:
    wfe
    b           _hang

_master:
    // Clear bss section
    ldr         x1, =__bss_start
    ldr         w2, =__bss_size

_clear_bss:
    cbz         w2, _done_clear
    str         xzr, [x1], #8
    sub         w2, w2, #1
    cbnz        w2, _clear_bss

_done_clear:
    ldr         x0, =SCTLR_EL1_VALUE
    msr         sctlr_el1, x0

    ldr         x0, =HCR_EL2_VALUE
    msr         hcr_el2, x0

    ldr         x0, =SPSR_EL2_VALUE
    msr         spsr_el2, x0

    adr         x2, _el1_entry
    msr         elr_el2, x2

    ldr         x0, =_start
    msr         sp_el0, x0

    // Set top of stack for SP_EL1
    sub         x1, x0, #PAGE_SIZE
    msr         sp_el1, x1

    eret

_el1_entry:
    // Switch to using SP_EL0
    msr         spsel, xzr
    mov         sp, x0

    // Enter kernel start function
    bl          _kernel_start
    b           _hang
