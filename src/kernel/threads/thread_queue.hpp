#ifndef THREAD_QUEUE_HPP
#define THREAD_QUEUE_HPP

#include "thread_control_block.hpp"
#include <stdint.h>

namespace kernel::threads {
constexpr uint32_t thread_queue_capacity = 5;

struct ThreadQueue {
private:
  ThreadControlBlock queue[thread_queue_capacity];
  uint32_t head = 0;
  uint32_t tail = 0;
  uint32_t size = 0;

public:
  bool enqueue(ThreadControlBlock tcb);
  bool remove(uint64_t thread_id);

  const ThreadControlBlock &peek() const { return queue[head]; }
  uint32_t get_size() const { return size; }
};
} // namespace kernel::threads

#endif // THREAD_QUEUE_HPP