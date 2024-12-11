#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#define CPU_CTX_STACK_SIZE (16 * 16)

#ifndef __ASSEMBLER__

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
  ThreadState state;
  uint64_t sp;
  uint64_t pc;
  uint64_t spsr_el1;

public:
  ThreadControlBlock(uint64_t initial_sp, uint64_t initial_pc, uint64_t lr);
  ThreadControlBlock()
      : thread_id(0), page(0), state(ThreadState::Blocked), sp(0), pc(0),
        spsr_el1(0) {}

  void save_ctx(uint64_t sp);
  uint64_t restore_ctx() const;

  uint64_t get_sp() const { return sp; }
  uint64_t get_pc() const { return pc; }
  uint64_t get_spsr_el1() const { return spsr_el1; }
};
} // namespace kernel::threads

#endif // __ASSEMBLER__

#endif // THREAD_CONTROL_BLOCK_HPP