all: main

CC = gcc
override CFLAGS += -Wall -Wextra -Wpedantic
override CFLAGS += -std=c99 -Iinclude

SRCS = $(shell find src/ -type f -name '*.c' -print)
HDRS = $(shell find include/ -type f -name '*.h' -print)

main: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o interpreter

clean:
	rm -f interpreter

