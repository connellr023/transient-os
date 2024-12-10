#include "thread_control_block.hpp"
#include "../../drivers/uart0.hpp"
#include "../../utils/mem_utils.hpp"

using namespace kernel::threads;

ThreadControlBlock::ThreadControlBlock(uint64_t initial_sp, uint64_t initial_pc)
    : cpu_ctx(initial_sp, initial_pc) {
  static uint64_t thread_id_counter = 0;

  this->thread_id = thread_id_counter++;
  this->state = ThreadState::Ready;

  // Set SPSR_EL1 to be the same as the kernel's SPSR_EL1
  uint64_t spsr_el1;
  asm volatile("mrs %0, spsr_el1" : "=r"(spsr_el1));

  this->cpu_ctx.set_spsr_el1(spsr_el1);
}

void CpuContext::init_thread_stack(thread_return_handler_t lr) {
  uint64_t *stack = reinterpret_cast<uint64_t *>(this->sp);

  // Initialize general purpose registers to 0
  for (int i = 0; i < 31; ++i) {
    stack[i] = 0;
  }

  stack[30] = reinterpret_cast<uint64_t>(lr);

  // Set the stack pointer to the top of the initialized stack
  this->sp -= CPU_CTX_STACK_SIZE;
  this->sp = mem_align_16(this->sp);

  uart0::puts("Thread stack initialized.");
}

void CpuContext::save(uint64_t sp) {
  asm volatile("mrs %0, elr_el1" : "=r"(this->pc));
  asm volatile("mrs %0, spsr_el1" : "=r"(this->spsr_el1));

  this->sp = sp;
}

uint64_t CpuContext::restore() const {
  asm volatile("msr elr_el1, %0" : : "r"(this->pc));
  asm volatile("msr spsr_el1, %0" : : "r"(this->spsr_el1));

  return this->sp;
}