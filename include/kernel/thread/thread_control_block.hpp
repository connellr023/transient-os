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

#include <kernel/memory/free_list.hpp>
#include <stdint.h>

/**
 * @brief Enumeration of the possible states of a thread.
 */
enum class ThreadState : uint8_t {
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
  uint32_t wake_time;
  uint64_t sp;
  ThreadState state;

public:
  /**
   * @brief Initializes a thread control block.
   * @param quantum_us The amount of microseconds the thread will run for before
   * being interrupted.
   * @param page The address of the page allocated for the thread stack and
   * heap.
   * @param sp The stack pointer of the thread.
   */
  void init(uint32_t quantum_us, void *page, void *sp);

  /**
   * @brief Frees the page allocated for the thread stack and heap.
   */
  void *get_page() const { return reinterpret_cast<void *>(this->page_addr); }

  /**
   * @brief Returns the stack pointer of the thread.
   */
  void *get_sp() const { return reinterpret_cast<void *>(this->sp); }

  /**
   * @brief Returns the thread ID.
   */
  uint64_t get_thread_id() const { return this->thread_id; }

  /**
   * @brief Returns the time quantum of the thread in microseconds.
   */
  uint32_t get_quantum() const { return this->quantum_us; }

  /**
   * @brief Returns the wake time of the thread in microseconds. Only applicable
   * if the thread is sleeping.
   */
  uint32_t get_wake_time() const { return this->wake_time; }

  /**
   * @brief Returns the free list node for the start of this thread's heap.
   */
  FreeListNode *get_heap_start() const {
    return reinterpret_cast<FreeListNode *>(this->page_addr);
  }

  bool is_complete() const { return this->state == ThreadState::Complete; }

  bool is_ready() const { return this->state == ThreadState::Ready; }

  bool is_running() const { return this->state == ThreadState::Running; }

  bool is_sleeping() const { return this->state == ThreadState::Sleeping; }

  void mark_as_ready() { this->state = ThreadState::Ready; }

  void mark_as_running() { this->state = ThreadState::Running; }

  void mark_as_complete() { this->state = ThreadState::Complete; }

  /**
   * @brief Marks the thread as sleeping and sets the wake time.
   * @param wake_time The time in microseconds when the scheduler should wake
   * this thread.
   */
  void mark_as_sleeping(uint32_t wake_time) {
    this->state = ThreadState::Sleeping;
    this->wake_time = wake_time;
  }

  /**
   * @brief Sets the stack pointer of the thread.
   */
  void set_sp(void *sp) { this->sp = reinterpret_cast<uintptr_t>(sp); }
};

#endif // THREAD_CONTROL_BLOCK_HPP