#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include "kernel/threads/thread_control_block.hpp"
#include "utils/mutex.hpp"
#include <stdint.h>

using namespace kernel::threads;

Mutex uart_mutex = Mutex();

void test_task_1(void *arg) {
  uint64_t shared = reinterpret_cast<uint64_t>(arg);
  int k = 0x69;

  while (true) {
    asm volatile("wfi");
  }
}

void test_task_3(void *arg) {
  uint64_t shared = reinterpret_cast<uint64_t>(arg);

  while (true) {
    for (int i = 0; i < 1000; i++)

    {
      uart_mutex.lock();

      uint64_t sp;
      asm volatile("mov %0, sp" : "=r"(sp));

      uart0::puts("\nTask 3 stack pointer: ");
      uart0::hex(sp);
      uart0::puts("\n");

      uart_mutex.unlock();
    }

    asm volatile("wfi");
  }
}

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  uint64_t test_shared = 0x69;

  ThreadControlBlock tcb_1(&test_task_1, 1800, &test_shared);
  ThreadControlBlock tcb_3(&test_task_3, 1200, &test_shared);

  kernel::schedule_thread(&tcb_1);
  kernel::schedule_thread(&tcb_3);

  // Print stack pointer
  uint64_t sp;
  asm volatile("mov %0, sp" : "=r"(sp));
  uart0::puts("Main stack pointer: ");
  uart0::hex(sp);
  uart0::puts("\n");

  uart0::puts("Starting kernel\n");
  kernel::start();

  while (true) {
    uart0::puts("Waiting for kernel to start\n");
    asm volatile("wfi");
  }

  return 0;
}