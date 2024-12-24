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

#include "../../../include/kernel/interrupts/internal_isr.hpp"
#include "../../../include/kernel/interrupts/interrupts.hpp"
#include "../../../include/kernel/kernel.hpp"
#include "../../../include/kernel/scheduler/internal_cpu_scheduler.hpp"
#include "../../../include/kernel/sys/internal_sys_call_handler.hpp"
#include "../../../include/kernel/tcb/thread_control_block.hpp"

namespace kernel::interrupts {
void *internal_irq_exception_handler(void *interrupted_sp) {
  const ThreadControlBlock *next_tcb =
      scheduler::internal_context_switch(interrupted_sp);

  prepare_timer_interrupt(next_tcb->get_quantum());
  clear_timer_interrupt();

  // Return the stack pointer of the next thread
  return next_tcb->get_sp();
}

void *internal_synch_exception_handler(SystemCall call_code, void *arg,
                                       void *interrupted_sp) {
  // Check if the exception was a system call
  uint64_t ec;
  asm volatile("mrs %0, esr_el1" : "=r"(ec));
  ec >>= 26;

  if (ec != SVC_EC) {
    panic("Non-SVC synchronous exception occurred");
  }

  // Special handling for exit system call
  if (call_code == SystemCall::Exit) {
    const ThreadControlBlock *next_tcb =
        scheduler::internal_exit_context_switch();

    prepare_timer_interrupt(next_tcb->get_quantum());
    clear_timer_interrupt();

    return next_tcb->get_sp();
  }

  // Handle all other system calls
  void *value = sys::internal_handle_sys_call(call_code, arg);

  // Write return value to x0 register on the interrupted stack
  uint64_t *sp = reinterpret_cast<uint64_t *>(interrupted_sp);
  sp[0] = reinterpret_cast<uint64_t>(value);

  // Switch to a different thread
  return internal_irq_exception_handler(interrupted_sp);
}
} // namespace kernel::interrupts