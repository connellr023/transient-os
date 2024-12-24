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

#include "../../../include/kernel/kernel.hpp"
#include "../../../include/kernel/memory/internal_heap.hpp"
#include "../../../include/kernel/memory/internal_paging.hpp"
#include "../../../include/kernel/scheduler/cpu_scheduler.hpp"
#include "../../../include/kernel/scheduler/internal_cpu_scheduler.hpp"
#include "../../../include/kernel/sys/internal_sys_call_handler.hpp"

namespace kernel::sys {
void *internal_handle_sys_call(SystemCall call_code, const void *arg) {
  switch (call_code) {
  case SystemCall::PutString: {
    const char *str = reinterpret_cast<const char *>(arg);
    safe_puts(str);
    break;
  }
  case SystemCall::PageAlloc: {
    return memory::internal_page_alloc();
  }
  case SystemCall::PageFree: {
    void *page = const_cast<void *>(arg);
    memory::internal_page_free(page);
    break;
  }
  case SystemCall::HeapAlloc: {
    const uint64_t size = reinterpret_cast<uintptr_t>(arg);
    return memory::internal_heap_alloc(
        scheduler::get_current_thread()->get_heap_start(), size);
  }
  case SystemCall::HeapFree: {
    void *ptr = const_cast<void *>(arg);
    memory::internal_heap_free(ptr);
    break;
  }
  case SystemCall::Sleep: {
    const uint32_t sleep_us = reinterpret_cast<uintptr_t>(arg);
    scheduler::internal_sleep(sleep_us);
    break;
  }
  default:
    // Yield system call does nothing so this case will be hit for it
    break;
  }

  return nullptr;
}
} // namespace kernel::sys