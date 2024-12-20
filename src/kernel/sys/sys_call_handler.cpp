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

#include "../../../include/kernel/sys/sys_call_handler.hpp"
#include "../../../include/kernel/kernel.hpp"
#include "../../../include/kernel/memory/heap.hpp"
#include "../../../include/kernel/memory/paging.hpp"

namespace kernel::sys {
void *handle_system_call(SystemCall call_code, const void *arg) {
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
    memory::internal_page_free(const_cast<void *>(arg));
    break;
  }
  case SystemCall::HeapAlloc: {
    return memory::internal_heap_alloc(reinterpret_cast<uintptr_t>(arg));
  }
  case SystemCall::HeapFree: {
    memory::internal_heap_free(const_cast<void *>(arg));
    break;
  }
  case SystemCall::Yield:
    // Do nothing
  default:
    break;
  }

  return nullptr;
}
} // namespace kernel::sys