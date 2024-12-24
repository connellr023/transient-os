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

#include "../../../include/kernel/memory/internal_heap.hpp"
#include <stddef.h>

namespace kernel::memory {
/**
 * @brief Aligns a value to the next multiple of the alignment.
 */
constexpr uint64_t align_up(uint64_t value) {
  return (value + alignof(max_align_t) - 1) & ~(alignof(max_align_t) - 1);
}

void *internal_heap_alloc(FreeListNode *start_node, uint64_t size) {
  FreeListNode *current = start_node;
  size = align_up(size);

  while (current) {
    // Allocate in this node's payload
    if (current->is_free() && current->get_payload_size() >= size) {
      const uint64_t payload_start_addr =
          reinterpret_cast<uintptr_t>(current + 1);

      // Check if the node can be split
      if (current->get_payload_size() > size + sizeof(FreeListNode)) {
        // New node should be placed right after the payload
        FreeListNode *new_node =
            reinterpret_cast<FreeListNode *>(payload_start_addr + size);

        const uint64_t new_size =
            current->get_payload_size() - size - sizeof(FreeListNode);

        new_node->init(new_size, current->get_next());

        current->set_size(size);
        current->set_next(new_node);
      }

      current->mark_as_used();
      return reinterpret_cast<void *>(payload_start_addr);
    }

    // Visit next node
    current = current->get_next();
  }

  return nullptr;
}

void internal_heap_free(void *ptr) {
  FreeListNode *node = reinterpret_cast<FreeListNode *>(ptr) - 1;
  node->mark_as_free();

  // Fill freed memory
  uint64_t *payload = reinterpret_cast<uint64_t *>(ptr);

  for (uint64_t i = 0; i < node->get_payload_size() / sizeof(uint64_t); i++) {
    payload[i] = FREED_MEMORY_FILL;
  }
}
} // namespace kernel::memory