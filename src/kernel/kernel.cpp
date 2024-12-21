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
#include "../../include/kernel/scheduler/cpu_scheduler.hpp"

namespace kernel {
/**
 * @brief The output handler for kernel debugging.
 */
output_handler_t kernel_string_output_handler = nullptr;

/**
 * @brief Flag to indicate if the kernel has started.
 */
bool is_kernel_started = false;

void set_output_handler(output_handler_t string_handler) {
  kernel_string_output_handler = string_handler;
}

bool is_started() { return is_kernel_started; }

void start() {
  // Ensure we are in EL1
  uint64_t current_el;
  asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
  current_el >>= 2;

  if (current_el != 1) {
    panic("Not running in EL1");
  }

  // Initialize the interrupt controller and timer
  interrupts::init_interrupt_vector();
  interrupts::enable_interrupt_controller();
  interrupts::enable_preemption();

  is_kernel_started = true;

  // Trigger the first timer interrupt
  interrupts::prepare_timer_interrupt(15);

  while (true) {
    asm volatile("wfi");
  }
}

bool prepare_thread(ThreadControlBlock *tcb) {
  return tcb->alloc() && scheduler::enqueue(tcb);
}

void safe_puts(const char *str) {
  if (kernel_string_output_handler != nullptr) {
    kernel_string_output_handler(str);
  }
}

void safe_hex(uint64_t value) {
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

void panic(const char *msg) {
  interrupts::disable_preemption();

  safe_puts("Kernel panic: ");
  safe_puts(msg);
  safe_puts("\n");

  while (true) {
    asm volatile("wfe");
  }
}
} // namespace kernel