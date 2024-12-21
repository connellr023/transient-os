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

#ifndef THREAD_CONTROL_BLOCK_HPP
#define THREAD_CONTROL_BLOCK_HPP

#define CPU_CTX_STACK_SIZE (17 * 16) // 17 pairs of 8 byte registers

#ifndef __ASSEMBLER__

#define FP_IDX 29
#define LR_IDX 30
#define ELR_EL1_IDX 31
#define SPSR_EL1_IDX 32

#include <stdint.h>

typedef void (*thread_handler_t)(void *);

/**
 * @brief Enumeration of the possible states of a thread.
 */
enum class ThreadState : uint8_t {
  Unallocated,
  Ready,
  Running,
  Sleeping,
  Complete,
};

/**
 * @brief A class to encapsulate thread information.
 */
class ThreadControlBlock {
private:
  uint64_t thread_id;
  uint64_t page_addr;
  uint32_t quantum_us;
  uint64_t sleep_until_us;
  ThreadState state;
  uint64_t sp;
  thread_handler_t handler;
  void *arg;

public:
  /**
   * @brief Initializes a thread control block.
   * @param handler A pointer to the function that the thread will run.
   * @param quantum_us The amount of microseconds the thread will run for before
   * being interrupted.
   * @param arg An optional argument to pass to the thread function.
   */
  void init(thread_handler_t handler, uint32_t quantum_us, void *arg = nullptr);

  /**
   * @brief Allocates a page for the thread stack and heap.
   * @return True if the page was successfully allocated, false otherwise.
   */
  bool alloc();

  void *get_page() const { return reinterpret_cast<void *>(this->page_addr); }

  void *get_sp() const { return reinterpret_cast<void *>(this->sp); }

  uint64_t get_thread_id() const { return this->thread_id; }

  uint32_t get_burst_time() const { return this->quantum_us; }

  bool is_complete() const { return this->state == ThreadState::Complete; }

  bool is_ready() const { return this->state == ThreadState::Ready; }

  bool is_running() const { return this->state == ThreadState::Running; }

  bool is_sleeping() const { return this->state == ThreadState::Sleeping; }

  bool is_allocated() const { return this->state != ThreadState::Unallocated; }

  void mark_as_ready() { this->state = ThreadState::Ready; }

  void mark_as_running() { this->state = ThreadState::Running; }

  void mark_as_complete() { this->state = ThreadState::Complete; }

  void mark_as_sleeping(uint64_t sleep_until_us) {
    this->state = ThreadState::Sleeping;
    this->sleep_until_us = sleep_until_us;
  }

  void set_sp(void *sp) { this->sp = reinterpret_cast<uint64_t>(sp); }
};

#endif // __ASSEMBLER__

#endif // THREAD_CONTROL_BLOCK_HPP