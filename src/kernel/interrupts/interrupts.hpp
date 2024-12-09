#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#define STACK_FRAME_SIZE (16 * 16)

#ifndef __ASSEMBLER__

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

__attribute__((noreturn)) void synch_exception_handler();
__attribute__((noreturn)) void fiq_exception_handler();
__attribute__((noreturn)) void serror_exception_handler();
}
} // namespace kernel::interrupts

#endif // __ASSEMBLER__

#endif // INTERRUPTS_HPP