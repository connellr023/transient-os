#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

namespace kernel::interrupts {
void enable_interrupt_controller();
void enable_interrupts();
void disable_interrupts();
void init_timer();

extern "C" {
void init_interrupt_vector();
void pre_isr();
void interrupt_service_routine();
}
} // namespace kernel::interrupts

#endif // INTERRUPTS_HPP