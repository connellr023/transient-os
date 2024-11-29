.section .text

.align 11
_vectors:
.align 7
        b       _synch_handler

.align 7
        b       _irq_handler

.align 7
        b       _fiq_handler

.align 7
        b       _serror_handler

.globl init_interrupt_vector
init_interrupt_vector:
        adrp    x2, _vectors
        add     x2, x2, :lo12:_vectors
        msr     vbar_el1, x2
        ret
