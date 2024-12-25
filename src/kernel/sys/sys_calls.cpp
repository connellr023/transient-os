#include "../../../include/kernel/sys/sys_calls.hpp"
#include "../../../include/kernel/kernel.hpp"

namespace kernel::sys {
void puts(const char *str) { trigger_sys_call(SystemCall::PutString, str); }

void *heap_alloc(uint64_t size) {
  return trigger_sys_call(SystemCall::HeapAlloc,
                          reinterpret_cast<void *>(size));
}

void heap_free(void *ptr) { trigger_sys_call(SystemCall::HeapFree, ptr); }

void yield() { trigger_sys_call(SystemCall::Yield); }

void exit() {
  trigger_sys_call(SystemCall::Exit);

  // Should not reach here
  while (true) {
    safe_puts("should not reach here\n");
    asm volatile("wfi");
  }
}

void sleep(uint32_t sleep_us) {
  trigger_sys_call(SystemCall::Sleep, reinterpret_cast<void *>(sleep_us));
}

bool spawn_thread(ThreadControlBlock *tcb) {
  return static_cast<bool>(trigger_sys_call(SystemCall::SpawnThread, tcb));
}
} // namespace kernel::sys