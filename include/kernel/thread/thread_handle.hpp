#ifndef THREAD_HANDLE_HPP
#define THREAD_HANDLE_HPP

#include <kernel/thread/thread_control_block.hpp>
#include <stdint.h>

class ThreadHandle {
private:
  ThreadControlBlock *tcb;

public:
  void init(ThreadControlBlock *tcb) { this->tcb = tcb; }

  uint64_t get_thread_id() const { return this->tcb->get_thread_id(); }

  /**
   * @brief Blocks the current thread until the thread associated with this
   * handle completes.
   */
  void join();
};

#endif // THREAD_HANDLE_HPP