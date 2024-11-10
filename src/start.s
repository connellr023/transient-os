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
    cbz     w2, n
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, c

n:
    ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0		

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

	ldr	x0, =SPSR_VALUE
	msr	spsr_el3, x0

	adr	x0, el1_entry		
	msr	elr_el3, x0

    eret

el1_entry:
    // Jump to C code, should not return
    bl      main
    // For failsafe, halt this core too
    b       hang