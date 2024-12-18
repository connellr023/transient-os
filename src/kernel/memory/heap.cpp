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

#include "../../../include/kernel/memory/heap.hpp"

using namespace kernel::memory;

/**
 * @brief Singleton class to manage heap pages.
 */
HeapPageManager heap_page_manager = HeapPageManager<10>();

void *kernel::memory::kmalloc(uint64_t size) {
  // Dont allocate more than a page (we cannot assume each heap page is
  // contiguous)
  if (size > PAGE_SIZE) {
    return nullptr;
  }

  // Ensure we have a page to allocate from
  if (heap_page_manager.get_current_heap_page() == nullptr) {
    void *heap_page = heap_page_manager.alloc_heap_page();

    if (!heap_page) {
      return nullptr;
    }

    // Initialize the heap page
    // Create a free block at the start of the page
    FreeListNode *free_list = reinterpret_cast<FreeListNode *>(heap_page);
    free_list->set_size(size);
    free_list->set_next(nullptr);

    void *payload = reinterpret_cast<void *>(
        reinterpret_cast<uint64_t>(heap_page) + sizeof(FreeListNode));

    return payload;
  }

  // Find a free block in the current heap page
  FreeListNode *current = reinterpret_cast<FreeListNode *>(
      heap_page_manager.get_current_heap_page());
  FreeListNode *prev = nullptr;

  while (current != nullptr) {
  }

  return nullptr; // Maybe...
}

void kernel::memory::kmfree(void *ptr) {}