#include "kernel.hpp"
#include "interrupts/interrupts.hpp"
#include "threads/thread_control_block.hpp"
#include <stdint.h>

using namespace kernel::threads;
using namespace kernel::interrupts;

SchedulerQueue kernel::scheduler = SchedulerQueue();
kernel::string_output_handler_t kernel_string_output_handler = nullptr;
kernel::hex_output_handler_t kernel_hex_output_handler = nullptr;

void kernel::init_debug_io(string_output_handler_t output_handler,
                           hex_output_handler_t hex_handler) {
  kernel_string_output_handler = output_handler;
  kernel_hex_output_handler = hex_handler;
}

void kernel::start() {
  // Ensure we are in EL1
  uint64_t current_el;
  asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
  current_el >>= 2;

  if (current_el != 1) {
    safe_put("Not running in EL1\n");
    panic_handler();
  }

  // Initialize the interrupt controller and timer
  init_interrupt_vector();
  init_timer();
  enable_interrupt_controller();
  enable_interrupts();
}

void kernel::init_thread(thread_handler_t handler, uint64_t stack_size,
                         void *arg) {
  static uint64_t current_high_memory_end = mem_align_16(high_memory_end);

  // Align the stack size to 16 bytes
  stack_size = mem_align_16(stack_size);

  // Setup thread control block
  ThreadControlBlock tcb = ThreadControlBlock(
      current_high_memory_end, reinterpret_cast<uint64_t>(handler));

  tcb.get_cpu_ctx().init_thread_stack();

  if (!scheduler.enqueue(tcb)) {
    safe_put("Thread queue is full\n");
    panic_handler();
  }

  // Since stack grows downwards, we need to decrement the high memory end
  // after each thread is initialized
  current_high_memory_end -= stack_size;
}

void kernel::safe_put(const char *str) {
  if (kernel_string_output_handler != nullptr) {
    kernel_string_output_handler(str);
  }
}

void kernel::safe_hex(uint64_t value) {
  if (kernel_hex_output_handler != nullptr) {
    kernel_hex_output_handler(value);
  }
}

void kernel::panic_handler() {
  disable_interrupts();

  while (true) {
    safe_put("Kernel panic!\n");
    asm volatile("wfe");
  }
}