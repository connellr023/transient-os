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

#ifndef SYS_CALL_HPP
#define SYS_CALL_HPP

#include <stdint.h>

namespace kernel::sys {
/**
 * @brief An enumeration of possible system call codes.
 */
enum class SystemCall : uint8_t {
  HelloWorld = 0,
  HeapAlloc = 1,
};

/**
 * @brief Utility function to trigger a system call.
 * @param call_code The system call code.
 * @param arg The argument to the system call.
 * @return The return value of the system call.
 */
void *trigger_system_call(SystemCall call_code,
                          void *arg) asm("_trigger_system_call");
} // namespace kernel::sys

#endif // SYS_CALL_HPP