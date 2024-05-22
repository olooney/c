CC := gcc
CFLAGS := -g -Wall -Wextra -Wno-unused-parameter

all: test

include dependencies.makefile

.PHONY: all clean test dep serve

clean:
	rm -f test_list test_dict test_rabin_karp test_results.txt
	rm -f main server *.o
	rm -f dependencies.makefile
	gcc -M *.c > dependencies.makefile

dep:
	gcc -M *.c > dependencies.makefile

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.o
	$(CC) $(CFLAGS) $^ -o $@

server: server.o
	$(CC) $(CFLAGS) $< -o $@

main: main.o rabin_karp.o

test_list: test_list.o list.o
test_dict: test_dict.o dict.o
test_rabin_karp: test_rabin_karp.o rabin_karp.o

test: test_list test_dict test_rabin_karp
	@rm -f test_results.txt
	@./test_list >> test_results.txt
	@./test_dict >> test_results.txt
	@./test_rabin_karp >> test_results.txt
	@./test_rabin_karp water ../data/brothers_karamazov.txt >> test_results.txt
	@echo "Pass Count:" $$(grep '^PASS:' test_results.txt | wc -l)
	@echo "Fail Count:" $$(grep '^FAIL:' test_results.txt | wc -l)

serve: ./server
	./server
