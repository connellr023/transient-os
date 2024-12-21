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

/**
 * @brief Queue of sleeping threads.
 */
SchedulerQueue sleeping_queue;

/**
 * @brief Flag to indicate if the first context switch has occurred. Needed to
 * avoid stack corruption on first switch.
 */
bool is_first_context_switch = true;

uint64_t get_thread_id() { return primary_queue.peek()->get_thread_id(); }

bool enqueue(ThreadControlBlock *tcb) {
  // For now, only allow enqueuing if the kernel has not started.
  // Threads should also have their stack allocated before being enqueued.
  if (is_started() || !tcb->is_allocated()) {
    return false;
  }

  return primary_queue.enqueue(tcb);
}

const ThreadControlBlock *internal_context_switch(void *interrupted_sp) {
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