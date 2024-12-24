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

#ifndef FREE_LIST_HPP
#define FREE_LIST_HPP

#include <stdint.h>

/**
 * @brief A node in the heap allocator's free list.
 */
class FreeListNode {
private:
  bool is_free_node;
  uint64_t payload_size;
  FreeListNode *next;

public:
  void init(uint64_t size, FreeListNode *next = nullptr) {
    this->is_free_node = true;
    this->payload_size = size;
    this->next = next;
  }

  bool is_free() const { return is_free_node; }

  uint64_t get_payload_size() const { return payload_size; }
  FreeListNode *get_next() const { return next; }

  void set_size(uint64_t size) { this->payload_size = size; }
  void set_next(FreeListNode *next) { this->next = next; }

  void mark_as_used() { this->is_free_node = false; }
  void mark_as_free() { this->is_free_node = true; }
};

#endif // FREE_LIST_HPP