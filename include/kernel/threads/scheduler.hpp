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

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "thread_control_block.hpp"

#define SCHEDULER_CAPACITY 250

namespace kernel::threads {
/**
 * @brief A simple queue for the scheduler.
 */
class SchedulerQueue {
private:
  ThreadControlBlock *queue[SCHEDULER_CAPACITY]{nullptr};

  uint32_t head;
  uint32_t tail;
  uint32_t size;
  uint32_t completed;

public:
  SchedulerQueue() : head(0), tail(0), size(0), completed(0){};

  /**
   * @brief Enqueues a thread in the scheduler queue.
   * @param tcb The thread to enqueue.
   * @return True if the thread was enqueued, false if the queue is full.
   */
  bool enqueue(ThreadControlBlock *tcb);

  /**
   * @brief Marks the current thread as complete and increments the completed
   * count.
   */
  void mark_current_as_complete();

  /**
   * @brief Switches this scheduler to the next thread.
   */
  void next();

  bool is_empty() const { return size == 0 || completed == size; }

  /**
   * @brief Peeks the thread currently at the head of the queue.
   * @return A pointer to the thread control block.
   */
  ThreadControlBlock *peek() { return queue[head]; }
};
} // namespace kernel::threads

#endif // SCHEDULER_HPP