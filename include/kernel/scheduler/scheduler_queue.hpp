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

#ifndef SCHEDULER_QUEUE_HPP
#define SCHEDULER_QUEUE_HPP

#include "../tcb/thread_control_block.hpp"

#define QUEUE_CAPACITY 250

/**
 * @brief A simple queue for the scheduler.
 */
class SchedulerQueue {
private:
  ThreadControlBlock *queue[QUEUE_CAPACITY]{nullptr};

  uint32_t head = 0;
  uint32_t current = 0;
  uint32_t tail = 0;
  uint32_t size = 0;

public:
  SchedulerQueue() = default;

  /**
   * @brief Enqueues a thread in the scheduler queue.
   * @param tcb The thread to enqueue.
   * @return True if the thread was enqueued, false if the queue is full.
   */
  bool enqueue(ThreadControlBlock *tcb);

  /**
   * @brief Removes the thread at the head of the queue.
   * @return A pointer to the thread control block that was dequeued. Will be
   * nullptr if the queue is empty.
   */
  ThreadControlBlock *dequeue();

  /**
   * @brief This will move the head of the queue to the next ready thread.
   */
  void next();

  /**
   * @brief Predicate to check if the queue is empty.
   * @return True if the queue is empty, false otherwise.
   */
  bool is_empty() const { return size == 0; }

  /**
   * @brief Predicate to check if the queue is full.
   * @return True if the queue is full, false otherwise.
   */
  bool is_full() const { return size >= QUEUE_CAPACITY - 1; }

  /**
   * @brief Peeks the thread currently at the head of the queue.
   * @return A pointer to the thread control block.
   */
  ThreadControlBlock *peek();
};

#endif // SCHEDULER_QUEUE_HPP