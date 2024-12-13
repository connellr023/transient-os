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
  uint64_t k = 0x69;
  int j = 0x420;

  while (true) {
    // Print k
    {
      AtomicGuard guard;
      uart0::puts("Thread 1 k: ");
      uart0::hex(k);
      uart0::puts("\n");
    }

    // Print j
    {
      AtomicGuard guard;
      uart0::puts("Thread 1 j: ");
      uart0::hex(j);
      uart0::puts("\n");
    }
  }
}

void test_task_2(void *arg) {
  uint64_t shared_struct = reinterpret_cast<uint64_t>(arg);
  int i = 0;

  while (true) {
    // Print address of shared struct
    {
      AtomicGuard guard;
      uart0::puts("Thread 2 shared_struct: ");
      uart0::hex(reinterpret_cast<uint64_t>(shared_struct));
      uart0::puts("\n");
    }

    // shared_struct->c++;

    // {
    //   AtomicGuard guard;
    //   uart0::puts("i: ");
    //   uart0::hex(i);
    //   uart0::puts("\n");
    // }

    // i++;
  }
}

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  SharedTestStruct shared_struct = {.a = 0, .b = 0, .c = 0};

  ThreadControlBlock tcb_1(&test_task_1, 1300, &shared_struct);
  ThreadControlBlock tcb_2(&test_task_2, 1000, &shared_struct);

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