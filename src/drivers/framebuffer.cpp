#include "framebuffer.hpp"
#include "mbox.hpp"

uint32_t width, height, pitch, isrgb;
uint8_t *lfb;

void framebuffer::init() {
  mbox::buffer[0] = 35 * 4;
  mbox::buffer[1] = MBOX_REQUEST;

  mbox::buffer[2] = 0x48003; // set phy wh
  mbox::buffer[3] = 8;
  mbox::buffer[4] = 8;
  mbox::buffer[5] = 1024; // FrameBufferInfo.width
  mbox::buffer[6] = 768;  // FrameBufferInfo.height

  mbox::buffer[7] = 0x48004; // set virt wh
  mbox::buffer[8] = 8;
  mbox::buffer[9] = 8;
  mbox::buffer[10] = 1024; // FrameBufferInfo.virtual_width
  mbox::buffer[11] = 768;  // FrameBufferInfo.virtual_height

  mbox::buffer[12] = 0x48009; // set virt offset
  mbox::buffer[13] = 8;
  mbox::buffer[14] = 8;
  mbox::buffer[15] = 0; // FrameBufferInfo.x_offset
  mbox::buffer[16] = 0; // FrameBufferInfo.y.offset

  mbox::buffer[17] = 0x48005; // set depth
  mbox::buffer[18] = 4;
  mbox::buffer[19] = 4;
  mbox::buffer[20] = 32; // FrameBufferInfo.depth

  mbox::buffer[21] = 0x48006; // set pixel order
  mbox::buffer[22] = 4;
  mbox::buffer[23] = 4;
  mbox::buffer[24] = 1; // RGB, not BGR preferably

  mbox::buffer[25] = 0x40001; // get framebuffer, gets alignment on request
  mbox::buffer[26] = 8;
  mbox::buffer[27] = 8;
  mbox::buffer[28] = 4096; // FrameBufferInfo.pointer
  mbox::buffer[29] = 0;    // FrameBufferInfo.size

  mbox::buffer[30] = 0x40008; // get pitch
  mbox::buffer[31] = 4;
  mbox::buffer[32] = 4;
  mbox::buffer[33] = 0; // FrameBufferInfo.pitch

  mbox::buffer[34] = MBOX_TAG_LAST;

  // this might not return exactly what we asked for, could be
  // the closest supported resolution instead
  if (mbox::call(MBOX_CH_PROP) && mbox::buffer[20] == 32 &&
      mbox::buffer[28] != 0) {
    mbox::buffer[28] &= 0x3FFFFFFF; // convert GPU address to ARM address
    width = mbox::buffer[5];        // get actual physical width
    height = mbox::buffer[6];       // get actual physical height
    pitch = mbox::buffer[33];       // get number of bytes per line
    isrgb = mbox::buffer[24];       // get the actual channel order
    lfb = (uint8_t *)((uint64_t)mbox::buffer[28]);
  }
}

void framebuffer::draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
  const uint32_t offs = (y * pitch) + (x * 4);
  *((uint32_t *)(lfb + offs)) = color;
}

void framebuffer::fill_screen(uint32_t color) {
  for (int row = 0; row < height; row++)
    for (int col = 0; col < width; col++)
      draw_pixel(col, row, color);
}

void framebuffer::draw_rect(int x1, int y1, int x2, int y2, uint32_t color,
                            bool fill) {
  int y = y1;

  while (y <= y2) {
    int x = x1;
    while (x <= x2) {
      // Conditionally render based on current color
      if ((x == x1 || x == x2) || (y == y1 || y == y2))
        draw_pixel(x, y, color);
      else if (fill)
        draw_pixel(x, y, color);
      x++;
    }
    y++;
  }
}