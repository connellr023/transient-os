#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include <stdint.h>

// void test_task_1() {
//   while (true) {
//     uart0::puts("One\n");
//   }
// }

// void test_task_2() {
//   while (true) {
//     uart0::puts("Two\n");
//   }
// }

int main() {
  // kernel::init_thread(&test_task_1, 1024);
  // kernel::init_thread(&test_task_2, 1024);

  // uart0::init();
  framebuffer::init();
  kernel::init();

  while (true) {
    // for (int i = 0; i < 15; i++) {
    //   uart0::puts("Main\n");
    // }

    framebuffer::fill_screen(0x00FF00);

    // uint8_t current_el;
    // asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
    // current_el >>= 2;

    // if (current_el == 1) {
    //   uart0::puts("Main EL1\n");
    // } else if (current_el == 2) {
    //   uart0::puts("Main EL2\n");
    // } else if (current_el == 3) {
    //   uart0::puts("Main EL3\n");
    // } else {
    //   uart0::puts("Main ELX\n");
    // }
  }

  return 0;
}