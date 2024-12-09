#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <stdint.h>

namespace kernel::interrupts {
// us = microseconds
constexpr uint32_t timer_interval_us = 2e6;

void enable_interrupt_controller();
void enable_interrupts();
void disable_interrupts();

void init_interrupt_vector();
void init_timer();

extern "C" {
void interrupt_service_routine(void *sp);
void post_isr();

void synch_exception_handler();
void fiq_exception_handler();
void serror_exception_handler();
}
} // namespace kernel::interrupts

#endif // INTERRUPTS_HPP