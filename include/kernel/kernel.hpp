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

#ifndef KERNEL_HPP
#define KERNEL_HPP

#define CPU_CTX_STACK_SIZE (17 * 16) // 17 pairs of 8 byte registers

#ifndef __ASSEMBLER__

#include "memory/paging.hpp"

#define THREAD_STACK_SIZE PAGE_SIZE

#define LR_IDX 30
#define ELR_EL1_IDX 31
#define SPSR_EL1_IDX 32

#include "tcb/thread_control_block.hpp"
#include <stdint.h>

typedef void (*output_handler_t)(const char *);

namespace kernel {
/**
 * @brief Initializes an output handler for kernel debugging.
 */
void set_output_handler(output_handler_t output_handler);

/**
 * @brief Starts the kernel and triggers the first context switch.
 * This function should be called after all initialization is done.
 * This function will not return.
 */
[[noreturn]] void start();

/**
 * @brief If something goes wrong, call this function to panic the kernel.
 * Will print a message if an output handler is set and then put the CPU in a
 * low power state.
 */
[[noreturn]] void panic(const char *msg);

/**
 * @brief Schedules a thread to run. This function is not thread safe (should
 * only be used during initialization).
 * @param tcb The thread control block of the thread.
 * @return True if the thread was scheduled, false otherwise.
 */
bool schedule_thread(ThreadControlBlock *tcb);

/**
 * @brief Prints a message to the output handler.
 * @param str The message to print.
 */
void safe_puts(const char *str);

/**
 * @brief Prints a hex value to the output handler.
 * @param value The value to print.
 */
void safe_hex(uint64_t value);

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
 * @brief Frees resources of currently scheduled thread. Should only be called
 * by a system call handler.
 */
void internal_thread_free();
} // namespace kernel

#endif // __ASSEMBLER__

#endif // KERNEL_HPP