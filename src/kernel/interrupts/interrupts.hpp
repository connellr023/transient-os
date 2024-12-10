#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <stdint.h>

namespace kernel::interrupts {
// us = microseconds
constexpr uint32_t timer_interval_us = 1e6;

void enable_interrupt_controller();
void enable_interrupts();
void disable_interrupts();

void init_interrupt_vector();
void init_timer();

uint64_t isr(uint64_t sp);
void post_isr();
void synch_exception_handler();
void fiq_exception_handler();
void serror_exception_handler();
} // namespace kernel::interrupts

// Unmangled function names for assembly linkage
extern "C" {
uint64_t _kernel_isr(uint64_t sp);
void _kernel_post_isr();
void _kernel_synch_exception_handler();
void _kernel_fiq_exception_handler();
void _kernel_serror_exception_handler();
}

#endif // INTERRUPTS_HPP