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
#include "../../../include/kernel/scheduler/internal_cpu_scheduler.hpp"
#include "../../../include/kernel/scheduler/scheduler_queue.hpp"

namespace kernel::scheduler {
/**
 * @brief Primary queue of threads.
 */
SchedulerQueue primary_queue;

const ThreadControlBlock *get_current_thread() { return primary_queue.peek(); }

bool enqueue(ThreadControlBlock *tcb) {
  // For now, only allow enqueuing if the kernel has not started.
  // Threads should also have their stack allocated before being enqueued.
  if (is_started() || !tcb->is_allocated()) {
    return false;
  }

  return primary_queue.enqueue(tcb);
}

const ThreadControlBlock *internal_context_switch(void *interrupted_sp) {
  // Flag to indicate if the first context switch has occurred.
  static bool is_first_context_switch = true;

  if (primary_queue.is_empty()) {
    panic("No threads to schedule");
  }

  if (!is_first_context_switch) {
    // Save context of interrupted thread
    primary_queue.peek()->set_sp(interrupted_sp);
    primary_queue.peek()->mark_as_ready();

    // Goto next thread
    primary_queue.next();
  } else {
    // Set flag to false after first context switch
    is_first_context_switch = false;
  }

  primary_queue.peek()->mark_as_running();
  return primary_queue.peek();
}

void internal_thread_free() {
  memory::internal_page_free(primary_queue.peek()->get_page());
  primary_queue.dequeue();
}

bool internal_wake() { return false; }

bool internal_sleep() { return false; }
} // namespace kernel::scheduler