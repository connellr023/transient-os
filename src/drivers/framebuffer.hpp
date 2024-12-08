#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#define MBOX_TAG_SET_PHYS_WIDTH_HEIGHT 0x00048003
#define MBOX_TAG_SET_VIRT_WIDTH_HEIGHT 0x00048004
#define MBOX_TAG_SET_VIRT_OFFSET 0x00048004
#define MBOX_TAG_SET_COLOR_DEPTH 0x00048005
#define MBOX_TAG_SET_PIXEL_ORDER 0x00048006
#define MBOX_TAG_ALLOC_FRAMEBUFFER 0x00040001
#define MBOX_TAG_GET_PITCH 0x00040008

#define PIXEL_ORDER_RGB 1
#define PIXEL_ORDER_BGR 0

#define FB_WIDTH 800
#define FB_HEIGHT 480

#include <stdint.h>

namespace framebuffer {
bool init();
void draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void fill_screen(uint32_t color);
void draw_rect(int x1, int y1, int x2, int y2, uint32_t color,
               bool fill = true);
} // namespace framebuffer

constexpr uint32_t bgr_to_rgb(uint32_t color) {
  return ((color & 0xFF) << 16) | (color & 0xFF00) | ((color & 0xFF0000) >> 16);
}

#endif