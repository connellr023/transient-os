#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "../utils/mem_utils.hpp"
#include "threads/scheduler.hpp"
#include <stdint.h>

namespace kernel {
typedef void (*thread_handler_t)(void *);
typedef void (*string_output_handler_t)(const char *);
typedef void (*hex_output_handler_t)(uint64_t);

// These must match the values in config.txt
constexpr uint64_t total_mem_mb = 1024; // total_mem
constexpr uint64_t gpu_mem_mb = 64;     // gpu_mem

// The highest usable physical address for the kernel and thread stacks
constexpr uint64_t high_memory_end = mem_align_16(0x30000000);
// mem_align_16((total_mem_mb - gpu_mem_mb) * 1024 * 1024);

extern threads::SchedulerQueue scheduler;

void init_dbg_out(string_output_handler_t string_handler = nullptr,
                  hex_output_handler_t hex_handler = nullptr);
void init_thread(thread_handler_t handler, uint64_t stack_size,
                 void *arg = nullptr);

void start();

void safe_put(const char *str);
void safe_hex(uint64_t value);

__attribute__((noreturn)) void panic_handler();
} // namespace kernel

#endif // KERNEL_HPP