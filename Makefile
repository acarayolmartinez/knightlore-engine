# Simple Makefile for host development and z88dk

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -I.
ZCC = zcc

.PHONY: all host zx tap clean

all: host

host: demo/main.c platform/zx_platform.c platform/zx_platform.h
	$(CC) $(CFLAGS) -DHOST demo/main.c platform/zx_platform.c -o knightlore-demo

zx: demo/main.c platform/zx_platform.c platform/zx_platform.h
	@echo "Building for ZX with z88dk..."
	$(ZCC) +zx -vn -create-app -o knightlore demo/main.c platform/zx_platform.c
	@echo "Build complete. Output may include .tap/.z80/.bin depending on z88dk setup"

tap: demo/main.c platform/zx_platform.c platform/zx_platform.h
	@echo "Building TAP output for ZX Spectrum..."
	$(ZCC) +zx -vn -create-app -o knightlore demo/main.c platform/zx_platform.c
	@echo "Build complete. Check for .tap output in the current directory"

clean:
	rm -f knightlore-demo knightlore knightlore.tap knightlore.bin knightlore.z80
