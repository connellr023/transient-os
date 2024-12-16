#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/interrupts/interrupts.hpp"
#include "kernel/kernel.hpp"
#include "kernel/threads/thread_control_block.hpp"
#include <stdint.h>

using namespace kernel::threads;

// struct SharedTestStruct {
// public:
//   uint64_t a;
//   uint64_t b;
//   uint64_t c;
// };

void test_task_1(void *arg) {
  uint64_t shared = reinterpret_cast<uint64_t>(arg);

  while (true) {
    {
      kernel::interrupts::disable_interrupts();

      uint64_t x2, x3;
      asm volatile("mov %0, x2" : "=r"(x2));
      asm volatile("mov %0, x3" : "=r"(x3));

      uart0::puts("\nTask 1 x2: ");
      uart0::hex(x2);
      uart0::puts("\n");
      uart0::puts("Task 1 x3: ");
      uart0::hex(x3);
      uart0::puts("\n");

      kernel::interrupts::enable_interrupts();
    }

    asm volatile("wfi");
  }
}

void test_task_2(void *arg) {
  uint64_t shared = reinterpret_cast<uint64_t>(arg);

  while (true) {
    for (int i = 0; i < 1000; i++)
      asm volatile("nop");

    {
      kernel::interrupts::disable_interrupts();

      uint64_t x2, x3;
      asm volatile("mov %0, x2" : "=r"(x2));
      asm volatile("mov %0, x3" : "=r"(x3));

      uart0::puts("\nTask 2 x2: ");
      uart0::hex(x2);
      uart0::puts("\n");
      uart0::puts("Task 2 x3: ");
      uart0::hex(x3);
      uart0::puts("\n");

      asm volatile("add x2, x2, 1");

      kernel::interrupts::enable_interrupts();
    }

    asm volatile("wfi");
  }
}

void test_task_3(void *arg) {
  uint64_t shared = reinterpret_cast<uint64_t>(arg);

  while (true) {
    for (int i = 0; i < 1000; i++)
      asm volatile("nop");

    {
      kernel::interrupts::disable_interrupts();

      uint64_t sp;
      asm volatile("mov %0, sp" : "=r"(sp));

      uart0::puts("\nTask 3 stack pointer: ");
      uart0::hex(sp);
      uart0::puts("\n");

      kernel::interrupts::enable_interrupts();
    }

    asm volatile("wfi");
  }
}

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  uint64_t test_shared = 0x69;

  // ThreadControlBlock tcb_1(&test_task_1, 1800, &test_shared);
  // ThreadControlBlock tcb_2(&test_task_2, 1200, &test_shared);
  ThreadControlBlock tcb_3(&test_task_3, 1200, &test_shared);

  // kernel::spawn_thread(&tcb_1);
  // kernel::spawn_thread(&tcb_2);
  kernel::spawn_thread(&tcb_3);

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