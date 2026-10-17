#include <stdio.h>
#include <stdint.h>
#include "platform/zx_platform.h"

/* Simple 8x8 checker tile */
static const uint8_t demo_tile[8] = {
    0b11111111,
    0b10000001,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b10000001,
    0b11111111,
};

int main(void) {
    zx_init();

    /* draw several tiles at different positions */
    zx_blit_tile(10, 10, demo_tile);
    zx_blit_tile(50, 20, demo_tile);
    zx_blit_tile(120, 40, demo_tile);

#ifdef HOST
    printf("Demo: press any key to exit.\n");

    while (!zx_keypressed()) {
        zx_wait_vsync();
    }

    printf("Key pressed, exiting.\n");
#else
    /* On ZX, wait for any key (no stdio) */
    while (!zx_keypressed()) {
        zx_wait_vsync();
    }
#endif

    return 0;
}
