#include "zx_platform.h"

#include <stdint.h>
#include <string.h>

#ifndef HOST
volatile uint8_t *const SCREEN = (volatile uint8_t *)0x4000;
volatile uint8_t *const ATTR = (volatile uint8_t *)0x5800;
#else
static uint8_t host_screen[6912];
static uint8_t host_attr[768];
volatile uint8_t *const SCREEN = host_screen;
volatile uint8_t *const ATTR = host_attr;
#endif

static uint16_t screen_offset_for(int x, int y) {
    int x_byte = x >> 3;
    return ((y & 0xC0) << 5) + ((y & 0x38) << 2) + ((y & 0x07) << 8) + x_byte;
}

void zx_init(void) {
    memset((void *)SCREEN, 0x00, 6912);
    memset((void *)ATTR, 0x07, 768);
}

void zx_wait_vsync(void) {
#ifndef HOST
    for (volatile int i = 0; i < 1200; ++i) {
        __asm__("nop");
    }
#else
    /* On host, sleep ~16ms to simulate 60Hz frame */
    #include <unistd.h>
    usleep(16000);
#endif
}

void zx_blit_tile(int px, int py, const uint8_t *tile) {
    if (px < 0 || px + 7 < 0 || px >= 256 || py < 0 || py + 7 < 0 || py >= 192) {
        return; /* fully outside */
    }

    /* clip horizontally to [0,255] to avoid writing past screen */
    if (px < 0 || px + 7 >= 256) {
        /* simple clipping: do nothing if any part would be outside */
        return;
    }

    int x_byte = px >> 3;
    int shift = px & 7;

    for (int row = 0; row < 8; ++row) {
        int y = py + row;
        if (y < 0 || y >= 192) {
            continue;
        }
        int offset = screen_offset_for(x_byte, y);

        uint8_t t = tile[row];
        if (shift == 0) {
            SCREEN[offset] = t;
        } else {
            uint8_t low = t >> shift;
            uint8_t high = t << (8 - shift);

            /* write affected bits preserving surrounding bits */
            /* low affects bits [0..7-shift] in low byte */
            uint8_t low_mask = ((1u << (8 - shift)) - 1u);
            SCREEN[offset] = (SCREEN[offset] & ~low_mask) | (low & low_mask);

            /* high affects the remaining bits in the next byte */
            if (x_byte + 1 < 32) {
                uint8_t high_mask = ~low_mask;
                SCREEN[offset + 1] = (SCREEN[offset + 1] & ~high_mask) | (high & high_mask);
            }
        }
    }
}

void zx_set_attr(int tx, int ty, uint8_t attr) {
    if (tx < 0 || ty < 0 || tx >= 32 || ty >= 24) {
        return;
    }

    ATTR[ty * 32 + tx] = attr;
}

#ifndef HOST
/* ZX (z88dk) key detection using port 0xFE.
   This is a simple "any key pressed" check. A proper keyboard scanner
   should poll each row (0xfe with appropriate row mask) and decode keys.
   Using inp() from z88dk/SDCC runtime. */

extern unsigned char inp(unsigned short);

int zx_keypressed(void) {
    unsigned char v = inp(0xFE);
    return (v != 0xFF);
}

uint8_t *zx_screen_ptr(void) {
    return (uint8_t *)SCREEN;
}

#else
/* Host-side implementations for development / testing */

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdio.h>

static struct termios orig_term;
static int term_init = 0;

static void host_term_init(void) {
    if (term_init) return;
    tcgetattr(STDIN_FILENO, &orig_term);
    struct termios t = orig_term;
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    term_init = 1;
}

static void host_term_restore(void) {
    if (!term_init) return;
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
    term_init = 0;
}

int zx_keypressed(void) {
    host_term_init();
    fd_set rfds;
    struct timeval tv = {0, 0};
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);
    int ret = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
    if (ret > 0 && FD_ISSET(STDIN_FILENO, &rfds)) {
        /* consume one byte */
        char c;
        ssize_t r = read(STDIN_FILENO, &c, 1);
        (void)r;
        return (int)(unsigned char)c;
    }
    return 0;
}

uint8_t *zx_screen_ptr(void) {
    return (uint8_t *)SCREEN;
}

/* ensure terminal restored on exit */
__attribute__((destructor)) static void _host_cleanup(void) {
    host_term_restore();
}
#endif
