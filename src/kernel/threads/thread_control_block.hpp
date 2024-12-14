#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#define CPU_CTX_STACK_SIZE (17 * 16) // 17 pairs of 8 byte registers

#define LR_IDX 30
#define ELR_EL1_IDX 32
#define SPSR_EL1_IDX 33

#define INITIAL_SPSR_EL1 0x00000344

#ifndef __ASSEMBLER__

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
  bool is_stack_initialized;
  bool is_complete;

public:
  ThreadControlBlock(thread_handler_t handler, uint32_t burst_time,
                     void *arg = nullptr);
  ThreadControlBlock() : ThreadControlBlock(nullptr, 0) {}

  void init_stack(void *page);

  void set_sp(void *sp) { this->sp = reinterpret_cast<uint64_t>(sp); }
  void *get_sp() const { return reinterpret_cast<void *>(sp); }

  void set_state(ThreadState state) { this->state = state; }
  void mark_complete() { this->is_complete = true; }

  void *get_page() const { return reinterpret_cast<void *>(page_addr); }
  uint64_t get_thread_id() const { return thread_id; }
  uint32_t get_burst_time() const { return burst_time; }
};
} // namespace kernel::threads

#endif // __ASSEMBLER__

#endif // THREAD_CONTROL_BLOCK_HPP