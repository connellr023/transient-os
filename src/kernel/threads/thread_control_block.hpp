#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#define CPU_CTX_STACK_SIZE (16 * 16)

#ifndef __ASSEMBLER__

#include "threads.hpp"
#include <stdint.h>

namespace kernel::threads {
enum class ThreadState {
  Ready,
  Running,
  Blocked,
};

class ThreadControlBlock {
private:
  uint64_t thread_id;
  uint64_t page;
  uint32_t burst_time;
  ThreadState state;
  uint64_t sp;
  uint64_t pc;
  uint64_t spsr_el1;
  bool is_stack_initialized;

public:
  ThreadControlBlock(thread_handler_t handler, uint32_t burst_time);
  ThreadControlBlock() : ThreadControlBlock(nullptr, 0) {}

  void init_stack(uint64_t page);

  void save_ctx(uint64_t sp);
  uint64_t restore_ctx() const;

  uint64_t get_thread_id() const { return thread_id; }
  uint64_t get_page() const { return page; }
  uint32_t get_burst_time() const { return burst_time; }
  uint64_t get_sp() const { return sp; }
  uint64_t get_pc() const { return pc; }
  uint64_t get_spsr_el1() const { return spsr_el1; }
};
} // namespace kernel::threads

#endif // __ASSEMBLER__

#endif // THREAD_CONTROL_BLOCK_HPP