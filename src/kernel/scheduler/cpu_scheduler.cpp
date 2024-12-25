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

#include "../../../include/kernel/scheduler/cpu_scheduler.hpp"
#include "../../../include/kernel/kernel.hpp"
#include "../../../include/kernel/memory/internal_paging.hpp"
#include "../../../include/kernel/peripherals/timer.hpp"
#include "../../../include/kernel/scheduler/internal_cpu_scheduler.hpp"
#include "../../../include/kernel/scheduler/scheduler_queue.hpp"

namespace kernel::scheduler {
/**
 * @brief Primary queue of thread control blocks.
 */
SchedulerQueue primary_queue;

const ThreadControlBlock *get_current_thread() { return primary_queue.peek(); }

bool enqueue(ThreadControlBlock *tcb) {
  // Threads should have their stack allocated before being enqueued.
  if (!tcb->is_allocated()) {
    return false;
  }

  return primary_queue.enqueue(tcb);
}

/**
 * @brief Linearly scans the primary queue to find the next ready thread.
 */
ThreadControlBlock *find_next_thread() {
  ThreadControlBlock *next = nullptr;

  do {
    next = primary_queue.next();

    if (next->is_ready()) {
      break;
    } else if (next->is_sleeping()) {
      // Check if thread should be woken up
      if (next->get_wake_time() < *TIMER_COUNTER_LOW) {
        next->mark_as_ready();
        break;
      }
    } else {
      panic("Thread in invalid state");
    }
  } while (true); // Loop until a ready thread is found

  return next;
}

const ThreadControlBlock *internal_context_switch(void *interrupted_sp) {
  // Flag to indicate if the first context switch has occurred.
  static bool is_first_context_switch = true;

  if (primary_queue.is_empty()) {
    panic("No threads to schedule");
  }

  if (is_first_context_switch) {
    // Set flag to false after first context switch
    // All threads at this point are in the ready state
    is_first_context_switch = false;

    // Next thread for first context switch
    ThreadControlBlock *next = primary_queue.peek();
    next->mark_as_running();

    return next;
  }

  ThreadControlBlock *current = primary_queue.peek();

  // Save context of interrupted thread
  current->set_sp(interrupted_sp);

  // Prevent states such as sleeping from being removed
  if (current->is_running()) {
    current->mark_as_ready();
  }

  // Goto next thread
  ThreadControlBlock *next = find_next_thread();
  next->mark_as_running();

  return next;
}

const ThreadControlBlock *internal_exit_context_switch() {
  ThreadControlBlock *tcb = primary_queue.dequeue();

  // Free thread resources
  memory::internal_page_free(tcb->get_page());
  tcb->mark_as_complete();

  if (primary_queue.is_empty()) {
    panic("No threads to schedule after exit");
  }

  // Next thread is the first thread in the queue after a thread is dequeued
  ThreadControlBlock *next_tcb = primary_queue.peek();

  // Find the next ready thread if the first thread is not ready
  if (!next_tcb->is_ready()) {
    next_tcb = find_next_thread();
  }

  next_tcb->mark_as_running();
  return next_tcb;
}

void internal_sleep(uint32_t sleep_us) {
  primary_queue.peek()->mark_as_sleeping(*TIMER_COUNTER_LOW + sleep_us);
}
} // namespace kernel::scheduler