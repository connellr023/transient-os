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
  SharedTestStruct *shared_struct = reinterpret_cast<SharedTestStruct *>(arg);

  for (uint64_t i = 0; i < 0xFFUL; i++) {
    {
      AtomicGuard guard;
      uart0::puts("Thread 1 i: ");
      uart0::hex(i);
      uart0::puts("\n");
    }
  }

  {
    AtomicGuard guard;
    uart0::puts("Thread 1 returning\n");
  }

  while (true) {
    asm volatile("wfi");
  }
}

void test_task_2(void *arg) {
  SharedTestStruct *shared_struct = reinterpret_cast<SharedTestStruct *>(arg);

  for (uint64_t i = 0x69; i < 0xFFUL; i += 2) {
    {
      AtomicGuard guard;
      uart0::puts("Thread 2 i: ");
      uart0::hex(i);
      uart0::puts("\n");
    }
  }

  {
    AtomicGuard guard;
    uart0::puts("Thread 2 returning\n");
  }

  while (true) {
    asm volatile("wfi");
  }
}

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  SharedTestStruct shared_struct = {.a = 0, .b = 0, .c = 0};

  ThreadControlBlock tcb_1(&test_task_1, 2000, &shared_struct);
  ThreadControlBlock tcb_2(&test_task_2, 2000, &shared_struct);

  if (!kernel::spawn_thread(&tcb_1)) {
    uart0::puts("Failed to spawn thread 1\n");
  }

  if (!kernel::spawn_thread(&tcb_2)) {
    uart0::puts("Failed to spawn thread 2\n");
  }

  uart0::puts("Starting kernel\n");

  kernel::start();

  while (true) {
    uart0::puts("Waiting for kernel to start\n");
    asm volatile("wfi");
  }

  return 0;
}