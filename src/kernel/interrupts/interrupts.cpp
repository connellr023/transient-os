#include "interrupts.hpp"
#include "../../drivers/interrupt_requests.hpp"
#include "../../drivers/timer.hpp"
#include "../../drivers/uart0.hpp"
#include "../kernel.hpp"
#include <stdint.h>

void kernel::interrupts::interrupt_service_routine(void *sp) {
  uint64_t *stack = static_cast<uint64_t *>(sp);

  // Access saved registers
  // ...

  uart0::puts("Timed IRQ\n");
}

// us = microseconds
constexpr uint32_t timer_interval_us = 200000;
uint32_t current_us = 0;

void kernel::interrupts::init_timer() {
  current_us = *TIMER_COUNTER_LOW;
  current_us += timer_interval_us;

  // Generate an interrupt when the timer reaches the current value
  *TIMER_CMP_1 = current_us;
}

void kernel::interrupts::pre_isr() {
  current_us += timer_interval_us;

  // Generate an interrupt when the timer reaches the current value
  *TIMER_CMP_1 = current_us;

  // Clear the interrupt
  *TIMER_CS = TIMER_CS_M1;
}

void kernel::interrupts::enable_interrupt_controller() {
  // Enable the system timer IRQ
  *ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1;
}

void kernel::interrupts::enable_interrupts() {
  asm volatile("msr daifclr, #2");
}

void kernel::interrupts::disable_interrupts() {
  asm volatile("msr daifset, #2");
}