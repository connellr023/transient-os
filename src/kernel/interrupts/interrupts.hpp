#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <stdint.h>

namespace kernel::interrupts {
// us = microseconds
constexpr uint32_t timer_interval_us = 800000;

void enable_interrupt_controller();
void enable_interrupts();
void disable_interrupts();

void init_interrupt_vector();
void init_timer();

extern "C" {
void interrupt_service_routine(void *sp);
void post_isr();
}
} // namespace kernel::interrupts

#endif // INTERRUPTS_HPP