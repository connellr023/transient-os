#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#define CPU_CTX_STACK_SIZE (16 * 16)

#ifndef __ASSEMBLER__

#include <stdint.h>

namespace kernel::threads {
typedef void (*thread_return_handler_t)();

class CpuContext {
private:
  uint64_t sp;
  uint64_t pc;
  uint64_t spsr_el1;

public:
  CpuContext(uint64_t initial_sp, uint64_t initial_pc)
      : sp(initial_sp), pc(initial_pc), spsr_el1(0){};

  void save(uint64_t sp);
  uint64_t restore() const;

  void init_thread_stack(thread_return_handler_t lr);

  void set_spsr_el1(uint64_t spsr) { spsr_el1 = spsr; }

  uint64_t get_sp() const { return sp; }
  uint64_t get_pc() const { return pc; }
  uint64_t get_spsr_el1() const { return spsr_el1; }
};

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
  CpuContext cpu_ctx{0, 0};

public:
  ThreadControlBlock() : thread_id(0), page(0), state(ThreadState::Blocked){};
  ThreadControlBlock(uint64_t page, uint64_t initial_pc);

  uint64_t get_thread_id() const { return thread_id; }
  ThreadState get_state() const { return state; }
  CpuContext &get_cpu_ctx() { return cpu_ctx; }
};
} // namespace kernel::threads

#endif // __ASSEMBLER__

#endif // THREAD_CONTROL_BLOCK_HPP