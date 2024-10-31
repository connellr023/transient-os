/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 * Modified by 2024 connellr023@github
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "gpio.hpp"

/* PL011 UART registers */
#define UART0_DR ((volatile unsigned int *)(MMIO_BASE + 0x00201000))
#define UART0_FR ((volatile unsigned int *)(MMIO_BASE + 0x00201018))
#define UART0_IBRD ((volatile unsigned int *)(MMIO_BASE + 0x00201024))
#define UART0_FBRD ((volatile unsigned int *)(MMIO_BASE + 0x00201028))
#define UART0_LCRH ((volatile unsigned int *)(MMIO_BASE + 0x0020102C))
#define UART0_CR ((volatile unsigned int *)(MMIO_BASE + 0x00201030))
#define UART0_IMSC ((volatile unsigned int *)(MMIO_BASE + 0x00201038))
#define UART0_ICR ((volatile unsigned int *)(MMIO_BASE + 0x00201044))

namespace uart0
{
    void init();
    void send(unsigned int c);
    void puts(char *s);
    void hex(unsigned int d);
}
