#include "../../../include/kernel/threads/scheduler.hpp"

bool SchedulerQueue::enqueue(ThreadControlBlock *tcb) {
  if (this->size >= SCHEDULER_CAPACITY) {
    return false;
  }

  this->queue[this->tail] = tcb;
  this->tail = (this->tail + 1) % SCHEDULER_CAPACITY;
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