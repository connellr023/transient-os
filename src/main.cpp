#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include <stdint.h>

// https://github.com/s-matyukevich/raspberry-pi-os

int main() {
  kernel::init();

  while (true) {
    uint64_t current_el;
    asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
    current_el >>= 2;

    uart0::puts("0x");
    uart0::hex(current_el);
    uart0::send('|');

    for (int i = 0; i < 100000; i++)
      asm volatile("nop");
  }

  return 0;
}