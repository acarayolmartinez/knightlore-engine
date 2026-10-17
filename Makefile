# Simple Makefile for host development

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -I.

.PHONY: all host clean

all: host

host: demo/main.c platform/zx_platform.c platform/zx_platform.h
	$(CC) $(CFLAGS) -DHOST demo/main.c platform/zx_platform.c -o knightlore-demo

clean:
	rm -f knightlore-demo
