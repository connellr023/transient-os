#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <stdint.h>

#define TIMER_CS ((volatile uint32_t *)(MMIO_BASE + 0x00003000))
#define TIMER_COUNTER_LOW ((volatile uint32_t *)(MMIO_BASE + 0x00003004))
#define TIMER_COUNTER_HIGH ((volatile uint32_t *)(MMIO_BASE + 0x00003008))

#define TIMER_CMP_1 ((volatile uint32_t *)(MMIO_BASE + 0x00003010))
#define TIMER_CS_M1 (1 << 1)

namespace kernel::interrupts {
void enable_interrupt_controller();
void enable_interrupts();
void disable_interrupts();

void trigger_isr();

void *isr(void *interrupted_sp) asm("_isr");
void post_isr() asm("_post_isr");

void init_interrupt_vector() asm("_init_interrupt_vector");

void synch_exception_handler() asm("_synch_exception_handler");
void fiq_exception_handler() asm("_fiq_exception_handler");
void serror_exception_handler() asm("_serror_exception_handler");
} // namespace kernel::interrupts

#endif // INTERRUPTS_HPP