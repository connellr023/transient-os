.section .text

.globl _init_interrupt_vector
_init_interrupt_vector:
        adrp    x0, _vectors
        add     x0, x0, :lo12:_vectors
        msr     vbar_el1, x0
        ret

.align 11
.globl _vectors
_vectors:
.align 7
        b       _synch_handler

.align 7
        b       _irq_handler

.align 7
        b       _fiq_handler

.align 7
        b       _serror_handler

.align 7
        b       _synch_handler //
