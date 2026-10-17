knightlore-engine
==================

This repository contains a small ZX Spectrum oriented rendering platform (C) and a host-side demo for development.

What I added
- platform/zx_platform.h: public header for platform functions.
- platform/zx_platform.c: improved and host-friendly implementation
  - conditional HOST build uses an in-memory framebuffer and POSIX terminal input.
  - zx_blit_tile now supports arbitrary pixel-aligned X positions (handles byte boundary splits).
  - zx_keypressed has a host implementation using non-blocking stdin (termios + select).
- demo/main.c: simple demo that draws a few tiles and waits for a keypress.
- Makefile: builds a host demo (target `host`) as `knightlore-demo`.
- README.md (this file).

Build and run (host):

  make
  ./knightlore-demo

Notes
- The HOST path is intended for native development and testing. For building for an actual ZX Spectrum toolchain, remove -DHOST and adapt platform/zx_platform.c to your toolchain/IO conventions. The original hardware I/O (volatile memory addresses and a stubbed zx_keypressed) are preserved when HOST is not defined.

