CC := gcc
CFLAGS := -g -Wall -Wextra -Wno-unused-parameter

all: main server

include dependencies.makefile

.PHONY: all clean test dep

clean:
	@rm -f main server *.o dependencies.makefile
	gcc -M *.c > dependencies.makefile

dep:
	gcc -M *.c > dependencies.makefile

test: server
	./server

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

server: server.o
	$(CC) $(CFLAGS) $< -o $@

main: main.o rabin_karp.o
	$(CC) $(CFLAGS) $^ -o $@
