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

#ifndef CPU_SCHEDULER_HPP
#define CPU_SCHEDULER_HPP

#include <kernel/thread/thread_control_block.hpp>

namespace kernel::scheduler {
/**
 * @brief Enqueues a thread in the primary scheduler queue.
 * @param tcb The thread control block to enqueue.
 * @return True if the thread was enqueued, false if the queue is full.
 */
bool enqueue(ThreadControlBlock *tcb);

/**
 * @brief Gets the current thread's thread control block.
 * @return The thread control block of the current thread.
 */
const ThreadControlBlock *get_current_thread();

/**
 * @brief Handles a context switch by updating the current thread's TCB and
 * scheduling the next thread. This function should never be called directly.
 * @param interrupted_sp The stack pointer of the interrupted thread.
 * @return The thread control block of the next thread to run.
 */
const ThreadControlBlock *context_switch(void *interrupted_sp);

/**
 * @brief Special handling for context switches when the current thread is
 * exiting.
 * @return The thread control block of the next thread to run.
 */
const ThreadControlBlock *exit_context_switch();

/**
 * @brief Marks the current thread as sleeping.
 * @param sleep_us The number of microseconds to sleep.
 */
void sleep_current(uint32_t sleep_us);
} // namespace kernel::scheduler

#endif // CPU_SCHEDULER_HPP