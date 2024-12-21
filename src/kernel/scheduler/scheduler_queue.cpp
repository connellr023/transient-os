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

ThreadControlBlock *SchedulerQueue::dequeue() {
  if (this->is_empty()) {
    return nullptr;
  }

  ThreadControlBlock *tcb = this->peek();

  for (uint32_t i = 0; i < this->size - 1; i++) {
    this->queue[(this->head + i) % this->size] =
        this->queue[(this->head + i + 1) % this->size];
  }

  this->queue[this->tail] = nullptr;
  this->tail = (this->tail - 1) % QUEUE_CAPACITY;
  this->size--;

  return tcb;
}

void SchedulerQueue::next() {
  if (this->is_empty()) {
    return;
  }

  do {
    this->head = (this->head + 1) % this->size;
  } while (!this->peek()->is_ready());
}