#include "thread_control_block.hpp"
#include "../memory/memory.hpp"

using namespace kernel::threads;

ThreadControlBlock::ThreadControlBlock(thread_handler_t handler,
                                       uint32_t burst_time)
    : thread_id(0), page(0), burst_time(burst_time), state(ThreadState::Ready),
      sp(0), pc(reinterpret_cast<uint64_t>(handler)), spsr_el1(0),
      is_stack_initialized(false) {
  static uint64_t thread_id_counter = 0;
  this->thread_id = thread_id_counter++;

  // Set SPSR_EL1 to be the same as the kernel's SPSR_EL1
  uint64_t spsr_el1;
  asm volatile("mrs %0, spsr_el1" : "=r"(spsr_el1));

  this->spsr_el1 = spsr_el1;
}

void ThreadControlBlock::init_stack(uint64_t page) {
  if (this->is_stack_initialized) {
    return;
  }

  this->sp = this->page = page;

  // Initialize the thread stack
  uint64_t *register_stack = reinterpret_cast<uint64_t *>(this->sp);

  // Initialize general-purpose registers to 0
  for (int i = 0; i < 31; ++i) {
    register_stack[i] = 0;
  }

  // Set the link register to the thread return handler of the kernel
  register_stack[30] = reinterpret_cast<uint64_t>(&thread_return_handler);

  this->is_stack_initialized = true;
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