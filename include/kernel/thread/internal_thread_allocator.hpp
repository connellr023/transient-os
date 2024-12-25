#ifndef INTERNAL_THREAD_ALLOCATOR_HPP
#define INTERNAL_THREAD_ALLOCATOR_HPP

#define CPU_CTX_STACK_SIZE (34 * 8) // 34 registers * 8 bytes per register

#ifndef __ASSEMBLER__

#include <kernel/thread/thread_control_block.hpp>
#include <kernel/thread/thread_handle.hpp>

#define FP_IDX 29
#define LR_IDX 30
#define ELR_EL1_IDX 31
#define SPSR_EL1_IDX 32

enum class PSRExceptionLevel : uint8_t {
  EL0t = 0x00,
  EL1t = 0x04,
};

namespace kernel::thread {
/**
 * @brief Allocates and initializes a page of memory for a new thread.
 * @param handler The handler function for the thread.
 * @param quantum_us The time quantum for the thread.
 * @param arg The argument to pass to the thread handler.
 * @return A pointer to the thread control block for the new thread. Will be
 * nullptr if allocation fails.
 */
ThreadControlBlock *internal_alloc_thread(thread_handler_t handler,
                                          uint32_t quantum_us,
                                          void *arg = nullptr);
} // namespace kernel::thread

#endif // __ASSEMBLER__

#endif // INTERNAL_THREAD_ALLOCATOR_HPP