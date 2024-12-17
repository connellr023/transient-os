#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "thread_control_block.hpp"
#include <stdint.h>

namespace kernel::threads {
template <int queue_capacity> class SchedulerQueue {
private:
  ThreadControlBlock *queue[queue_capacity]{nullptr};

  uint32_t head;
  uint32_t tail;
  uint32_t size;
  uint32_t completed;

public:
  SchedulerQueue() : head(0), tail(0), size(0), completed(0){};

  bool enqueue(ThreadControlBlock *tcb) {
    if (this->size >= queue_capacity) {
      return false;
    }

    this->queue[this->tail] = tcb;
    this->tail = (this->tail + 1) % queue_capacity;
    this->size++;

    return true;
  }

  void mark_current_as_complete() {
    this->peek()->mark_as_complete();
    this->completed++;
  }

  void next() {
    if (this->size == 0) {
      return;
    }

    do {
      this->head = (this->head + 1) % this->size;
    } while (this->peek()->is_complete());
  }

  bool is_empty() const { return size == 0 || completed == size; }

  ThreadControlBlock *peek() { return queue[head]; }
};
} // namespace kernel::threads

#endif // SCHEDULER_HPP