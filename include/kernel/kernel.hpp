#ifndef KERNEL_HPP
#define KERNEL_HPP

#define CPU_CTX_STACK_SIZE (17 * 16) // 17 pairs of 8 byte registers

#define LR_IDX 30
#define ELR_EL1_IDX 31
#define SPSR_EL1_IDX 32

#ifndef __ASSEMBLER__

#include "memory/paging.hpp"

#define THREAD_STACK_SIZE PAGE_SIZE

#include "threads/scheduler.hpp"
#include "threads/thread_control_block.hpp"
#include <stdint.h>

namespace kernel {
using namespace threads;

typedef void (*output_handler_t)(const char *);

void set_output_handler(output_handler_t output_handler);
void start();
void yield();

const ThreadControlBlock *context_switch(void *interrupted_sp);

bool alloc_thread_stack(ThreadControlBlock *tcb);
bool schedule_thread(ThreadControlBlock *tcb);

uint64_t get_thread_id();
void thread_return_handler();
void kernel_panic(const char *msg);

void safe_put(const char *str);
void safe_hex(uint64_t value);
} // namespace kernel

#endif // __ASSEMBLER__

#endif // KERNEL_HPP