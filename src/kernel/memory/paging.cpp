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

#include <kernel/memory/paging.hpp>

extern const uint64_t _end;

namespace kernel::memory {
/**
 * @brief Array of booleans representing the whether or not a page is allocated.
 */
bool page_table[PAGE_COUNT] = {false};

void *kernel_page_alloc() {
  for (uint64_t i = 0; i < PAGE_COUNT; i++) {
    if (!page_table[i]) {
      page_table[i] = true;
      uint64_t *page_addr =
          reinterpret_cast<uint64_t *>(LOW_MEMORY + (i * PAGE_SIZE));

      // Fill page with a default fill value
      for (uint64_t j = 0; j < PAGE_SIZE / sizeof(uint64_t); j++) {
        page_addr[j] = MEMORY_FILL;
      }

      return reinterpret_cast<void *>(page_addr);
    }
  }

  return nullptr;
}

void kernel_page_free(void *page) {
  const uint64_t page_addr = reinterpret_cast<uintptr_t>(page);
  page_table[(page_addr - LOW_MEMORY) / PAGE_SIZE] = false;
}
} // namespace kernel::memory