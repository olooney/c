CC := gcc
CFLAGS := -g -O3 -Wall -Wextra -Wno-unused-parameter -Wno-stringop-truncation
BINDIR := bin
BUILDDIR := build
TESTDIR := test
SRCDIR := src

# Ensure target directories exist
$(shell mkdir -p $(BINDIR) $(BUILDDIR) $(TESTDIR) $(SRCDIR))

all: test

include dependencies.makefile

.PHONY: all clean test dep serve

clean:
	rm -f $(BINDIR)/*
	rm -f $(BUILDDIR)/*.o
	rm -f $(TESTDIR)/*
	rm -f dependencies.makefile
	gcc -M $(SRCDIR)/*.c > dependencies.makefile

dep:
	gcc -M $(SRCDIR)/*.c > dependencies.makefile

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/%: $(BUILDDIR)/%.o
	$(CC) $(CFLAGS) $^ -o $@

$(BINDIR)/server: $(BUILDDIR)/server.o
	$(CC) $(CFLAGS) $< -o $@

$(BINDIR)/main: $(BUILDDIR)/main.o $(BUILDDIR)/rabin_karp.o

$(BINDIR)/test_list: $(BUILDDIR)/test_list.o $(BUILDDIR)/list.o
$(BINDIR)/test_dict: $(BUILDDIR)/test_dict.o $(BUILDDIR)/dict.o
$(BINDIR)/test_token: $(BUILDDIR)/test_token.o $(BUILDDIR)/token.o
$(BINDIR)/test_skew_heap: $(BUILDDIR)/test_skew_heap.o $(BUILDDIR)/skew_heap.o
$(BINDIR)/test_rabin_karp: $(BUILDDIR)/test_rabin_karp.o $(BUILDDIR)/rabin_karp.o
$(BINDIR)/test_sort: $(BUILDDIR)/test_sort.o $(BUILDDIR)/sort.o

test: $(BINDIR)/test_list $(BINDIR)/test_dict $(BINDIR)/test_rabin_karp
	@rm -f $(TESTDIR)/test_results.txt
	@$(BINDIR)/test_list >> $(TESTDIR)/test_results.txt
	@$(BINDIR)/test_dict >> $(TESTDIR)/test_results.txt
	@$(BINDIR)/test_rabin_karp >> $(TESTDIR)/rabin_karp_test_results.txt
	@echo "Pass Count:" $$(grep '^PASS:' $(TESTDIR)/test_results.txt | wc -l)
	@echo "Fail Count:" $$(grep '^FAIL:' $(TESTDIR)/test_results.txt | wc -l)

serve: $(BINDIR)/server
	@$(BINDIR)/server
