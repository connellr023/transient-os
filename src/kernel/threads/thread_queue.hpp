#ifndef THREAD_QUEUE_HPP
#define THREAD_QUEUE_HPP

#include "thread_control_block.hpp"
#include <stdint.h>

namespace kernel::threads {
constexpr uint32_t thread_queue_capacity = 5;

struct ThreadQueue {
private:
  ThreadControlBlock queue[thread_queue_capacity]{};
  uint32_t head;
  uint32_t tail;
  uint32_t size;

public:
  ThreadQueue() : head(0), tail(0), size(0){};

  bool enqueue(ThreadControlBlock tcb);
  void next();

  ThreadControlBlock peek() { return queue[head]; }
  uint32_t get_size() const { return size; }
};
} // namespace kernel::threads

#endif // THREAD_QUEUE_HPP