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

#include <api/sys/sys_calls.hpp>
#include <kernel/memory/paging.hpp>
#include <kernel/thread/thread_allocator.hpp>

namespace kernel::thread {
void init_stack(void *sp, void *arg, thread_handler_t handler) {
  uint64_t *register_stack = reinterpret_cast<uint64_t *>(sp);

  // Set x1 to x29 (FP) equal to 0
  for (int i = 1; i <= FP_IDX; i++) {
    register_stack[i] = 0;
  }

  // Set argument to x0
  register_stack[0] = reinterpret_cast<uintptr_t>(arg);

  // Set x30 (LR) to the exit system call
  register_stack[LR_IDX] = reinterpret_cast<uintptr_t>(&api::sys::exit);

  // Set ELR_EL1 to the thread handler
  register_stack[ELR_EL1_IDX] = reinterpret_cast<uintptr_t>(handler);

  // Set SPSR_EL1 to the initial value
  register_stack[SPSR_EL1_IDX] = static_cast<uint64_t>(PSRExceptionLevel::EL1t);
}

void init_heap(void *page) {
  FreeListNode *start_node = reinterpret_cast<FreeListNode *>(page);
  start_node->init(THREAD_HEAP_SIZE - sizeof(FreeListNode));
}

ThreadControlBlock *kernel_thread_alloc(thread_handler_t handler,
                                        uint32_t quantum_us, void *arg) {
  void *page = memory::kernel_page_alloc();

  if (!page) {
    return nullptr;
  }

  const uint64_t page_addr = reinterpret_cast<uintptr_t>(page);
  uint64_t sp = page_addr + PAGE_SIZE - sizeof(ThreadControlBlock);

  // Thread control blocks will be placed just before the thread's
  // stack space
  ThreadControlBlock *tcb = reinterpret_cast<ThreadControlBlock *>(sp);

  // Start stack for thread just after the thread control block
  sp -= CPU_CTX_STACK_SIZE;

  init_stack(reinterpret_cast<void *>(sp), arg, handler);
  init_heap(page);

  tcb->init(quantum_us, page, reinterpret_cast<void *>(sp));
  return tcb;
}
} // namespace kernel::thread