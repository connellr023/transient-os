#include "thread_control_block.hpp"

using namespace kernel::threads;

ThreadControlBlock::ThreadControlBlock(uint64_t sp, uint64_t elr_el1) {
  static uint64_t thread_id_counter = 0;

  this->thread_id = thread_id_counter++;
  this->state = ThreadState::Ready;

  // Set SPSR_EL1 to be the same as the kernel's SPSR_EL1
  uint64_t spsr_el1;
  asm volatile("mrs %0, spsr_el1" : "=r"(spsr_el1));

  this->cpu_ctx = CpuContext(sp, elr_el1, spsr_el1);
}

void CpuContext::init_thread_stack() {
  uint64_t *stack = reinterpret_cast<uint64_t *>(this->sp);

  stack[0] = 0;  // X0
  stack[1] = 0;  // X1
  stack[2] = 0;  // X2
  stack[3] = 0;  // X3
  stack[4] = 0;  // X4
  stack[5] = 0;  // X5
  stack[6] = 0;  // X6
  stack[7] = 0;  // X7
  stack[8] = 0;  // X8
  stack[9] = 0;  // X9
  stack[10] = 0; // X10
  stack[11] = 0; // X11
  stack[12] = 0; // X12
  stack[13] = 0; // X13
  stack[14] = 0; // X14
  stack[15] = 0; // X15
  stack[16] = 0; // X16
  stack[17] = 0; // X17
  stack[18] = 0; // X18
  stack[19] = 0; // X19
  stack[20] = 0; // X20
  stack[21] = 0; // X21
  stack[22] = 0; // X22
  stack[23] = 0; // X23
  stack[24] = 0; // X24
  stack[25] = 0; // X25
  stack[26] = 0; // X26
  stack[27] = 0; // X27
  stack[28] = 0; // X28
  stack[29] = 0; // X29
  stack[30] = 0; // X30

  this->sp -= CPU_CTX_STACK_SIZE;
}

void CpuContext::save(uint64_t sp) {
  asm volatile("mrs %0, elr_el1" : "=r"(this->elr_el1));
  asm volatile("mrs %0, spsr_el1" : "=r"(this->spsr_el1));

  this->sp = sp;
}

uint64_t CpuContext::restore() const {
  asm volatile("msr elr_el1, %0" : : "r"(this->elr_el1));
  asm volatile("msr spsr_el1, %0" : : "r"(this->spsr_el1));

  return this->sp;
}