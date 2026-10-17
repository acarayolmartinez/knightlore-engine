# Simple Makefile for host development and z88dk

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -I.
ZCC = zcc

.PHONY: all host zx clean

all: host

host: demo/main.c platform/zx_platform.c platform/zx_platform.h
	$(CC) $(CFLAGS) -DHOST demo/main.c platform/zx_platform.c -o knightlore-demo

zx: demo/main.c platform/zx_platform.c platform/zx_platform.h
	@echo "Building for ZX with z88dk..."
	$(ZCC) +zx -vn -m -create-app demo/main.c platform/zx_platform.c -o knightlore
	@echo "Build complete. Output: knightlore.z80 (or nearby z88dk output files)"

clean:
	rm -f knightlore-demo knightlore knightlore.z80 knightlore.rom
