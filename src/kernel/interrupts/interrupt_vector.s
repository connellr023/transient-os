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

.section .text

.globl _init_interrupt_vector
_init_interrupt_vector:
        ldr             x0, =_vectors
        msr             vbar_el1, x0
        ret

.globl _enable_preemption
_enable_preemption:
        msr             daifclr, #2
        ret

.globl _disable_preemption
_disable_preemption:
        msr             daifset, #2
        ret

.macro vec_entry label
.align 7
        b               \label
.endm

.macro skip_vec_entry
.align 7
        nop
.endm

.align 11
.globl _vectors
_vectors:
        // EL1t handlers
        vec_entry       _synch_handler
        vec_entry       _irq_handler
        skip_vec_entry
        skip_vec_entry

        // EL1h handlers
        skip_vec_entry
        skip_vec_entry
        skip_vec_entry
        skip_vec_entry

        // EL0 handlers
        vec_entry       _synch_handler
        vec_entry       _irq_handler
        skip_vec_entry
        skip_vec_entry

        // EL0 32-bit handlers
        skip_vec_entry
        skip_vec_entry
        skip_vec_entry
        skip_vec_entry
