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

#ifndef INTERNAL_CPU_SCHEDULER_HPP
#define INTERNAL_CPU_SCHEDULER_HPP

#include <kernel/thread/thread_control_block.hpp>

namespace kernel::scheduler {
/**
 * ### (INTERNAL)
 * @brief Handles a context switch by updating the current thread's TCB and
 * scheduling the next thread. This function should never be called directly.
 * @param interrupted_sp The stack pointer of the interrupted thread.
 * @return The thread control block of the next thread to run.
 */
const ThreadControlBlock *internal_context_switch(void *interrupted_sp);

/**
 * ### (INTERNAL)
 * @brief Special handling for context switches when the current thread is
 * exiting.
 * @return The thread control block of the next thread to run.
 */
const ThreadControlBlock *internal_exit_context_switch();

/**
 * ### (INTERNAL)
 * @brief Marks the current thread as sleeping.
 * @param sleep_us The number of microseconds to sleep.
 */
void internal_sleep(uint32_t sleep_us);
} // namespace kernel::scheduler

#endif // INTERNAL_CPU_SCHEDULER_HPP