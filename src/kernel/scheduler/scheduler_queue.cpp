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

#include <kernel/scheduler/scheduler_queue.hpp>

bool SchedulerQueue::enqueue(ThreadControlBlock *tcb) {
  if (this->is_full()) {
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

  ThreadControlBlock *tcb = this->queue[this->head];
  this->head = (this->head + 1) % QUEUE_CAPACITY;
  this->size--;

  return tcb;
}

ThreadControlBlock *SchedulerQueue::peek() {
  if (this->is_empty()) {
    return nullptr;
  }

  return this->queue[this->head];
}

ThreadControlBlock *SchedulerQueue::next() {
  if (this->is_empty()) {
    return nullptr;
  }

  // Remove the head
  ThreadControlBlock *tcb = this->queue[this->head];
  this->head = (this->head + 1) % QUEUE_CAPACITY;

  // Add the head to the tail
  this->queue[this->tail] = tcb;
  this->tail = (this->tail + 1) % QUEUE_CAPACITY;

  return this->queue[this->head];
}