#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include "kernel/threads/thread_control_block.hpp"
#include "kernel/utils/atomic_guard.hpp"
#include <stdint.h>

using namespace kernel::utils;
using namespace kernel::threads;

struct SharedTestStruct {
public:
  uint64_t a;
  uint64_t b;
  uint64_t c;
};

void test_task_1(void *arg) {
  // Print link register
  {
    AtomicGuard guard;
    uint64_t lr;
    asm volatile("mov %0, lr" : "=r"(lr));

    uart0::puts("Thread 1 LR: ");
    uart0::hex(lr);
    uart0::puts("\n");
  }

  for (uint64_t i = 0; i < 40000000; i++) {
    AtomicGuard guard;
    uart0::puts("Thread 1 i: ");
    uart0::hex(i);
    uart0::puts("\n");
  }

  {
    AtomicGuard guard;
    uart0::puts("Thread 1 returning\n");
  }
}

void test_task_2(void *arg) {
  for (uint64_t i = 0; i < 50000000; i++) {
    AtomicGuard guard;
    uart0::puts("Thread 2 i: ");
    uart0::hex(i);
    uart0::puts("\n");
  }

  {
    AtomicGuard guard;
    uart0::puts("Thread 2 returning\n");
  }
}

// SharedTestStruct shared;

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  // shared.a = 0x69;
  // shared.b = 0x420;
  // shared.c = 0x1337;

  ThreadControlBlock tcb_1(&test_task_1, 1300);
  ThreadControlBlock tcb_2(&test_task_2, 1000);

  if (!kernel::spawn_thread(&tcb_1)) {
    uart0::puts("Failed to spawn thread 1\n");
  }

  if (!kernel::spawn_thread(&tcb_2)) {
    uart0::puts("Failed to spawn thread 2\n");
  }

  kernel::start();

  while (true) {
    asm volatile("wfe");
  }

  return 0;
}