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

#include <kernel/memory/heap.hpp>
#include <utils/mem_utils.hpp>

namespace kernel::memory {
void *kernel_heap_alloc(FreeListNode *start_node, uint64_t size) {
  FreeListNode *current = start_node;
  size = utils::align_up(size);

  while (current != nullptr) {
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

void kernel_heap_free(void *ptr) {
  FreeListNode *node = reinterpret_cast<FreeListNode *>(ptr) - 1;
  node->mark_as_free();

  // Fill freed memory
  uint64_t *payload = reinterpret_cast<uint64_t *>(ptr);

  for (uint64_t i = 0; i < node->get_payload_size() / sizeof(uint64_t); i++) {
    payload[i] = FREED_MEMORY_FILL;
  }
}

void *kernel_heap_realloc(FreeListNode *start_node, void *ptr,
                          uint64_t new_size) {
  new_size = utils::align_up(new_size);

  // Free old block if new size is 0
  if (new_size == 0) {
    kernel_heap_free(ptr);
    return nullptr;
  }

  FreeListNode *current_block_node = reinterpret_cast<FreeListNode *>(ptr) - 1;

  // If new size fits within the current block, return the current block with
  // updated size
  if (current_block_node->get_payload_size() >= new_size) {
    current_block_node->set_size(new_size);
    return ptr;
  }

  // Allocate a new block with the new size
  void *new_block = kernel_heap_alloc(start_node, new_size);

  if (!new_block) {
    return nullptr;
  }

  // Copy the old block's data to the new block
  utils::memcpy(new_block, ptr, current_block_node->get_payload_size());

  // Free the old block
  kernel_heap_free(ptr);

  return new_block;
}
} // namespace kernel::memory