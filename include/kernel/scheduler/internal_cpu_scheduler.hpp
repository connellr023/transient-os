#ifndef INTERNAL_CPU_SCHEDULER_HPP
#define INTERNAL_CPU_SCHEDULER_HPP

#include "../thread/thread_control_block.hpp"

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
 * @brief Special handling for context switches when the current thread is
 * exiting.
 * @return The thread control block of the next thread to run.
 */
const ThreadControlBlock *internal_exit_context_switch();

/**
 * ### (INTERNAL)
 * @brief Marks the current thread as sleeping.
 * @param sleep_us The number of microseconds to sleep.
 */
void internal_sleep(uint32_t sleep_us);
} // namespace kernel::scheduler

#endif // INTERNAL_CPU_SCHEDULER_HPP