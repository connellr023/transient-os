#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#include <stdint.h>

namespace kernel::threads {
typedef void (*thread_handler_t)(void *);

enum class ThreadState {
  Uninitialized,
  Ready,
  Running,
  Complete,
};

class ThreadControlBlock {
private:
  uint64_t thread_id;
  uint64_t page_addr;
  uint32_t burst_time;
  ThreadState state;
  uint64_t sp;
  thread_handler_t handler;
  void *arg;

public:
  ThreadControlBlock(thread_handler_t handler, uint32_t burst_time,
                     void *arg = nullptr);
  ThreadControlBlock() : ThreadControlBlock(nullptr, 0) {}

  thread_handler_t get_handler() const { return this->handler; }
  void *get_arg() const { return this->arg; }
  void *get_page() const { return reinterpret_cast<void *>(this->page_addr); }
  void *get_sp() const { return reinterpret_cast<void *>(this->sp); }
  uint64_t get_thread_id() const { return this->thread_id; }
  uint32_t get_burst_time() const { return this->burst_time; }

  bool is_complete() const { return this->state == ThreadState::Complete; }
  bool is_ready() const { return this->state == ThreadState::Ready; }
  bool is_running() const { return this->state == ThreadState::Running; }
  bool is_initialized() const {
    return this->state != ThreadState::Uninitialized;
  }

  void mark_as_ready() { this->state = ThreadState::Ready; }
  void mark_as_running() { this->state = ThreadState::Running; }
  void mark_as_complete() { this->state = ThreadState::Complete; }

  void set_page(void *page) {
    this->page_addr = reinterpret_cast<uint64_t>(page);
  }

  void set_sp(void *sp) { this->sp = reinterpret_cast<uint64_t>(sp); }
};
} // namespace kernel::threads

#endif // THREAD_CONTROL_BLOCK_HPP