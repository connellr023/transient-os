#include "thread_control_block.hpp"
#include "../kernel.hpp"
#include "../memory/paging.hpp"

using namespace kernel::threads;

ThreadControlBlock::ThreadControlBlock(thread_handler_t handler,
                                       uint32_t burst_time, void *arg)
    : thread_id(0), page_addr(0), burst_time(burst_time),
      state(ThreadState::Ready), sp(0), handler(handler), arg(arg),
      is_stack_initialized(false) {
  static uint64_t thread_id_counter = 0;
  this->thread_id = thread_id_counter++;
}

void ThreadControlBlock::init_stack(void *page) {
  if (this->is_stack_initialized) {
    return;
  }

  this->sp = this->page_addr = reinterpret_cast<uint64_t>(page);

  // Initialize the thread stack
  uint64_t *register_stack = reinterpret_cast<uint64_t *>(this->sp);

  // Initialize x1 to x29 (FP) to 0
  for (int i = 1; i < LR_IDX; ++i) {
    register_stack[i] = 0;
  }

  // Set thread argument
  register_stack[0] = reinterpret_cast<uint64_t>(this->arg);

  // Set the link register to the thread return handler of the kernel
  register_stack[LR_IDX] = reinterpret_cast<uint64_t>(&thread_return_handler);

  // Set ELR_EL1 to the thread handler
  register_stack[ELR_EL1_IDX] = reinterpret_cast<uint64_t>(this->handler);

  // Set SPSR_EL1
  register_stack[SPSR_EL1_IDX] = INITIAL_SPSR_EL1;

  this->is_stack_initialized = true;
}