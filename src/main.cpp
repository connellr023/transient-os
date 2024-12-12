#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include "kernel/threads/thread_control_block.hpp"
#include "kernel/utils/atomic_guard.hpp"
#include <stdint.h>

using namespace kernel::utils;
using namespace kernel::threads;

void test_task_1(void *arg) {
  while (true) {
    AtomicGuard guard;
    uart0::puts("One current SP: \n");

    uint64_t sp;
    asm volatile("mov %0, sp" : "=r"(sp));

    uart0::hex(sp);
    uart0::puts("\n");
  }
}

void test_task_2(void *arg) {
  uint64_t k = 10000;

  for (int i = 0; i < 5; i++) {
    {
      AtomicGuard guard;
      uart0::puts("Two current SP: \n");

      uint64_t sp;
      asm volatile("mov %0, sp" : "=r"(sp));

      uart0::hex(sp);
      uart0::puts("\n");
    }

    k--;
  }
}

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  ThreadControlBlock tcb_1 = ThreadControlBlock(&test_task_1, 1300);
  ThreadControlBlock tcb_2 = ThreadControlBlock(&test_task_2, 1000);

  kernel::init_thread(&tcb_1);
  kernel::init_thread(&tcb_2);

  kernel::start();

  while (true) {
    asm volatile("nop");
  }

  return 0;
}