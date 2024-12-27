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

#ifndef ISR_HPP
#define ISR_HPP

#include <api/sys/sys_calls.hpp>

enum class SynchExceptionClass : uint8_t {
  Unknown = 0x0,
  SVC = 0x15,
  InstructionAbort = 0x20,
  DataAbort = 0x25,
};

namespace kernel::interrupts {
/**
 * ### (INTERNAL) IRQ Exception Handler
 * @brief Handles an interrupt request exception.
 * @param interrupted_sp The stack pointer at the time of the interrupt.
 * @return The new stack pointer.
 */
void *irq_exception_handler(void *interrupted_sp) asm("_irq_exception_handler");

/**
 * ### (INTERNAL) Synchronous Exception Handler
 * @brief Handles a synchronous exception.
 * @param call_code The system call code. (SVC only)
 * @param arg The argument to the system call. (SVC only)
 * @return Stack pointer of the next thread.
 */
void *
synch_exception_handler(SystemCall call_code, void *arg,
                        void *interrupted_sp) asm("_synch_exception_handler");
} // namespace kernel::interrupts

#endif // ISR_HPP