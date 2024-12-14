#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/interrupts/interrupts.hpp"
#include "kernel/kernel.hpp"
#include "kernel/threads/thread_control_block.hpp"
#include "kernel/utils/mutex.hpp"
#include <stdint.h>

using namespace kernel::utils;
using namespace kernel::threads;

struct SharedTestStruct {
public:
  uint64_t a;
  uint64_t b;
  uint64_t c;
};

Mutex a_mutex = Mutex();

void test_task_1(void *arg) {
  SharedTestStruct *shared_struct = reinterpret_cast<SharedTestStruct *>(arg);

  for (uint64_t i = 0; i < 0xFF; i++) {
    shared_struct->a += 1;
  }
}

void test_task_2(void *arg) {
  SharedTestStruct *shared_struct = reinterpret_cast<SharedTestStruct *>(arg);

  for (uint64_t i = 0; i < 0xFFUL; i += 1) {
    shared_struct->a += 1;
  }
}

int main() {
  SharedTestStruct shared_struct;
  shared_struct.a = 0;
  shared_struct.b = 0xFEDCBA0987654321;
  shared_struct.c = 0xCAFEBABEDEADBEEF;

  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  ThreadControlBlock tcb_1(&test_task_1, 2000, &shared_struct);
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