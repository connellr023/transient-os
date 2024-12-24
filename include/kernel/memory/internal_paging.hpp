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

#ifndef INTERNAL_PAGING_HPP
#define INTERNAL_PAGING_HPP

#include "../peripherals/mmio.hpp"

#define PAGE_SHIFT 12 // 4KB page size
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (1 * SECTION_SIZE)
#define HIGH_MEMORY MMIO_BASE

#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define PAGE_COUNT (PAGING_MEMORY / PAGE_SIZE)

#ifndef __ASSEMBLER__

// Pages allocated to threads will have the top half for the stack and the
// bottom half for the heap Stack and heap overflows are not checked
#define THREAD_STACK_SIZE (PAGE_SIZE / 2) // Half of page for stack
#define THREAD_HEAP_SIZE                                                       \
  (PAGE_SIZE - THREAD_STACK_SIZE) // Rest of page for heap

#define MEMORY_FILL 0xB0BABABE // ;)

#include <stdint.h>

namespace kernel::memory {
/**
 * ### Page allocator (INTERNAL)
 * @brief Allocates a page of memory. This operation is not thread safe and
 * should only be invoked by a system call handler.
 * @return A pointer to the allocated page. Returns nullptr if no memory is
 * available.
 */
void *internal_page_alloc();

/**
 * ### Page deallocator (INTERNAL)
 * @brief Frees a page of memory. This operation is not thread safe and should
 * only be invoked by a system call handler.
 * @param page The page to free.
 */
void internal_page_free(void *page);
} // namespace kernel::memory

#endif // __ASSEMBLER__

#endif // INTERNAL_PAGING_HPP