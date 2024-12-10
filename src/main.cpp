#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include "kernel/utils/atomic_guard.hpp"
#include <stdint.h>

using namespace kernel::utils;

void test_task_1(void *arg) {
  while (true) {
    AtomicGuard guard;
    uart0::puts("One\n");
  }
}

// void test_task_2(void *arg) {
//   while (true) {
//     AtomicGuard guard;
//     uart0::puts("Two\n");
//   }
// }

int main() {
  if (!framebuffer::init()) {
    while (true) {
      uart0::puts("Framebuffer initialization failed\n");
    }

    return 1;
  }

  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  kernel::init_thread(0);
  kernel::init_thread(&test_task_1);

  framebuffer::fill_screen(0x00FFFF);
  kernel::start();

  while (true) {
    AtomicGuard guard;
    uart0::puts("Main\n");
  }

  return 0;
}