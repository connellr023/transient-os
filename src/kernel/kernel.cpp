#include "kernel.hpp"
#include "../drivers/uart0.hpp"
#include <stdint.h>

void kernel::init() { uart0::init(); }

void kernel::default_panic_handler() {
  uart0::puts("Kernel panic!\n");

  while (true)
    ;
}