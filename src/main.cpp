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
    uart0::puts("One\n");
  }
}

void test_task_2(void *arg) {
  uint64_t k = 10000;

  for (int i = 0; i < 5; i++) {
    {
      AtomicGuard guard;
      uart0::puts("Two i: ");
      uart0::hex(i);
      uart0::puts("\n");
      uart0::puts("Two k: ");
      uart0::hex(k);
      uart0::puts("\n");
    }

    k--;
  }
}

ThreadControlBlock tcb_1;
ThreadControlBlock tcb_2;

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  tcb_1 = ThreadControlBlock(&test_task_1, 1300);
  tcb_2 = ThreadControlBlock(&test_task_2, 1000);

  // Print tcb_1 pc
  uart0::puts("Thread 1 pc: 0x");
  uart0::hex(reinterpret_cast<uint64_t>(&test_task_1));
  uart0::puts("\n");

  kernel::init_thread(&tcb_1);
  kernel::init_thread(&tcb_2);

  kernel::start();

  while (true) {
    asm volatile("nop");
  }

  return 0;
}