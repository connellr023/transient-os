/*
 * Copyright (C) 2024/2025 connellr023@github
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "../../../include/kernel/threads/thread_control_block.hpp"
#include "../../../include/kernel/memory/paging.hpp"

volatile uint64_t thread_id_counter = 0;

void ThreadControlBlock::init(thread_handler_t handler, uint32_t quantum_us,
                              void *arg) {
  this->thread_id = thread_id_counter++;
  this->page_addr = 0;
  this->quantum_us = quantum_us;
  this->sleep_until_us = 0;
  this->state = ThreadState::Uninitialized;
  this->sp = 0;
  this->handler = handler;
  this->arg = arg;
}

// bool alloc_thread_stack(ThreadControlBlock *tcb) {
//   if (tcb->is_allocated()) {
//     return false;
//   }

//   void *page = memory::internal_page_alloc();

//   if (!page) {
//     return false;
//   }

//   const uint64_t page_addr = reinterpret_cast<uint64_t>(page);
//   const uint64_t sp = page_addr + THREAD_STACK_SIZE - CPU_CTX_STACK_SIZE;

//   uint64_t *register_stack = reinterpret_cast<uint64_t *>(sp);

//   // Set x1 to x29 (FP) equal to 0
//   for (int i = 1; i < LR_IDX; i++) {
//     register_stack[i] = 0;
//   }

//   // Set argument to x0
//   register_stack[0] = reinterpret_cast<uint64_t>(tcb->get_arg());

//   // Set x30 (LR) to the exit system call
//   register_stack[LR_IDX] = reinterpret_cast<uint64_t>(&sys::exit);

//   // Set ELR_EL1 to the thread handler
//   register_stack[ELR_EL1_IDX] =
//   reinterpret_cast<uint64_t>(tcb->get_handler());

//   // Set SPSR_EL1 to the initial value
//   register_stack[SPSR_EL1_IDX] = INITIAL_SPSR_EL1_VALUE;

//   tcb->set_page(page);
//   tcb->set_sp(reinterpret_cast<void *>(sp));
//   tcb->mark_as_ready();

//   return true;
// }