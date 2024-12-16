#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/kernel.hpp"
#include "kernel/threads/thread_control_block.hpp"
#include "utils/atomic.hpp"
#include <stdint.h>

using namespace kernel::threads;

struct PrintTaskArgs {
public:
  const char *msg;
  uint64_t count;

  PrintTaskArgs() : msg(nullptr), count(0) {}
  PrintTaskArgs(const char *msg, uint64_t count) : msg(msg), count(count) {}
};

void print_task(void *arg) {
  PrintTaskArgs *args = reinterpret_cast<PrintTaskArgs *>(arg);

  // Print thread ID
  {
    AtomicBlock guard;
    uart0::hex(kernel::get_thread_id());
    uart0::puts(": ");
    uart0::puts("Starting thread\n");
  }

  asm volatile("wfi");

  for (uint64_t i = 0; i < args->count; i++) {
    {
      AtomicBlock guard;
      uart0::hex(kernel::get_thread_id());
      uart0::puts(": ");
      uart0::puts(args->msg);
    }
  }

  {
    AtomicBlock guard;
    uart0::hex(kernel::get_thread_id());
    uart0::puts(": ");
    uart0::puts("Thread complete\n");
  }
}

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  // Create and schedule n tasks
  constexpr uint64_t n = 6;
  ThreadControlBlock handles[n];

  PrintTaskArgs handle_args("Thread!\n", 3);

  for (uint64_t i = 0; i < n; i++) {
    handles[i] = ThreadControlBlock(&print_task, 3000, &handle_args);

    if (!kernel::schedule_thread(handles + i)) {
      uart0::puts("Failed to schedule thread\n");
      break;
    }
  }

  uart0::puts("Starting kernel\n");
  kernel::start();

  return 0;
}