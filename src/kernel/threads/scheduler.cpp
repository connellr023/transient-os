#include "scheduler.hpp"

using namespace kernel::threads;

bool SchedulerQueue::enqueue(ThreadControlBlock *tcb) {
  if (this->size >= MAX_CONCURRENT_THREADS) {
    return false;
  }

  this->queue[this->tail] = tcb;
  this->tail = (this->tail + 1) % MAX_CONCURRENT_THREADS;
  this->size++;

  return true;
}

void SchedulerQueue::mark_current_as_complete() {
  this->peek()->mark_as_complete();
  this->completed++;
}

void SchedulerQueue::next() {
  if (this->size == 0) {
    return;
  }

  do {
    this->head = (this->head + 1) % this->size;
  } while (this->peek()->is_complete());
}