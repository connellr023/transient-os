#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "thread_control_block.hpp"
#include <stdint.h>

#define MAX_CONCURRENT_THREADS 100

namespace kernel::threads {
class SchedulerQueue {
private:
  ThreadControlBlock *queue[MAX_CONCURRENT_THREADS]{nullptr};

  uint32_t head;
  uint32_t tail;
  uint32_t size;
  uint32_t completed;

public:
  SchedulerQueue() : head(0), tail(0), size(0), completed(0){};

  bool enqueue(ThreadControlBlock *tcb);
  void mark_current_as_complete();
  void next();

  bool is_empty() const { return size == 0 || completed == size; }

  ThreadControlBlock *peek() { return queue[head]; }
};
} // namespace kernel::threads

#endif // SCHEDULER_HPP