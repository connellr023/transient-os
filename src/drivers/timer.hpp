#ifndef TIMER_HPP
#define TIMER_HPP

#include "gpio.hpp"
#include <stdint.h>

#define TIMER_CS ((volatile uint32_t *)(MMIO_BASE + 0x00003000))
#define TIMER_COUNTER_LOW ((volatile uint32_t *)(MMIO_BASE + 0x00003004))
#define TIMER_COUNTER_HIGH ((volatile uint32_t *)(MMIO_BASE + 0x00003008))

#define TIMER_CMP_1 ((volatile uint32_t *)(MMIO_BASE + 0x00003010))
#define TIMER_CS_M1 (1 << 1)

#endif // TIMER_HPP