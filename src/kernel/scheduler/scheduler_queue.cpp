/*
 * Copyright (C) 2024/2025 connellr023@github
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "../../../include/kernel/scheduler/scheduler_queue.hpp"

bool SchedulerQueue::enqueue(ThreadControlBlock *tcb) {
  if (this->is_full()) {
    return false;
  }

  this->queue[this->tail] = tcb;
  this->tail = (this->tail + 1) % QUEUE_CAPACITY;
  this->size++;

  if (this->size == 1) {
    this->current = this->head;
  }

  return true;
}

ThreadControlBlock *SchedulerQueue::dequeue() {
  if (this->is_empty()) {
    return nullptr;
  }

  ThreadControlBlock *tcb = this->peek();
  const uint32_t new_head = (this->head + 1) % QUEUE_CAPACITY;

  // Keep current within bounds.
  if (this->current == this->head) {
    this->current = new_head;
  }

  this->head = new_head;
  this->size--;

  return tcb;
}

ThreadControlBlock *SchedulerQueue::peek() {
  if (this->is_empty()) {
    return nullptr;
  }

  return this->queue[this->current];
}

void SchedulerQueue::next() {
  if (this->is_empty()) {
    return;
  }

  do {
    this->current = (this->current + 1) % QUEUE_CAPACITY;

    if (this->current == this->tail) {
      this->current = this->head;
    }
  } while (!this->queue[this->current]->is_ready());
}