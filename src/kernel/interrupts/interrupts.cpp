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

#include "../../../include/kernel/interrupts/interrupts.hpp"
#include "../../../include/kernel/kernel.hpp"
#include "../../../include/kernel/peripherals/timer_interrupt.hpp"
#include "../../../include/kernel/threads/thread_control_block.hpp"

using namespace kernel::sys;

void kernel::interrupts::prepare_timer_interrupt(uint64_t interval) {
  static uint32_t current_us = 0;

  current_us = *TIMER_COUNTER_LOW;
  *TIMER_CMP_1 = current_us + interval;
}

void kernel::interrupts::clear_timer_interrupt() {
  // Clear the M1 bit in the control/status register
  *TIMER_CS = TIMER_CS_M1;
}

void kernel::interrupts::enable_interrupt_controller() {
  // Enable the system timer IRQ
  *ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1;
}

void *kernel::interrupts::irq_exception_handler(void *interrupted_sp) {
  const ThreadControlBlock *next_tcb = context_switch(interrupted_sp);

  // Prepare timer for next context switch
  prepare_timer_interrupt(next_tcb->get_burst_time());
  clear_timer_interrupt();

  // Return the stack pointer of the next thread
  return next_tcb->get_sp();
}

void *kernel::interrupts::synch_exception_handler(SystemCall call_code,
                                                  void *arg,
                                                  void *interrupted_sp) {
  // Check if the exception was a system call
  uint64_t ec;
  asm("mrs %0, esr_el1" : "=r"(ec));
  ec >>= 26;

  if (ec != SVC_EC) {
    kernel_panic("Non-SVC synchronous exception occurred");
  }

  void *value = handle_system_call(call_code, arg);

  // Write return value to x0 register on the interrupted stack
  uint64_t *sp = reinterpret_cast<uint64_t *>(interrupted_sp);
  sp[0] = reinterpret_cast<uint64_t>(value);

  // Switch to a different thread
  return irq_exception_handler(interrupted_sp);
}