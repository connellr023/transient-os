#ifndef INTERRUPT_REQUESTS_HPP
#define INTERRUPT_REQUESTS_HPP

#include "mmio.hpp"
#include <stdint.h>

#define ENABLE_IRQS_1 ((volatile uint32_t *)(MMIO_BASE + 0x0000B210))
#define SYSTEM_TIMER_IRQ_1 (1 << 1)

#endif // INTERRUPT_REQUESTS_HPP