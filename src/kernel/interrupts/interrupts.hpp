#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

namespace kernel::interrupts {
void enable_interrupts();
void disable_interrupts();

extern "C" {
void interrupt_service_routine();
}
} // namespace kernel::interrupts

#endif // INTERRUPTS_HPP