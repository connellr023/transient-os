#include "thread_control_block.hpp"
#include "../kernel.hpp"
#include "../memory/paging.hpp"
#include "../sys_registers.hpp"

using namespace kernel::threads;

ThreadControlBlock::ThreadControlBlock(thread_handler_t handler,
                                       uint32_t burst_time, void *arg)
    : thread_id(0), page_addr(0), burst_time(burst_time),
      state(ThreadState::Ready), sp(0), handler(handler), arg(arg),
      is_stack_initialized(false), is_complete(false) {
  static uint64_t thread_id_counter = 0;
  this->thread_id = thread_id_counter++;
}

void ThreadControlBlock::init_stack(void *page) {
  if (this->is_stack_initialized) {
    return;
  }

  this->page_addr = reinterpret_cast<uint64_t>(page);
  this->sp = this->page_addr - CPU_CTX_STACK_SIZE;

  // Initialize the thread stack
  uint64_t *register_stack = reinterpret_cast<uint64_t *>(this->sp);

  // Set x1 to x29 (FP) equal to 0
  for (int i = 1; i < LR_IDX; i++) {
    register_stack[i] = 0;
  }

  // Set argument to x0
  register_stack[0] = reinterpret_cast<uint64_t>(this->arg);

  // Set x30 (LR) to the thread return handler
  register_stack[LR_IDX] = reinterpret_cast<uint64_t>(&thread_return_handler);

  // Set ELR_EL1 to the thread handler
  register_stack[ELR_EL1_IDX] = reinterpret_cast<uint64_t>(this->handler);

  // Set SPSR_EL1 to the initial value
  register_stack[SPSR_EL1_IDX] = INITIAL_SPSR_EL1_VALUE;

  this->is_stack_initialized = true;
}