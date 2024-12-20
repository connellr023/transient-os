#ifndef INTERNAL_CPU_SCHEDULER_HPP
#define INTERNAL_CPU_SCHEDULER_HPP

#include "../tcb/thread_control_block.hpp"

namespace kernel::scheduler {
/**
 * ### (INTERNAL)
 * @brief Handles a context switch by updating the current thread's TCB and
 * scheduling the next thread. This function should never be called directly.
 * @param interrupted_sp The stack pointer of the interrupted thread.
 * @return The thread control block of the next thread to run.
 */
const ThreadControlBlock *internal_context_switch(void *interrupted_sp);

/**
 * ### (INTERNAL)
 * @brief Frees the resources of the current thread and dequeues it from the
 * primary queue.
 */
void internal_thread_free();

/**
 * ### (INTERNAL)
 * @brief Moves the first thread in the sleeping queue to the primary queue
 if it is time.
 * @return True if a thread was moved, false otherwise.
 */
bool internal_wake();

/**
 * ### (INTERNAL)
 * @brief Moves the first thread in the primary queue to the sleeping queue.
 * @return True if a thread was moved, false otherwise.
 */
bool internal_sleep();
} // namespace kernel::scheduler

#endif // INTERNAL_CPU_SCHEDULER_HPP