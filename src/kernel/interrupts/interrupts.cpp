#include "interrupts.hpp"
#include "../../drivers/uart0.hpp"
#include "../kernel.hpp"

void kernel::interrupts::interrupt_service_routine() { uart0::puts("IRQ\n"); }

void kernel::interrupts::enable_interrupts() {
  asm volatile("msr daifclr, #2");
}

void kernel::interrupts::disable_interrupts() {
  asm volatile("msr daifset, #2");
}