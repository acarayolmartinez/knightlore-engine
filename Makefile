# Simple Makefile for host development and z88dk

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -I.

.PHONY: all host zx clean

all: host

host: demo/main.c platform/zx_platform.c platform/zx_platform.h
	$(CC) $(CFLAGS) -DHOST demo/main.c platform/zx_platform.c -o knightlore-demo

zx: demo/main.c platform/zx_platform.c platform/zx_platform.h
	@echo "Building for ZX with z88dk..."
	@# Requires z88dk in PATH. Produces a .z80 snapshot and tape-ready files depending on zcc options.
	zcc +zx -vn -m -create-app -clib=sdcc_iy demo/main.c platform/zx_platform.c -o knightlore
	@echo "Build complete. Output: knightlore.z80 (and related files)"

clean:
	rm -f knightlore-demo knightlore.z80 knightlore.rom knightlore
