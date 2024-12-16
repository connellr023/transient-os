#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#include <stdint.h>

namespace kernel::threads {
typedef void (*thread_handler_t)(void *);

enum class ThreadState {
  Ready,
  Running,
  Blocked,
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
  bool is_initialized;
  bool is_complete;

public:
  ThreadControlBlock(thread_handler_t handler, uint32_t burst_time,
                     void *arg = nullptr);
  ThreadControlBlock() : ThreadControlBlock(nullptr, 0) {}

  thread_handler_t get_handler() const { return handler; }
  void *get_arg() const { return arg; }
  void *get_page() const { return reinterpret_cast<void *>(page_addr); }
  void *get_sp() const { return reinterpret_cast<void *>(sp); }
  uint64_t get_thread_id() const { return thread_id; }
  uint32_t get_burst_time() const { return burst_time; }
  bool get_is_initialized() const { return is_initialized; }
  bool get_is_complete() const { return is_complete; }

  void set_page(void *page) {
    this->page_addr = reinterpret_cast<uint64_t>(page);
  }

  void set_sp(void *sp) { this->sp = reinterpret_cast<uint64_t>(sp); }
  void set_state(ThreadState state) { this->state = state; }

  void mark_initialized() { this->is_initialized = true; }
  void mark_complete() { this->is_complete = true; }
};
} // namespace kernel::threads

#endif // THREAD_CONTROL_BLOCK_HPP