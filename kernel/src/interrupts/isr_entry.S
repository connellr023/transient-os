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

#include <kernel/include/thread/thread_allocator.hpp>

.section .text

    // Platform register
    pr .req x18

.macro push_registers
    sub sp, sp, #CPU_CTX_STACK_SIZE

    // Save that state of all general purpose registers
    stp x0, x1, [sp, #16*0]
    stp x2, x3, [sp, #16*1]
    stp x4, x5, [sp, #16*2]
    stp x6, x7, [sp, #16*3]
    stp x8, x9, [sp, #16*4]
    stp x10, x11, [sp, #16*5]
    stp x12, x13, [sp, #16*6]
    stp x14, x15, [sp, #16*7]
    stp x16, x17, [sp, #16*8]
    stp x18, x19, [sp, #16*9]
    stp x20, x21, [sp, #16*10]
    stp x22, x23, [sp, #16*11]
    stp x24, x25, [sp, #16*12]
    stp x26, x27, [sp, #16*13]
    stp x28, x29, [sp, #16*14]

    // Save ELR_EL1 and SPSR_EL1
    mrs x22, elr_el1
    mrs x23, spsr_el1

    // Push LR, ELR_EL1, and SPSR_EL1 onto the stack
    stp x30, x22, [sp, #16*15]
    str x23, [sp, #16*16]
.endm

.macro pop_registers
    // Restore SPSR_EL1, ELR_EL1, and LR
    ldr x23, [sp, #16*16]
    ldp x30, x22, [sp, #16*15]

    msr elr_el1, x22
    msr spsr_el1, x23

    // Restore state of all general purpose registers
    ldp x0, x1, [sp, #16*0]
    ldp x2, x3, [sp, #16*1]
    ldp x4, x5, [sp, #16*2]
    ldp x6, x7, [sp, #16*3]
    ldp x8, x9, [sp, #16*4]
    ldp x10, x11, [sp, #16*5]
    ldp x12, x13, [sp, #16*6]
    ldp x14, x15, [sp, #16*7]
    ldp x16, x17, [sp, #16*8]
    ldp x18, x19, [sp, #16*9]
    ldp x20, x21, [sp, #16*10]
    ldp x22, x23, [sp, #16*11]
    ldp x24, x25, [sp, #16*12]
    ldp x26, x27, [sp, #16*13]
    ldp x28, x29, [sp, #16*14]

    add sp, sp, #CPU_CTX_STACK_SIZE
.endm

.globl _irq_handler
_irq_handler:
    mrs pr, sp_el0
    mov sp, pr

    push_registers

    // Pass the base address of the saved registers to the interrupt service routine
    mov x0, sp
    bl _irq_exception_handler

    // Update the stack pointer to the next thread
    mov sp, x0

    // Prepare SP_EL0 for the next thread
    add x0, x0, #CPU_CTX_STACK_SIZE
    msr sp_el0, x0

    pop_registers
    eret

.globl _synch_handler
_synch_handler:
    mrs pr, sp_el0
    mov sp, pr

    push_registers

    mov x1, x0 // Move syscall argument into x1
    mov w0, w8 // Move call code into w0
    mov x2, sp // Move interrupted stack pointer into x2
    bl _synch_exception_handler

    // Update the stack pointer to the next thread
    mov sp, x0

    // Prepare SP_EL0 for the next thread
    add x0, x0, #CPU_CTX_STACK_SIZE
    msr sp_el0, x0

    pop_registers
    eret
