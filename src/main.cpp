#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
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
  Mutex uart_mutex;
};

void test_task_1(void *arg) {
  SharedTestStruct *shared_struct = reinterpret_cast<SharedTestStruct *>(arg);

  for (uint64_t i = 0; i < 0xFFUL; i++) {
    asm volatile("nop");
  }

  // uart_mutex.acquire();
  // uart0::puts("Thread 1 shared struct values: a b c\n");
  // uart0::hex(shared_struct->a);
  // uart0::puts(" ");
  // uart0::hex(shared_struct->b);
  // uart0::puts(" ");
  // uart0::hex(shared_struct->c);
  // uart0::puts("\n");
  // uart_mutex.release();

  while (true) {
    asm volatile("wfi");
  }
}

void test_task_2(void *arg) {
  int j = 0;
  SharedTestStruct *shared_struct = reinterpret_cast<SharedTestStruct *>(arg);

  for (uint64_t i = 0; i < 0xFFUL; i += 2) {
    asm volatile("nop");

    j = reinterpret_cast<uint64_t>(shared_struct);
  }

  shared_struct->uart_mutex.acquire();
  j += 5;
  // uart0::hex(shared_struct->a);
  // uart0::puts(" ");
  // uart0::hex(shared_struct->b);
  // uart0::puts(" ");
  // uart0::hex(shared_struct->c);
  // uart0::puts("\n");

  // // Print address of shared struct
  // uart0::puts("Thread 2 shared struct address: ");
  // uart0::hex(reinterpret_cast<uint64_t>(shared_struct));
  // uart0::puts("\n");
  shared_struct->uart_mutex.release();

  while (true) {
    asm volatile("wfi");
  }
}

int main() {
  SharedTestStruct shared_struct;
  shared_struct.a = 0x1234567890ABCDEF;
  shared_struct.b = 0xFEDCBA0987654321;
  shared_struct.c = 0xCAFEBABEDEADBEEF;
  shared_struct.uart_mutex = Mutex();

  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  ThreadControlBlock tcb_1(&test_task_1, 5000, &shared_struct);
  ThreadControlBlock tcb_2(&test_task_2, 5000, &shared_struct);

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