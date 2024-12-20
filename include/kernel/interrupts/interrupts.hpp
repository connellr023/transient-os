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

#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include "../../../include/kernel/sys/sys_call_handler.hpp"

#define SVC_EC 0x15

namespace kernel::interrupts {
using namespace sys;

/**
 * @brief Enables the system timer interrupt controller.
 */
void enable_interrupt_controller();

/**
 * @brief Initializes the interrupt vector.
 */
void init_interrupt_vector() asm("_init_interrupt_vector");

/**
 * @brief Enables preemptive interrupt requests.
 */
void enable_preemption() asm("_enable_preemption");

/**
 * @brief Disables preemptive interrupt requests.
 */
void disable_preemption() asm("_disable_preemption");

/**
 * @brief Prepares a timer interrupt to fire after a given interval.
 * @param interval The number of cycles to wait before firing the interrupt.
 */
void prepare_timer_interrupt(uint64_t interval);

/**
 * ### (INTERNAL) IRQ Exception Handler
 * @brief Handles an interrupt request exception.
 * @param interrupted_sp The stack pointer at the time of the interrupt.
 * @return The new stack pointer.
 */
void *internal_irq_exception_handler(void *interrupted_sp) asm(
    "_internal_irq_exception_handler");

/**
 * ### (INTERNAL) Synchronous Exception Handler
 * @brief Handles a synchronous exception.
 * @param call_code The system call code. (SVC only)
 * @param arg The argument to the system call. (SVC only)
 * @return Stack pointer of the next thread.
 */
void *internal_synch_exception_handler(
    SystemCall call_code, void *arg,
    void *interrupted_sp) asm("_internal_synch_exception_handler");
} // namespace kernel::interrupts

#endif // INTERRUPTS_HPP