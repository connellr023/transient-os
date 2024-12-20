#include "../../../include/kernel/sys/sys_calls.hpp"

namespace kernel::sys {
void puts(const char *str) { trigger_sys_call(SystemCall::PutString, str); }
void *page_alloc() { return nullptr; }
void page_free(void *page) { return; }
void *heap_alloc(uint64_t size) { return nullptr; }
void heap_free(void *ptr) { return; }
void yield() { trigger_sys_call(SystemCall::Yield); }
} // namespace kernel::sys