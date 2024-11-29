/**
 * https://gist.github.com/petemoore/4a17f701d2ad57025a1f6ab90a66ce33
 */

#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <stdint.h>

namespace kernel {
typedef void (*thread_handler_t)(void);
typedef void (*panic_handler_t)(void);

// These must match the values in config.txt
constexpr uint64_t total_mem_mb = 1024; // total_mem
constexpr uint64_t gpu_mem_mb = 64;     // gpu_mem

// The highest usable physical address for the kernel and thread stacks
constexpr uint64_t high_memory_end = (total_mem_mb - gpu_mem_mb) * 1024 * 1024;

void init_thread(thread_handler_t handler, uint64_t stack_size);

void init();

void default_panic_handler();
panic_handler_t panic_handler = default_panic_handler;
} // namespace kernel

#endif // KERNEL_HPP