#include "framebuffer.hpp"
#include "mbox.hpp"

uint32_t width, height, pitch;
bool is_rgb;

uint8_t *lfb;

bool framebuffer::init() {
  mbox::buffer[0] = 35 * 4;
  mbox::buffer[1] = MBOX_REQUEST;

  mbox::buffer[2] = 0x48003; // Set physical width/height
  mbox::buffer[3] = 8;
  mbox::buffer[4] = 8;
  mbox::buffer[5] = 800;
  mbox::buffer[6] = 480;

  mbox::buffer[7] = 0x48004; // Set virtual width/height
  mbox::buffer[8] = 8;
  mbox::buffer[9] = 8;
  mbox::buffer[10] = 800; // FrameBufferInfo.virtual_width
  mbox::buffer[11] = 480; // FrameBufferInfo.virtual_height

  mbox::buffer[12] = 0x48009; // Set virtual offset
  mbox::buffer[13] = 8;
  mbox::buffer[14] = 8;
  mbox::buffer[15] = 0; // FrameBufferInfo.x_offset
  mbox::buffer[16] = 0; // FrameBufferInfo.y.offset

  mbox::buffer[17] = 0x48005; // Set depth
  mbox::buffer[18] = 4;
  mbox::buffer[19] = 4;
  mbox::buffer[20] = 32; // FrameBufferInfo.depth

  mbox::buffer[21] = 0x48006; // Set pixel order
  mbox::buffer[22] = 4;
  mbox::buffer[23] = 4;
  mbox::buffer[24] = 1; // RGB, not BGR preferably

  mbox::buffer[25] = 0x40001; // Get framebuffer, gets alignment on request
  mbox::buffer[26] = 8;
  mbox::buffer[27] = 8;
  mbox::buffer[28] = 4096; // FrameBufferInfo.pointer
  mbox::buffer[29] = 0;    // FrameBufferInfo.size

  mbox::buffer[30] = 0x40008; // Get pitch
  mbox::buffer[31] = 4;
  mbox::buffer[32] = 4;
  mbox::buffer[33] = 0; // FrameBufferInfo.pitch

  mbox::buffer[34] = MBOX_TAG_LAST;

  if (mbox::call(MBOX_CH_PROP) && mbox::buffer[20] == 32 &&
      mbox::buffer[28] != 0) {
    mbox::buffer[28] &= 0x3FFFFFFF; // Convert GPU address to ARM address
    width = mbox::buffer[5];        // Get actual physical width
    height = mbox::buffer[6];       // Get actual physical height
    pitch = mbox::buffer[33];       // Get number of bytes per line
    is_rgb = mbox::buffer[24];      // Get the actual channel order
    lfb = reinterpret_cast<uint8_t *>(static_cast<uint64_t>(mbox::buffer[28]));

    return true;
  } else {
    return false;
  }
}

void framebuffer::draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
  const uint32_t offs = (y * pitch) + (x * 4);
  *(reinterpret_cast<uint32_t *>(lfb + offs)) =
      is_rgb ? color : bgr_to_rgb(color);
}

void framebuffer::fill_screen(uint32_t color) {
  for (uint32_t row = 0; row < height; row++)
    for (uint32_t col = 0; col < width; col++)
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