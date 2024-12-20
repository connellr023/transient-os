#include "../../../include/kernel/scheduler/cpu_scheduler.hpp"
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

bool enqueue(ThreadControlBlock *tcb) { return primary_queue.enqueue(tcb); }

bool wake() { return false; }

bool sleep() { return false; }
} // namespace kernel::scheduler