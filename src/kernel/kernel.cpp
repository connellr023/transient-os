#include "kernel.hpp"
#include "interrupts/interrupts.hpp"
#include "threads/thread_control_block.hpp"
#include <stdint.h>

using namespace kernel::threads;
using namespace kernel::interrupts;

ThreadQueue kernel::thread_queue = ThreadQueue();

void kernel::init() {
  init_interrupt_vector();
  init_timer();
  enable_interrupt_controller();
  enable_interrupts();
}

void kernel::init_thread(thread_handler_t handler, uint64_t stack_size) {
  static uint64_t current_high_memory_end = (high_memory_end + 0xf) & ~0xf;

  // Align the stack size to 16 bytes
  stack_size = (stack_size + 0xf) & ~0xf;

  // Setup thread control block
  ThreadControlBlock tcb = ThreadControlBlock(
      current_high_memory_end, reinterpret_cast<uint64_t>(handler));

  if (!thread_queue.enqueue(tcb)) {
    panic_handler();
  }

  // Since stack grows downwards, we need to decrement the high memory end
  // after each thread is initialized
  current_high_memory_end -= stack_size;
}

__attribute__((weak, noreturn)) void kernel::panic_handler() {
  disable_interrupts();

  while (true) {
    asm volatile("wfe");
  }
}