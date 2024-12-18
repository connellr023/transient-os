/*
 * Copyright (C) 2024/2025 connellr023@github
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "../../include/kernel/kernel.hpp"
#include "../../include/kernel/interrupts/interrupts.hpp"
#include "../../include/kernel/sys/sys_call.hpp"
#include "../../include/kernel/sys/sys_registers.hpp"

using namespace kernel;
using namespace kernel::threads;
using namespace kernel::interrupts;

/**
 * @brief Singleton scheduler queue for the kernel.
 */
SchedulerQueue scheduler = SchedulerQueue();

/**
 * @brief The output handler for kernel debugging.
 */
output_handler_t kernel_string_output_handler = nullptr;

void kernel::set_output_handler(output_handler_t string_handler) {
  kernel_string_output_handler = string_handler;
}

bool is_kernel_started = false;

void kernel::start() {
  // Ensure we are in EL1
  uint64_t current_el;
  asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
  current_el >>= 2;

  if (current_el != 1) {
    kernel_panic("Not running in EL1");
  }

  // Initialize the interrupt controller and timer
  init_interrupt_vector();
  enable_interrupt_controller();
  enable_interrupts();

  is_kernel_started = true;

  // Trigger the first timer interrupt
  yield();
}

const ThreadControlBlock *kernel::context_switch(void *interrupted_sp) {
  static bool is_first_context_switch = true;

  if (scheduler.is_empty()) {
    kernel_panic("No threads to schedule");
  }

  // Avoid corrupting a thread's context stack
  if (!is_first_context_switch) {
    // Save context of interrupted thread
    scheduler.peek()->set_sp(interrupted_sp);
    scheduler.peek()->mark_as_ready();

    // Goto next thread
    scheduler.next();
  } else {
    // Set flag to false after first context switch
    is_first_context_switch = false;
  }

  scheduler.peek()->mark_as_running();
  return scheduler.peek();
}

bool kernel::alloc_thread_stack(ThreadControlBlock *tcb) {
  if (tcb->is_initialized()) {
    return false;
  }

  void *page = memory::alloc_page();

  if (!page) {
    return false;
  }

  const uint64_t page_addr = reinterpret_cast<uint64_t>(page);
  const uint64_t sp = page_addr + THREAD_STACK_SIZE - CPU_CTX_STACK_SIZE;

  uint64_t *register_stack = reinterpret_cast<uint64_t *>(sp);

  // Set x1 to x29 (FP) equal to 0
  for (int i = 1; i < LR_IDX; i++) {
    register_stack[i] = 0;
  }

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
  tcb->mark_as_ready();

  return true;
}

bool kernel::schedule_thread(ThreadControlBlock *tcb) {
  // Not allowed to schedule threads after kernel has started (at least for now)
  if (is_kernel_started) {
    return false;
  }

  return alloc_thread_stack(tcb) && scheduler.enqueue(tcb);
}

void kernel::yield() {
  prepare_timer_interrupt(10);
  asm volatile("wfi");
}

void kernel::safe_put(const char *str) {
  if (kernel_string_output_handler != nullptr) {
    kernel_string_output_handler(str);
  }
}

void kernel::safe_hex(uint64_t value) {
  if (kernel_string_output_handler != nullptr) {
    constexpr const char *digits = "0123456789ABCDEF";
    constexpr uint8_t buffer_size = 17;

    char buffer[buffer_size];
    buffer[buffer_size - 1] = '\0';

    for (int i = buffer_size - 2; i >= 0; i--) {
      buffer[i] = digits[value & 0xf];
      value >>= 4;
    }

    kernel_string_output_handler(buffer);
  }
}

uint64_t kernel::get_thread_id() { return scheduler.peek()->get_thread_id(); }

void kernel::thread_return_handler() {
  disable_interrupts();

  scheduler.mark_current_as_complete();
  memory::free_page(scheduler.peek()->get_page());

  // Trigger context switch
  enable_interrupts();
  yield();
}

void kernel::kernel_panic(const char *msg) {
  disable_interrupts();

  safe_put("Kernel panic: ");
  safe_put(msg);
  safe_put("\n");

  while (true) {
    asm volatile("wfe");
  }
}