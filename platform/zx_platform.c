#include "zx_platform.h"

#include <stdint.h>
#include <string.h>

volatile uint8_t *const SCREEN = (volatile uint8_t *)0x4000;
volatile uint8_t *const ATTR = (volatile uint8_t *)0x5800;

static uint16_t screen_offset_for(int x, int y) {
    int x_byte = x >> 3;
    return ((y & 0xC0) << 5) + ((y & 0x38) << 2) + ((y & 0x07) << 8) + x_byte;
}

void zx_init(void) {
    memset((void *)SCREEN, 0x00, 6912);
    memset((void *)ATTR, 0x07, 768);
}

void zx_wait_vsync(void) {
    for (volatile int i = 0; i < 1200; ++i) {
        __asm__("nop");
    }
}

void zx_blit_tile(int px, int py, const uint8_t *tile) {
    int x_byte = px >> 3;
    for (int row = 0; row < 8; ++row) {
        int y = py + row;
        if (y < 0 || y >= 192) {
            continue;
        }
        int offset = screen_offset_for(x_byte, y) + (x_byte ? 0 : 0);
        SCREEN[offset] = tile[row];
    }
}

void zx_set_attr(int tx, int ty, uint8_t attr) {
    if (tx < 0 || ty < 0 || tx >= 32 || ty >= 24) {
        return;
    }

    ATTR[ty * 32 + tx] = attr;
}

int zx_keypressed(void) {
    return 0;
}

uint8_t *zx_screen_ptr(void) {
    return (uint8_t *)SCREEN;
}
