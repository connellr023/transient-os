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
#include <kernel/kernel.hpp>
#include <kernel/memory/heap.hpp>
#include <kernel/scheduler/cpu_scheduler.hpp>
#include <kernel/sys/sys_call_handler.hpp>
#include <kernel/thread/thread_allocator.hpp>

namespace kernel::sys {
void *handle_sys_call(SystemCall call_code, const void *arg) {
  switch (call_code) {
  case SystemCall::SetOutputHandler: {
    output_handler_t handler =
        reinterpret_cast<output_handler_t>(const_cast<void *>(arg));
    set_output_handler(handler);
    break;
  }
  case SystemCall::PutString: {
    const char *str = reinterpret_cast<const char *>(arg);
    safe_puts(str);
    break;
  }
  case SystemCall::HeapAlloc: {
    const uint64_t size = reinterpret_cast<uintptr_t>(arg);
    return memory::kernel_heap_alloc(
        scheduler::get_current_thread()->get_heap_start(), size);
  }
  case SystemCall::HeapRealloc: {
    HeapReallocArgs *realloc_args =
        reinterpret_cast<HeapReallocArgs *>(const_cast<void *>(arg));

    return memory::kernel_heap_realloc(
        scheduler::get_current_thread()->get_heap_start(), realloc_args->ptr,
        realloc_args->new_size);
  }
  case SystemCall::HeapFree: {
    void *ptr = const_cast<void *>(arg);
    memory::kernel_heap_free(ptr);
    break;
  }
  case SystemCall::Sleep: {
    const uint32_t sleep_us = reinterpret_cast<uintptr_t>(arg);
    scheduler::sleep_current(sleep_us);
    break;
  }
  case SystemCall::SpawnThread: {
    AllocThreadArgs *alloc_thread_args =
        reinterpret_cast<AllocThreadArgs *>(const_cast<void *>(arg));

    ThreadControlBlock *tcb = thread::kernel_thread_alloc(
        alloc_thread_args->handler, alloc_thread_args->quantum_us,
        alloc_thread_args->arg);

    alloc_thread_args->handle->init(tcb);

    return reinterpret_cast<void *>(tcb != nullptr && scheduler::enqueue(tcb));
  }
  default:
    // Yield system call does nothing so this case will be hit for it
    break;
  }

  return nullptr;
}
} // namespace kernel::sys