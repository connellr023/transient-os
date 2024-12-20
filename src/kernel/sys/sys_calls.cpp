#include "../../../include/kernel/sys/sys_calls.hpp"

namespace kernel::sys {
void puts(const char *str) { trigger_sys_call(SystemCall::PutString, str); }

void *page_alloc() { return trigger_sys_call(SystemCall::PageAlloc); }

void page_free(void *page) { trigger_sys_call(SystemCall::PageFree, page); }

void *heap_alloc(uint64_t size) {
  return trigger_sys_call(SystemCall::HeapAlloc,
                          reinterpret_cast<void *>(size));
}

void heap_free(void *ptr) { trigger_sys_call(SystemCall::HeapFree, ptr); }

void yield() { trigger_sys_call(SystemCall::Yield); }

void exit() {
  trigger_sys_call(SystemCall::Exit);

  while (true) {
    asm volatile("wfi");
  }
}
} // namespace kernel::sys