#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "threads/thread_queue.hpp"
#include <stdint.h>

namespace kernel {
typedef void (*thread_handler_t)(void);

// These must match the values in config.txt
constexpr uint64_t total_mem_mb = 1024; // total_mem
constexpr uint64_t gpu_mem_mb = 64;     // gpu_mem

// The highest usable physical address for the kernel and thread stacks
constexpr uint64_t high_memory_end = (total_mem_mb - gpu_mem_mb) * 1024 * 1024;

extern threads::ThreadQueue thread_queue;

void init_thread(thread_handler_t handler, uint64_t stack_size);
void init();

void panic_handler();
} // namespace kernel

#endif // KERNEL_HPP