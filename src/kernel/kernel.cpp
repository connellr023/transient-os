#include "kernel.hpp"
#include "interrupts/interrupts.hpp"
#include "memory/paging.hpp"
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

    while (true) {
      asm volatile("wfe");
    }
  }

  // Initialize the interrupt controller and timer
  init_interrupt_vector();
  enable_interrupt_controller();
  enable_interrupts();

  // Trigger the first context switch
  trigger_isr();
}

bool kernel::spawn_thread(ThreadControlBlock *tcb) {
  void *page = memory::palloc();

  if (!page) {
    return false;
  }

  tcb->init_stack(page);

  if (!scheduler.enqueue(tcb)) {
    memory::pfree(page);
    return false;
  }

  return true;
}

void kernel::yield_current_thread() {
  scheduler.peek()->set_state(ThreadState::Ready);
  trigger_isr();
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

void kernel::thread_return_handler() {
  interrupts::disable_interrupts();

  const ThreadControlBlock *current_tcb = scheduler.peek();

  safe_put("Thread ID: ");
  safe_hex(current_tcb->get_thread_id());
  safe_put("completed!\n");

  // Free thread page
  memory::pfree(current_tcb->get_page());

  // Remove thread from scheduler
  scheduler.dequeue();

  // Trigger context switch
  interrupts::enable_interrupts();
  interrupts::trigger_isr();
}