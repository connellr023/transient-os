#include "../threads/thread_control_block.hpp"

.section .text

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
    stp x30, xzr, [sp, #16*15]

    // Save ELR_EL1 and SPSR_EL1
    mrs x0, elr_el1
    mrs x1, spsr_el1

    stp x0, x1, [sp, #16*16]
.endm

.macro pop_registers
    // Restore ELR_EL1 and SPSR_EL1
    ldp x0, x1, [sp, #16*16]

    msr elr_el1, x0
    msr spsr_el1, x1

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
    ldp x30, xzr, [sp, #16*15]

    add sp, sp, #CPU_CTX_STACK_SIZE
.endm

.globl _irq_handler
_irq_handler:
    push_registers

    // Pass the base address of the saved registers to the interrupt service routine
    mov x0, sp
    bl _context_switch

    // Update the stack pointer to the next thread
    mov sp, x0

    // Prepare SP_EL0 for the next thread
    add x0, x0, #CPU_CTX_STACK_SIZE
    msr sp_el0, x0

    pop_registers
    eret

.globl _synch_handler
_synch_handler:
    bl _synch_exception_handler
    eret

.globl _fiq_handler
_fiq_handler:
    bl _fiq_exception_handler
    eret

.globl _serror_handler
_serror_handler:
    bl _serror_exception_handler
    eret
