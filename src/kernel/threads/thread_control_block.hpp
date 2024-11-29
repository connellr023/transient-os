#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#include <stdint.h>

namespace kernel::threads {
struct GeneralPurposeRegisterStates {
public:
  uint64_t x0;
  uint64_t x1;
  uint64_t x2;
  uint64_t x3;
  uint64_t x4;
  uint64_t x5;
  uint64_t x6;
  uint64_t x7;
  uint64_t x8;
  uint64_t x9;
  uint64_t x10;
  uint64_t x11;
  uint64_t x12;
  uint64_t x13;
  uint64_t x14;
  uint64_t x15;
  uint64_t x16;
  uint64_t x17;
  uint64_t x18;
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint64_t x24;
  uint64_t x25;
  uint64_t x26;
  uint64_t x27;
  uint64_t x28;
  uint64_t x29;
  uint64_t x30;

  void read_from_sp(uint64_t *base);
  void restore_to_sp(uint64_t *base);
};

struct SpecialPurposeRegisterStates {
public:
  uint64_t pc;
  uint64_t sp;
};

struct ThreadControlBlock {
private:
  uint64_t thread_id;
  SpecialPurposeRegisterStates sp_registers{};
  GeneralPurposeRegisterStates gp_registers{};

public:
  ThreadControlBlock();

  uint64_t get_thread_id() const { return thread_id; }

  SpecialPurposeRegisterStates &get_sp_registers() { return sp_registers; }
  GeneralPurposeRegisterStates &get_gp_registers() { return gp_registers; }
};
} // namespace kernel::threads

#endif // THREAD_CONTROL_BLOCK_HPP