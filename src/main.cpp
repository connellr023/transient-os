#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/interrupts/interrupts.hpp"
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

  if (!framebuffer::init()) {
    while (true) {
      uart0::puts("Framebuffer initialization failed\n");
    }

    return 1;
  }

  uart0::init();
  kernel::init(&uart0::puts, &uart0::hex);

  framebuffer::fill_screen(0x00FFFF);

  while (true) {
    // Print to UART atomically
    kernel::interrupts::disable_interrupts();
    uart0::puts("Main Atomic!\n");
    kernel::interrupts::enable_interrupts();

    // uart0::puts("Main\n");
    // framebuffer::draw_rect(100, 100, 200, 200, 0xFF0000);
    //  uart0::puts("Main\n");
  }

  return 0;
}