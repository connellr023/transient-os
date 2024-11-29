#include "kernel.hpp"
#include "../drivers/uart0.hpp"
#include "interrupts/interrupts.hpp"
#include "threads/thread_control_block.hpp"
#include <stdint.h>

using namespace kernel::threads;
using namespace kernel::interrupts;

extern ThreadQueue kernel::thread_queue = ThreadQueue();

void kernel::init() {
  uart0::init();
  init_interrupt_vector();
  init_timer();
  enable_interrupt_controller();
  enable_interrupts();
}

void kernel::init_thread(thread_handler_t handler, uint64_t stack_size) {
  static uint64_t current_high_memory_end = high_memory_end;

  // Align the stack size to 16 bytes
  stack_size = (stack_size + 0xf) & ~0xf;

  // Allocate the stack
  current_high_memory_end -= stack_size;

  // Setup thread control block
  ThreadControlBlock tcb = ThreadControlBlock();
  tcb.get_sp_registers().sp = current_high_memory_end;
  tcb.get_sp_registers().pc = reinterpret_cast<uint64_t>(handler);

  if (!thread_queue.enqueue(tcb)) {
    uart0::puts("Failed to enqueue thread\n");
    panic_handler();
  }
}

void kernel::default_panic_handler() {
  while (true) {
    uart0::puts("Kernel panic!\n");
  }
}