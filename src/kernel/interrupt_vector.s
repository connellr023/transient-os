.section    .text

.macro  vector_entry    label
.align  7
b       \label
.endm

.macro  call_handle_invalid_entry    error_code
mov     x0, \error_code
bl      handle_invalid_entry
.endm

.align  11
.globl  vectors
vectors:
        vector_entry    sync_invalid_el1t
        vector_entry    irq_invalid_el1t
        vector_entry    fiq_invalid_el1t
        vector_entry    error_invalid_el1t

        vector_entry    sync_invalid_el1h
        vector_entry    handle_interrupt
        vector_entry    fiq_invalid_el1h
        vector_entry    error_invalid_el1h

        vector_entry    sync_invalid_el0_64
        vector_entry    irq_invalid_el0_64
        vector_entry    fiq_invalid_el0_64
        vector_entry    error_invalid_el0_64

        vector_entry    sync_invalid_el0_32
        vector_entry    irq_invalid_el0_32
        vector_entry    fiq_invalid_el0_32
        vector_entry    error_invalid_el0_32

sync_invalid_el1t:
        call_handle_invalid_entry #0

irq_invalid_el1t:
        call_handle_invalid_entry #1

fiq_invalid_el1t:
        call_handle_invalid_entry #2

error_invalid_el1t:
        call_handle_invalid_entry #3

sync_invalid_el1h:
        call_handle_invalid_entry #4

fiq_invalid_el1h:
        call_handle_invalid_entry #6

error_invalid_el1h:
        call_handle_invalid_entry #7

sync_invalid_el0_64:
        call_handle_invalid_entry #8

irq_invalid_el0_64:
        call_handle_invalid_entry #9

fiq_invalid_el0_64:
        call_handle_invalid_entry #10

error_invalid_el0_64:
        call_handle_invalid_entry #11

sync_invalid_el0_32:
        call_handle_invalid_entry #12

irq_invalid_el0_32:
        call_handle_invalid_entry #13

fiq_invalid_el0_32:
        call_handle_invalid_entry #14

error_invalid_el0_32:
        call_handle_invalid_entry #15