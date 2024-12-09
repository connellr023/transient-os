#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#include <stdint.h>

namespace kernel::threads {
class CpuContext {
public:
  uint64_t x0 = 0;
  uint64_t x1 = 0;
  uint64_t x2 = 0;
  uint64_t x3 = 0;
  uint64_t x4 = 0;
  uint64_t x5 = 0;
  uint64_t x6 = 0;
  uint64_t x7 = 0;
  uint64_t x8 = 0;
  uint64_t x9 = 0;
  uint64_t x10 = 0;
  uint64_t x11 = 0;
  uint64_t x12 = 0;
  uint64_t x13 = 0;
  uint64_t x14 = 0;
  uint64_t x15 = 0;
  uint64_t x16 = 0;
  uint64_t x17 = 0;
  uint64_t x18 = 0;
  uint64_t x19 = 0;
  uint64_t x20 = 0;
  uint64_t x21 = 0;
  uint64_t x22 = 0;
  uint64_t x23 = 0;
  uint64_t x24 = 0;
  uint64_t x25 = 0;
  uint64_t x26 = 0;
  uint64_t x27 = 0;
  uint64_t x28 = 0;
  uint64_t x29 = 0; // Frame Pointer (FP)
  uint64_t x30 = 0; // Link Register (LR)
  uint64_t sp = 0;
  uint64_t elr_el1 = 0;
  uint64_t spsr_el1 = 0;

  CpuContext() = default;

  void save(uint64_t *base);
  void restore(uint64_t *base) const;

  void init_thread_stack();
};

enum class ThreadState {
  Ready,
  Running,
  Blocked,
};

class ThreadControlBlock {
private:
  uint64_t thread_id;
  ThreadState state;
  CpuContext cpu_ctx{};

public:
  ThreadControlBlock() : thread_id(0), state(ThreadState::Blocked){};
  ThreadControlBlock(uint64_t sp, uint64_t elr_el1);

  uint64_t get_thread_id() const { return thread_id; }
  ThreadState get_state() const { return state; }
  CpuContext &get_cpu_ctx() { return cpu_ctx; }
};
} // namespace kernel::threads

#endif // THREAD_CONTROL_BLOCK_HPP