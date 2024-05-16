CFLAGS := -g -Wall -Wextra -Wno-unused-parameter

SOURCES := server.c
BINARY := server


.PHONEY: all clean test
all: server

clean:
	@rm -rf $(BINARY)

test: server
	./server

server: $(SOURCES)
	gcc $(CFLAGS) $< -o $@


