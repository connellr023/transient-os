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

#include "uart0.hpp"
#include "mbox.hpp"

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart0::init() {
  uint32_t r;

  /* initialize UART */
  *UART0_CR = 0; // turn off UART0

  /* set up clock for consistent divisor values */
  mbox::buffer[0] = 9 * 4;
  mbox::buffer[1] = MBOX_REQUEST;
  mbox::buffer[2] = MBOX_TAG_SETCLKRATE; // set clock rate
  mbox::buffer[3] = 12;
  mbox::buffer[4] = 8;
  mbox::buffer[5] = 2;       // UART clock
  mbox::buffer[6] = 4000000; // 4Mhz
  mbox::buffer[7] = 0;       // clear turbo
  mbox::buffer[8] = MBOX_TAG_LAST;

  mbox::call(MBOX_CH_PROP);

  /* map UART0 to GPIO pins */
  r = *GPFSEL1;
  r &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15
  r |= (4 << 12) | (4 << 15);    // alt0

  *GPFSEL1 = r;
  *GPPUD = 0; // enable pins 14 and 15

  r = 150;

  while (r--)
    asm volatile("nop");

  *GPPUDCLK0 = (1 << 14) | (1 << 15);
  r = 150;

  while (r--)
    asm volatile("nop");

  *GPPUDCLK0 = 0; // flush GPIO setup

  *UART0_ICR = 0x7FF; // clear interrupts
  *UART0_IBRD = 2;    // 115200 baud
  *UART0_FBRD = 0xB;
  *UART0_LCRH = 0x7 << 4; // 8n1, enable FIFOs
  *UART0_CR = 0x301;      // enable Tx, Rx, UART
}

/**
 * Send a character
 */
void uart0::send(uint32_t c) {
  /* wait until we can send */
  do {
    asm volatile("nop");
  } while (*UART0_FR & 0x20);
  /* write the character to the buffer */
  *UART0_DR = c;
}

/**
 * Display a string
 */
void uart0::puts(const char *s) {
  while (*s) {
    /* convert newline to carrige return + newline */
    if (*s == '\n')
      uart0::send('\r');

    uart0::send(*s++);
  }
}

/**
 * Display a binary value in hexadecimal
 */
void uart0::hex(uint64_t d) {
  uint64_t n;
  int c;

  for (c = 60; c >= 0; c -= 4) {
    // get highest tetrad
    n = (d >> c) & 0xF;

    // 0-9 => '0'-'9', 10-15 => 'A'-'F'
    n += n > 9 ? 0x37 : 0x30;

    uart0::send(n);
  }
}