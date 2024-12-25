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

#ifndef THREAD_HANDLE_HPP
#define THREAD_HANDLE_HPP

#include <stdint.h>

// Forward declaration to avoid exposing kernel headers
class ThreadControlBlock;

class ThreadHandle {
private:
  ThreadControlBlock *tcb;

public:
  /**
   * @brief Initializes the thread handle with the given thread control block.
   * @param tcb The thread control block.
   */
  void init(ThreadControlBlock *tcb) { this->tcb = tcb; }

  /**
   * @brief Gets the ID of the thread associated with this handle.
   * @return The thread ID.
   */
  uint64_t get_thread_id();

  /**
   * @brief Blocks the current thread until the thread associated with this
   * handle completes.
   */
  void join();
};

#endif // THREAD_HANDLE_HPP