#include "scheduler.hpp"

using namespace kernel::threads;

bool SchedulerQueue::enqueue(ThreadControlBlock *tcb) {
  if (this->size >= scheduler_queue_capacity) {
    return false;
  }

  this->queue[this->tail] = tcb;
  this->tail = (this->tail + 1) % scheduler_queue_capacity;
  this->size++;

  return true;
}

void SchedulerQueue::dequeue() {
  if (this->size == 0) {
    return;
  }

  this->queue[this->head] = nullptr;
  this->head = (this->head + 1) % this->size;
  this->size--;
}

void SchedulerQueue::next() {
  if (this->size == 0) {
    return;
  }

  this->head = (this->head + 1) % this->size;
}