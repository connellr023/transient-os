#include "thread_control_block.hpp"

using namespace kernel::threads;

ThreadControlBlock::ThreadControlBlock(uint64_t initial_sp, uint64_t initial_pc,
                                       uint64_t lr)
    : sp(initial_sp), pc(initial_pc), spsr_el1(0) {
  static uint64_t thread_id_counter = 0;

  this->thread_id = thread_id_counter++;
  this->state = ThreadState::Ready;

  // Set SPSR_EL1 to be the same as the kernel's SPSR_EL1
  uint64_t spsr_el1;
  asm volatile("mrs %0, spsr_el1" : "=r"(spsr_el1));

  this->spsr_el1 = spsr_el1;

  // Initialize the thread stack
  uint64_t *register_stack = reinterpret_cast<uint64_t *>(initial_sp);

  // Initialize general-purpose registers to 0
  for (int i = 0; i < 31; ++i) {
    register_stack[i] = 0;
  }

  // Set the link register
  register_stack[30] = lr;
}

void ThreadControlBlock::save_ctx(uint64_t sp) {
  asm volatile("mrs %0, elr_el1" : "=r"(this->pc));
  // asm volatile("mrs %0, spsr_el1" : "=r"(this->spsr_el1));

  this->sp = sp;
}

uint64_t ThreadControlBlock::restore_ctx() const {
  asm volatile("msr elr_el1, %0" : : "r"(this->pc));
  // asm volatile("msr spsr_el1, %0" : : "r"(this->spsr_el1));

  return this->sp;
}