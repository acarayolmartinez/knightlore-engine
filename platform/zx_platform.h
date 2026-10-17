#ifndef ZX_PLATFORM_H
#define ZX_PLATFORM_H

#include <stdint.h>

void zx_init(void);
void zx_wait_vsync(void);
void zx_blit_tile(int px, int py, const uint8_t *tile);
void zx_set_attr(int tx, int ty, uint8_t attr);
int zx_keypressed(void);
uint8_t *zx_screen_ptr(void);

#endif /* ZX_PLATFORM_H */
