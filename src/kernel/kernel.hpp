#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "../utils/mem_utils.hpp"
#include "threads/scheduler.hpp"
#include "threads/thread_control_block.hpp"
#include <stdint.h>

namespace kernel {
typedef void (*string_output_handler_t)(const char *);
typedef void (*hex_output_handler_t)(uint64_t);

extern threads::SchedulerQueue scheduler;

void init_dbg_out(string_output_handler_t string_handler = nullptr,
                  hex_output_handler_t hex_handler = nullptr);
void init_thread(threads::ThreadControlBlock *tcb);

void start();

void safe_put(const char *str);
void safe_hex(uint64_t value);

void panic_handler();
} // namespace kernel

#endif // KERNEL_HPP