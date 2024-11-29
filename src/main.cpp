#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include <stdint.h>

// https://github.com/s-matyukevich/raspberry-pi-os

int main() {
  kernel::init();

  while (true) {
    asm volatile("wfi");
  }

  return 0;
}