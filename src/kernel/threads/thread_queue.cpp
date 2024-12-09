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

void ThreadQueue::next() {
  if (this->size == 0) {
    return;
  }

  this->head = (this->head + 1) % this->size;
}