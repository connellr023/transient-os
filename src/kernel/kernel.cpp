#include "kernel.hpp"
#include "interrupts/interrupts.hpp"
#include "memory/paging.hpp"
#include "sys_registers.hpp"
#include <stdint.h>

using namespace kernel::threads;
using namespace kernel::interrupts;

SchedulerQueue scheduler = SchedulerQueue();

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
    asm volatile("wfe");
  }

  // Initialize the interrupt controller and timer
  init_interrupt_vector();
  enable_interrupt_controller();
  enable_interrupts();

  // Trigger the first context switch
  trigger_timer_interrupt();
}

const ThreadControlBlock *kernel::context_switch(void *interrupted_sp) {
  static bool is_first_context_switch = true;

  // Avoid corrupting the first thread's context when switching from main
  if (!is_first_context_switch) {
    // Save context of interrupted thread
    scheduler.peek()->set_sp(interrupted_sp);
    scheduler.peek()->set_state(ThreadState::Ready);

    // Goto next thread
    scheduler.next();
  } else {
    // Set flag to false after first context switch
    is_first_context_switch = false;
  }

  scheduler.peek()->set_state(ThreadState::Running);
  return scheduler.peek();
}

bool kernel::alloc_thread_stack(ThreadControlBlock *tcb) {
  void *page = memory::palloc();

  if (!page) {
    return false;
  }

  const uint64_t page_addr = reinterpret_cast<uint64_t>(page);
  const uint64_t sp = page_addr - CPU_CTX_STACK_SIZE;

  uint64_t *register_stack = reinterpret_cast<uint64_t *>(sp);

  // Set x1 to x29 (FP) equal to 0
  for (int i = 1; i < LR_IDX; i++) {
    register_stack[i] = 0;
  }

  // Test
  register_stack[1] = 0x1111111111111111;
  register_stack[2] = 0x2222222222222222;
  register_stack[3] = 0x3333333333333333;
  register_stack[4] = 0x4444444444444444;
  register_stack[5] = 0x5555555555555555;

  // Set argument to x0
  register_stack[0] = reinterpret_cast<uint64_t>(tcb->get_arg());

  // Set x30 (LR) to the thread return handler
  register_stack[LR_IDX] = reinterpret_cast<uint64_t>(&thread_return_handler);

  // Set ELR_EL1 to the thread handler
  register_stack[ELR_EL1_IDX] = reinterpret_cast<uint64_t>(tcb->get_handler());

  // Set SPSR_EL1 to the initial value
  register_stack[SPSR_EL1_IDX] = INITIAL_SPSR_EL1_VALUE;

  tcb->set_page(page);
  tcb->set_sp(reinterpret_cast<void *>(sp));

  return true;
}

bool kernel::spawn_thread(ThreadControlBlock *tcb) {
  if (!alloc_thread_stack(tcb)) {
    return false;
  }

  if (!scheduler.enqueue(tcb)) {
    memory::pfree(tcb->get_page());
    return false;
  }

  return true;
}

void kernel::join_thread(ThreadControlBlock *tcb) {
  while (!tcb->get_is_complete()) {
    asm volatile("wfi");
  }
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

  ThreadControlBlock *current_tcb = scheduler.peek();
  current_tcb->mark_complete();

  // Free thread page
  memory::pfree(current_tcb->get_page());

  // Remove thread from scheduler
  scheduler.dequeue();

  // Trigger context switch
  interrupts::enable_interrupts();
  interrupts::trigger_timer_interrupt();
}