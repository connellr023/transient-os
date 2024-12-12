#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <stdint.h>

namespace kernel::interrupts {
void enable_interrupt_controller();
void enable_interrupts();
void disable_interrupts();

void trigger_isr();

extern "C" {
uint64_t isr(uint64_t sp) asm("_isr");
void post_isr() asm("_post_isr");

void init_interrupt_vector() asm("_init_interrupt_vector");

void synch_exception_handler() asm("_synch_exception_handler");
void fiq_exception_handler() asm("_fiq_exception_handler");
void serror_exception_handler() asm("_serror_exception_handler");
}
} // namespace kernel::interrupts

#endif // INTERRUPTS_HPP