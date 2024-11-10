#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include <stdint.h>

// https://github.com/s-matyukevich/raspberry-pi-os

int main() {
  kernel::init();

  while (true) {
    uart0::send('a');

    for (int i = 0; i < 10000; i++)
      asm volatile("nop");

    uart0::send('v');

    for (int i = 0; i < 10000; i++)
      asm volatile("nop");
  }

  return 0;
}