#include "../../../include/kernel/scheduler/scheduler_queue.hpp"

bool SchedulerQueue::enqueue(ThreadControlBlock *tcb) {
  if (this->size >= QUEUE_CAPACITY) {
    return false;
  }

  this->queue[this->tail] = tcb;
  this->tail = (this->tail + 1) % QUEUE_CAPACITY;
  this->size++;

  return true;
}

ThreadControlBlock *dequeue() { return nullptr; }

void SchedulerQueue::next() {
  if (this->size == 0) {
    return;
  }

  do {
    this->head = (this->head + 1) % this->size;
  } while (this->peek()->is_complete());
}