#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sort.h"

void assert_sorted(char** values, size_t length, const char* name) {
    bool is_sorted = check_sorted(values, length);
    if ( is_sorted ) {
        printf("PASS: values sorted after %s\n", name);
    } else {
        printf("FAIL: values not sorted after %s\n", name);
    }
}

void test_bubble_sort() {
    char* values[] = { "cat", "dog", "parakeet", "iguana", "rock" };
    size_t length = 5;
    bubble_sort(values, length);
    assert_sorted(values, length, "bubble_sort()");
}


void test_merge_sort() {
    char* values[] = { "cat", "dog", "parakeet", "iguana", "rock" };
    size_t length = 5;
    merge_sort(values, length);
    assert_sorted(values, length, "merge_sort()");
}


void test_quick_sort() {
    char* values[] = { "cat", "dog", "parakeet", "iguana", "rock" };
    size_t length = 5;
    quick_sort(values, length);
    assert_sorted(values, length, "quick_sort()");
}

int main(int argc, char** argv) {
	test_bubble_sort();
	test_merge_sort();
	test_quick_sort();
    return 0;
}
