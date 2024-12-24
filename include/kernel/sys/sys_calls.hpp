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

#ifndef SYS_CALL_HPP
#define SYS_CALL_HPP

#include <stdint.h>

/**
 * @brief An enumeration of possible system call codes.
 */
enum class SystemCall : uint8_t {
  PutString,
  PageAlloc,
  PageFree,
  HeapAlloc,
  HeapFree,
  Yield,
  Exit,
  Sleep,
};

namespace kernel::sys {
/**
 * @brief Utility function to trigger a system call. Can be called directly, or
 * use wrapper functions.
 * @param call_code The system call code.
 * @param arg The argument to the system call.
 * @return The return value of the system call.
 */
void *trigger_sys_call(SystemCall call_code,
                       const void *arg = nullptr) asm("_trigger_sys_call");

/**
 * @brief Triggers a system call that writes a message to the output handler.
 * The output handler must have been initialized or nothing will happen.
 */
void puts(const char *str);

/**
 * @brief Triggers a system call that allocates a page of memory.
 * @return A pointer to the allocated page or nullptr if no memory is available.
 */
void *page_alloc();

/**
 * @brief Triggers a system call that frees a page of memory.
 * @param page The page to free.
 */
void page_free(void *page);

/**
 * @brief Triggers a system call that allocates a block of memory on the heap.
 * @param size The size of the block to allocate.
 * @return A pointer to the allocated block or nullptr if no memory is
 * available.
 */
void *heap_alloc(uint64_t size);

/**
 * @brief Triggers a system call that frees a block of memory on the heap.
 * @param ptr The pointer to the block to free.
 */
void heap_free(void *ptr);

/**
 * @brief Triggers a system call that yields the current thread.
 */
void yield();

/**
 * @brief Triggers a system call that exits the current thread and cleans up any
 * resources it was using.
 */
[[noreturn]] void exit();

/**
 * @brief Triggers a system call that puts the current thread to sleep for a
 * specified number of microseconds.
 * @param sleep_us The number of microseconds to sleep.
 */
void sleep(uint32_t sleep_us);
} // namespace kernel::sys

#endif // SYS_CALL_HPP