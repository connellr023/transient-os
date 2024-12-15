#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "threads/scheduler.hpp"
#include "threads/thread_control_block.hpp"
#include <stdint.h>

namespace kernel {
using namespace threads;

typedef void (*string_output_handler_t)(const char *);
typedef void (*hex_output_handler_t)(uint64_t);

void init_dbg_out(string_output_handler_t string_handler = nullptr,
                  hex_output_handler_t hex_handler = nullptr);

const ThreadControlBlock *context_switch(void *interrupted_sp);

bool spawn_thread(ThreadControlBlock *tcb);
void join_thread(ThreadControlBlock *tcb);

void thread_return_handler();

void start();

void safe_put(const char *str);
void safe_hex(uint64_t value);
} // namespace kernel

#endif // KERNEL_HPP