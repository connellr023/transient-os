#include "kernel.hpp"
#include "../drivers/uart0.hpp"
#include "interrupts/interrupts.hpp"
#include <stdint.h>

void kernel::init() {
  uart0::init();
  interrupts::init_interrupt_vector();
  interrupts::init_timer();
  interrupts::enable_interrupt_controller();
  interrupts::enable_interrupts();
}

void kernel::default_panic_handler() {
  while (true) {
    uart0::puts("Kernel panic!\n");
  }
}