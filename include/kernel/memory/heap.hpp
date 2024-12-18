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

namespace kernel::memory {
/**
 * @brief A node in the heap allocator's free list.
 */
class FreeListNode {
private:
  uint64_t size;
  FreeListNode *next;

public:
  FreeListNode(uint64_t size, FreeListNode *next = nullptr)
      : size(size), next(next) {}

  uint64_t get_size() const { return size; }
  FreeListNode *get_next() const { return next; }

  void set_size(uint64_t size) { this->size = size; }
  void set_next(FreeListNode *next) { this->next = next; }
};

/**
 * @brief A class to encapsulate the management of heap pages.
 */
template <uint8_t max_heap_pages> class HeapPageManager {
private:
  /**
   * @brief Array of pointers to the start of each heap page.
   */
  uint8_t (*heap_pages)[max_heap_pages] = {nullptr};

  /**
   * @brief The index of the next heap page to allocate.
   */
  uint64_t heap_page_index;

public:
  HeapPageManager() : heap_page_index(0) {}

  void *get_current_heap_page() {
    return this->heap_pages[this->heap_page_index];
  }

  void *alloc_heap_page() {
    if (this->heap_page_index >= max_heap_pages) {
      return nullptr;
    }

    if (this->heap_pages[this->heap_page_index] == nullptr) {
      this->heap_pages[this->heap_page_index] = palloc();
    }

    return this->heap_pages[this->heap_page_index];
  }

  void next_heap_page() {
    if (this->heap_page_index < max_heap_pages) {
      this->heap_page_index++;
    }
  }
};

/**
 * @brief Allocates a block of memory on the heap.
 * @param size The size of the block to allocate.
 * @return A pointer to the allocated block. Returns nullptr if no memory is
 * available.
 */
void *kmalloc(uint64_t size);

/**
 * @brief Frees a block of memory on the heap.
 * @param ptr The pointer to the block to free.
 */
void kmfree(void *ptr);
} // namespace kernel::memory

#endif // HEAP_HPP