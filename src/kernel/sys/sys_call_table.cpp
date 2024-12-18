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

#include "../../../include/kernel/sys/sys_call_table.hpp"
#include "../../../include/kernel/kernel.hpp"

#define SYSTEM_CALL_TABLE_SIZE                                                 \
  (sizeof(system_call_table) / sizeof(system_call_table[0]))

using namespace kernel::sys;

/**
 * @brief Test system call that writes a message to the kernel debug output
 * handler. Should only be invoked by system call handler.
 * @param _arg Unused.
 * @return Always nullptr.
 */
void *sys_call_hello_world(void *_arg) {
  // Test system call
  kernel::safe_put("Hello, world!\n");

  return nullptr;
}

/**
 * @brief System call that allocates a block of memory on the heap. Should only
 * be invoked by system call handler.
 * @param size The size of the block to allocate.
 * @return A pointer to the allocated block.
 */
void *sys_call_heap_alloc(void *size) {
  const uint64_t alloc_size = reinterpret_cast<uint64_t>(size);

  // For now
  return nullptr;
}

/**
 * @brief Table of system call handlers.
 */
const system_call_handler system_call_table[SYSTEM_CALL_TABLE_SIZE] = {
    sys_call_hello_world,
    sys_call_heap_alloc,
};

void *kernel::sys::handle_system_call(SystemCall call_code, void *arg) {
  const uint8_t numerical_call_code = static_cast<uint8_t>(call_code);

  if (numerical_call_code >= SYSTEM_CALL_TABLE_SIZE) {
    return nullptr;
  }

  return system_call_table[numerical_call_code](arg);
}