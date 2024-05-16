CFLAGS := -g -Wall -Wextra -Wno-unused-parameter

SOURCES := main.c
BINARY := server


.PHONEY: all clean test
all: server

clean:
	@rm -rf $(BINARY)

test: server
	./server

server: main.c
	gcc $(CFLAGS) $< -o $@


