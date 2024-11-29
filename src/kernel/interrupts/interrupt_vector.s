.section .text

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
