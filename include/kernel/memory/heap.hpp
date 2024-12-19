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

#ifndef HEAP_HPP
#define HEAP_HPP

#include "paging.hpp"
#include <stdint.h>

#define MAX_HEAP_PAGES 10

namespace kernel::memory {
/**
 * @brief A node in the heap allocator's free list.
 */
class FreeListNode {
private:
  uint64_t size;
  FreeListNode *next;

public:
  void init(uint64_t size, FreeListNode *next = nullptr) {
    this->size = size;
    this->next = next;
  }

  uint64_t get_size() const { return size; }
  FreeListNode *get_next() const { return next; }

  void set_size(uint64_t size) { this->size = size; }
  void set_next(FreeListNode *next) { this->next = next; }
};

/**
 * ### Kernel memory heap allocator
 * @brief Allocates a block of memory on the heap. This is not thread safe and
 * should only be invoked by a system call handler or if mutual exclusion is
 * guaranteed.
 * @param size The size of the block to allocate.
 * @return A pointer to the allocated block. Returns nullptr if no memory is
 * available.
 */
void *km_heap_alloc(uint64_t size);

/**
 * ### Kernel memory heap free
 * @brief Frees a block of memory on the heap. This is not thread safe and
 * should only be invoked by a system call handler or if mutual exclusion is
 * guaranteed.
 * @param ptr The pointer to the block to free.
 */
void km_heap_free(void *ptr);
} // namespace kernel::memory

#endif // HEAP_HPP