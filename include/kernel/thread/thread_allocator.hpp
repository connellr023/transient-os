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

#ifndef INTERNAL_THREAD_ALLOCATOR_HPP
#define INTERNAL_THREAD_ALLOCATOR_HPP

#define CPU_CTX_STACK_SIZE (34 * 8) // 34 registers * 8 bytes per register

#ifndef __ASSEMBLER__

#include <api/handler_types.hpp>
#include <kernel/thread/thread_control_block.hpp>

#define FP_IDX 29
#define LR_IDX 30
#define ELR_EL1_IDX 31
#define SPSR_EL1_IDX 32

enum class PSRMode : uint64_t {
  EL0t = 0b0000,
  EL1t = 0b0100,
};

namespace kernel::thread {
/**
 * @brief Allocates and initializes a page of memory for a new thread.
 * @param handler The handler function for the thread.
 * @param quantum_us The time quantum for the thread.
 * @param mode Is the initial value of the PSR mode bits.
 * @param arg The argument to pass to the thread handler.
 * @return A pointer to the thread control block for the new thread. Will be
 * nullptr if allocation fails.
 */
ThreadControlBlock *kernel_thread_alloc(thread_handler_t handler,
                                        uint32_t quantum_us, PSRMode mode,
                                        void *arg = nullptr);
} // namespace kernel::thread

#endif // __ASSEMBLER__

#endif // INTERNAL_THREAD_ALLOCATOR_HPP