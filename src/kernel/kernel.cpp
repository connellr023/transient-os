#include "kernel.hpp"
#include "../drivers/uart0.hpp"
#include "interrupts/interrupts.hpp"
#include <stdint.h>

void kernel::init() {
  uart0::init();
  interrupts::init_interrupt_vector();
  interrupts::enable_interrupts();
}

void kernel::default_panic_handler() {
  uart0::puts("Kernel panic!\n");

  while (true)
    ;
}