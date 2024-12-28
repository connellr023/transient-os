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

#include <api/include/sys/sys_calls.hpp>

namespace api::sys {
void set_output_handler(output_handler_t handler) {
  trigger_sys_call(SystemCall::SetOutputHandler,
                   reinterpret_cast<void *>(handler));
}

void puts(const char *str) { trigger_sys_call(SystemCall::PutString, str); }

void *heap_alloc(uint64_t size) {
  return trigger_sys_call(SystemCall::HeapAlloc,
                          reinterpret_cast<void *>(size));
}

void heap_free(void *ptr) { trigger_sys_call(SystemCall::HeapFree, ptr); }

void heap_realloc(void *ptr, uint64_t new_size) {
  HeapReallocArgs args;
  args.ptr = ptr;
  args.new_size = new_size;

  trigger_sys_call(SystemCall::HeapRealloc, &args);
}

void yield() { trigger_sys_call(SystemCall::Yield); }

void exit() {
  trigger_sys_call(SystemCall::Exit);

  // Should not reach here
  while (true) {
    asm volatile("wfi");
  }
}

void sleep(uint32_t sleep_us) {
  trigger_sys_call(SystemCall::Sleep, reinterpret_cast<void *>(sleep_us));
}

/**
 * @brief Helper function to spawn a thread either in kernel or user mode.
 */
bool spawn_thread(ThreadHandle *handle, thread_handler_t handler,
                  uint32_t quantum_us, void *arg, bool is_kernel) {
  AllocThreadArgs args;
  args.handle = handle;
  args.handler = handler;
  args.quantum_us = quantum_us;
  args.arg = arg;

  return static_cast<bool>(trigger_sys_call(
      is_kernel ? SystemCall::SpawnKernelThread : SystemCall::SpawnUserThread,
      &args));
}

bool spawn_kernel_thread(ThreadHandle *handle, thread_handler_t handler,
                         uint32_t quantum_us, void *arg) {
  return spawn_thread(handle, handler, quantum_us, arg, true);
}

bool spawn_user_thread(ThreadHandle *handle, thread_handler_t handler,
                       uint32_t quantum_us, void *arg) {
  return spawn_thread(handle, handler, quantum_us, arg, false);
}
} // namespace api::sys