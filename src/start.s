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
.section ".text.boot"

.globl _start
_start:
    // Read cpu id, stop slave cores
    mrs     x0, mpidr_el1
    tst     x0, 0x3
    b.eq    master
    // If cpu id > 0, stop
hang:
    wfe
    b       hang

master:
    // Clear bss section
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size

_c: 
    cbz     w2, done_clear
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, _c

done_clear:
    // Set top of stack just before our code (stack grows to a lower address per AAPCS64)
    adrp    x1, _start
    add     x1, x1, :lo12:_start
    msr     sp_el1, x1

    // Enable AArch64 in EL1 by setting bits RW and SWIC to 1 in the
	// Hypervisor Configuration Register (see p. D10-2492 and D10-2503 in
	// the ARM Architecture Reference Manual). Since all other bits are 0,
	// most instructions are not trapped, and the Physical SError, IRQ, and
	// FIQ routings are set so that these exceptions are not taken to EL2,
	// but are instead handled at EL1.
	mov	    x0, (1 << 31)		// Enable AArch64
	orr	    x0, x0, (1 << 1)	// SWIO is hardwired on the Pi
	msr	    hcr_el2, x0

    // Set the Vector Base Address Register (EL1) to the address of the
	// vectors defined below
	adrp	x2, _vectors
	add	    x2, x2, :lo12:_vectors
	msr     vbar_el1, x2

    // Change execution level to EL1:
	//
	// Set the Saved Program Status Register so that when entering EL1, the
	// DAIF bits are set to 1111 (exceptions are masked) and the M[3:2] bits
	// are set to 01 (EL1) and the M[0] bit is set to 0 (SP is always SP0)
	// (see p. C5-386-387 in the ARM Architecture Reference Manual).
	mov	    x2, 0x3C4
	msr	    spsr_el2, x2

	adr	    x2, el1_entry
	msr	    elr_el2, x2

    eret

el1_entry:
    mov     sp, x1

    // Jump to C code, should not return
    bl      main
    // For failsafe, halt this core too
    b       hang