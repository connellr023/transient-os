#include "thread_queue.hpp"

using namespace kernel::threads;

bool ThreadQueue::enqueue(ThreadControlBlock tcb) {
  if (this->size >= thread_queue_capacity) {
    return false;
  }

  this->queue[this->tail] = tcb;
  this->tail = (this->tail + 1) % thread_queue_capacity;
  this->size++;

  return true;
}

bool ThreadQueue::remove(uint64_t thread_id) {
  // Since thread IDs are sorted, we can use binary search
  uint32_t left = 0;
  uint32_t right = this->size - 1;

  while (left <= right) {
    uint32_t mid = left + (right - left) / 2;

    if (this->queue[mid].get_thread_id() == thread_id) {
      // Shift all elements to the left
      for (uint8_t j = mid; j < this->size - 1; j++) {
        this->queue[j] = this->queue[j + 1];
      }

      this->size--;
      return true;
    } else if (this->queue[mid].get_thread_id() < thread_id) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return false;
}