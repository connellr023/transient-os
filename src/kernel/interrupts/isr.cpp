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

#include <kernel/interrupts/interrupts.hpp>
#include <kernel/interrupts/isr.hpp>
#include <kernel/kernel.hpp>
#include <kernel/scheduler/cpu_scheduler.hpp>
#include <kernel/sys/sys_call_handler.hpp>
#include <kernel/thread/thread_allocator.hpp>
#include <kernel/thread/thread_control_block.hpp>

namespace kernel::interrupts {
void *irq_exception_handler(void *interrupted_sp) {
  const ThreadControlBlock *next_tcb =
      scheduler::context_switch(interrupted_sp);

  prepare_timer_interrupt(next_tcb->get_quantum());
  clear_timer_interrupt();

  // Return the stack pointer of the next thread
  return next_tcb->get_sp();
}

void *synch_exception_handler(SystemCall call_code, void *arg,
                              void *interrupted_sp) {
  // Check if the exception was a system call
  uint64_t ec_val;
  asm volatile("mrs %0, esr_el1" : "=r"(ec_val));
  ec_val = (ec_val >> 26) & 0x3F;

  const SynchExceptionClass ec = static_cast<SynchExceptionClass>(ec_val);

  switch (ec) {
  case SynchExceptionClass::SVC: {
    // Handle system call
    break;
  }
  case SynchExceptionClass::Unknown:
  case SynchExceptionClass::InstructionAbort:
  case SynchExceptionClass::DataAbort: {
    dbg_putln("Segmentation fault (thread killed)");

    // Kill the current thread
    call_code = SystemCall::Exit;
    break;
  }
  default: {
    panic("Unimplemented exception class handler");
  }
  }

  // Special handling for exit system call
  if (call_code == SystemCall::Exit) {
    const ThreadControlBlock *next_tcb = scheduler::exit_context_switch();

    prepare_timer_interrupt(next_tcb->get_quantum());
    clear_timer_interrupt();

    return next_tcb->get_sp();
  }

  // Get the interrupted stack pointer
  uint64_t *sp = reinterpret_cast<uint64_t *>(interrupted_sp);

  // Handle all other system calls
  const PSRMode callee_mode = static_cast<PSRMode>(sp[SPSR_EL1_IDX] & 0xF);
  void *value = sys::handle_sys_call(call_code, callee_mode, arg);

  // Write return value to x0 register on the interrupted stack
  sp[0] = reinterpret_cast<uintptr_t>(value);

  // Switch to a different thread
  return irq_exception_handler(interrupted_sp);
}
} // namespace kernel::interrupts