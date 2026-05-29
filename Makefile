CFLAGS = -std=c23 -pedantic -Wall -Wextra -Wformat-security -Wduplicated-cond \
-Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init \
-Wlogical-not-parentheses -Wnull-dereference -Wvla -Werror \
-fstack-protector-strong -fsanitize=undefined -fno-sanitize-recover \
-fno-omit-frame-pointer -O1 -g
CC = gcc

.PHONY: all, clean

all: polynomials

polynomials: polynomials.c
	$(CC) $< -o $@ $(CFLAGS)

clean:
	rm polynomials
