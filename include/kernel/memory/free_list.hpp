#ifndef FREE_LIST_HPP
#define FREE_LIST_HPP

#include <stdint.h>

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

#endif // FREE_LIST_HPP