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
  SharedTestStruct *shared = reinterpret_cast<SharedTestStruct *>(arg);

  while (true) {
    {
      AtomicGuard guard;
      uart0::puts("Thread 1 arg.a: ");
      uart0::hex(reinterpret_cast<uint64_t>(shared->a));
      uart0::puts("\n");
    }
  }
}

// void test_task_2(void *arg) {
//   SharedTestStruct *shared = reinterpret_cast<SharedTestStruct *>(arg);

//   while (true) {
//     {
//       AtomicGuard guard;
//       uart0::puts("Thread 2 arg.b: ");
//       uart0::hex(reinterpret_cast<uint64_t>(shared->b));
//       uart0::puts("\n");
//     }

//     if (shared->b++ == 1000) {
//       shared->b = 0;
//       uart0::puts("Thread 2 b hitting 1k\n");
//     }
//   }
// }

// void test_task_3(void *arg) {
//   SharedTestStruct *shared = reinterpret_cast<SharedTestStruct *>(arg);

//   while (true) {
//     {
//       AtomicGuard guard;
//       uart0::puts("Thread 3 arg.c: ");
//       uart0::hex(reinterpret_cast<uint64_t>(shared->b));
//       uart0::puts("\n");
//     }
//   }
// }

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  SharedTestStruct shared = {0x12345678, 0x87654321, 0xdeadbeef};

  ThreadControlBlock tcb_1(&test_task_1, 1300,
                           reinterpret_cast<void *>(&shared));
  // ThreadControlBlock tcb_2(&test_task_2, 1000,
  //                          reinterpret_cast<void *>(&shared));
  // ThreadControlBlock tcb_3(&test_task_3, 1500,
  //                          reinterpret_cast<void *>(&shared));

  if (!kernel::spawn_thread(&tcb_1)) {
    uart0::puts("Failed to spawn thread 1\n");
  }

  // if (!kernel::spawn_thread(&tcb_2)) {
  //   uart0::puts("Failed to spawn thread 2\n");
  // }

  // if (!kernel::spawn_thread(&tcb_3)) {
  //   uart0::puts("Failed to spawn thread 3\n");
  // }

  kernel::start();

  while (true) {
    asm volatile("wfe");
  }

  return 0;
}