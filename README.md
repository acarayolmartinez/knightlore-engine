knightlore-engine
==================

This repository contains a small ZX Spectrum oriented rendering platform (C) and a host-side demo for development.

What I added
- platform/zx_platform.h: public header for platform functions.
- platform/zx_platform.c: improved and host-friendly implementation
  - conditional HOST build uses an in-memory framebuffer and POSIX terminal input.
  - zx_blit_tile now supports arbitrary pixel-aligned X positions (handles byte boundary splits).
  - zx_keypressed has a host implementation using non-blocking stdin (termios + select).
  - non-HOST zx_keypressed uses a simple z88dk inp(0xFE) check to detect "any key".
- demo/main.c: simple demo that draws a few tiles and waits for a keypress.
- Makefile: builds a host demo (target `host`) as `knightlore-demo` and a z88dk target `zx` without the unsupported `-clib=sdcc_iy` flag.

Build and run (host):

  make
  ./knightlore-demo

Build for ZX using z88dk:

  make zx

This uses the default z88dk library available in your installation. If you want a specific Spectrum output format such as .tap or .sna, I can add the exact command needed.

Notes
- The HOST path is intended for native development and testing. For building for an actual ZX Spectrum toolchain, the code now provides a basic keypress detector using inp(0xFE). For a full keyboard matrix scan and proper input mapping, we should implement row-by-row scanning and key mapping.
- The `-clib=sdcc_iy` flag was removed because it causes failure in builds where that runtime variant is unavailable.
