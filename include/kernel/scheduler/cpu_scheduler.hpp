#ifndef CPU_SCHEDULER_HPP
#define CPU_SCHEDULER_HPP

#include "../tcb/thread_control_block.hpp"

namespace kernel::scheduler {
/**
 * @brief Enqueues a thread in the primary scheduler queue.
 * @param tcb The thread control block to enqueue.
 * @return True if the thread was enqueued, false if the queue is full.
 */
bool enqueue(ThreadControlBlock *tcb);

/**
 * @brief Gets the current thread's ID.
 * @return The thread ID.
 */
uint64_t get_thread_id();
} // namespace kernel::scheduler

#endif // CPU_SCHEDULER_HPP