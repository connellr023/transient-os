#ifndef INTERRUPT_REQUESTS_HPP
#define INTERRUPT_REQUESTS_HPP

#include "gpio.hpp"
#include <stdint.h>

#define ENABLE_IRQS_1 ((volatile uint32_t *)(MMIO_BASE + 0x0000B210))

#define SYSTEM_TIMER_IRQ_0 (1 << 0)
#define SYSTEM_TIMER_IRQ_1 (1 << 1)
#define SYSTEM_TIMER_IRQ_2 (1 << 2)
#define SYSTEM_TIMER_IRQ_3 (1 << 3)

#endif // INTERRUPT_REQUESTS_HPP