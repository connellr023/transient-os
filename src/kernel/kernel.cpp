#include "kernel.hpp"
#include "interrupts/interrupts.hpp"
#include "threads/thread_control_block.hpp"
#include <stdint.h>

using namespace kernel::threads;
using namespace kernel::interrupts;

ThreadQueue kernel::thread_queue = ThreadQueue();
kernel::string_output_handler_t kernel_string_output_handler = nullptr;
kernel::hex_output_handler_t kernel_hex_output_handler = nullptr;

void kernel::init(string_output_handler_t output_handler,
                  hex_output_handler_t hex_handler) {
  kernel_string_output_handler = output_handler;
  kernel_hex_output_handler = hex_handler;

  // Ensure we are in EL1
  uint64_t current_el;
  asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
  current_el >>= 2;

  if (current_el != 1) {
    safe_string_output("Not running in EL1\n");
    panic_handler();
  }

  // Initialize the interrupt controller and timer
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
    safe_string_output("Thread queue is full\n");
    panic_handler();
  }

  // Since stack grows downwards, we need to decrement the high memory end
  // after each thread is initialized
  current_high_memory_end -= stack_size;
}

void kernel::safe_string_output(const char *str) {
  if (kernel_string_output_handler != nullptr) {
    kernel_string_output_handler(str);
  }
}

void kernel::safe_hex_output(uint64_t value) {
  if (kernel_hex_output_handler != nullptr) {
    kernel_hex_output_handler(value);
  }
}

__attribute__((weak, noreturn)) void kernel::panic_handler() {
  disable_interrupts();

  while (true) {
    safe_string_output("Kernel Panic!\n");
    asm volatile("wfe");
  }
}