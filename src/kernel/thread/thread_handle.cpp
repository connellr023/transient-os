#include <kernel/sys/sys_calls.hpp>
#include <kernel/thread/thread_handle.hpp>

void ThreadHandle::join() {
  while (!this->tcb->is_complete()) {
    kernel::sys::yield();
  }
}