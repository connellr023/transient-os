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
 * @brief Moves the first thread in the sleeping queue to the primary queue
 if
 * it is time.
 * @return True if a thread was moved, false otherwise.
 */
bool wake();

/**
 * @brief Moves the first thread in the primary queue to the sleeping
 queue.
 * @return True if a thread was moved, false otherwise.
 */
bool sleep();
} // namespace kernel::scheduler

#endif // CPU_SCHEDULER_HPP