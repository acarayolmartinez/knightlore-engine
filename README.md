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
- Makefile: builds a host demo (target `host`) as `knightlore-demo`, a z88dk target `zx`, and a TAP-oriented z88dk target `tap`.

Build and run (host):

  make
  ./knightlore-demo

Build for ZX using z88dk (default Spectrum app output):

  make zx

Build for TAP output:

  make tap

This uses the default z88dk library available in your installation and does not rely on the unsupported `-clib=sdcc_iy` flag.

Notes
- The HOST path is intended for native development and testing.
- The ZX path uses a simple `inp(0xFE)` keypress check; for full keyboard matrix scanning, implement row-by-row polling.
- Different z88dk builds may create slightly different output file names/extensions depending on target configuration and version.
