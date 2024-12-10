#include "kernel.hpp"
#include "interrupts/interrupts.hpp"
#include "memory/memory.hpp"
#include "threads/thread_control_block.hpp"
#include <stdint.h>

using namespace kernel::threads;
using namespace kernel::interrupts;

SchedulerQueue kernel::scheduler = SchedulerQueue();
kernel::string_output_handler_t kernel_string_output_handler = nullptr;
kernel::hex_output_handler_t kernel_hex_output_handler = nullptr;

void kernel::init_dbg_out(string_output_handler_t string_handler,
                          hex_output_handler_t hex_handler) {
  kernel_string_output_handler = string_handler;
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

void kernel::init_thread(thread_handler_t handler) {
  uint64_t page = memory::palloc();

  // Setup thread control block
  ThreadControlBlock tcb =
      ThreadControlBlock(page, reinterpret_cast<uint64_t>(handler));

  tcb.get_cpu_ctx().init_thread_stack(&panic_handler);

  if (!scheduler.enqueue(tcb)) {
    safe_put("Thread queue is full\n");
    panic_handler();
  }

  safe_put("Thread initialized\n");
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