#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"

void generate_random_string(char* string, size_t length) {
    const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for ( size_t i=0; i<length; i++ ) {
        size_t char_index = rand() % (sizeof(characters) - 1);
        string[i] = characters[char_index];
    }
    string[length] = '\0';
}

char** generate_random_strings(int num_strings, int string_length) {
    char* block = (char*)malloc(num_strings * (string_length + 1) * sizeof(char));
    char** strings = (char**)malloc(num_strings * sizeof(char*));

    srand(time(NULL));
    for (int i = 0; i < num_strings; i++) {
        strings[i] = block + i * (string_length + 1);
        generate_random_string(strings[i], string_length);
    }

    return strings;
}

void assert_sorted(
    char** values, 
    size_t length,
    const char* name,
    int comparison_count
) {
    bool is_sorted = check_sorted(values, length);
    if ( is_sorted ) {
        printf("PASS: %zu values sorted by %s after %d comparisons\n", length, name, comparison_count);
    } else {
        printf("FAIL: %zu values *not* sorted by %s after %d comparisons\n", length, name, comparison_count);
    }
}

void test_sort(sort_func_t sort_function, const char* function_name) {
    int comparison_count;
    // test on an empty array.
    char* empty_values[] = { };
    comparison_count = sort_function(empty_values, 0);
    assert_sorted(empty_values, 0, function_name, comparison_count);

    // test on exactly 1 value.
    char* short_values[] = { "thing" };
    comparison_count = sort_function(short_values, 1);
    assert_sorted(short_values, 1, function_name, comparison_count);

    // test on exactly 2 values.
    char* two_values[] = { "thing2", "thing1" };
    comparison_count = sort_function(two_values, 2);
    assert_sorted(two_values, 2, function_name, comparison_count);

    // test on a fixed set of 10 strings
    char* values[] = { 
        "cat", "dog", "parakeet", "iguana", "rock",
        "fish", "alpaca", "monster", "hamster", "pig"
    };
    size_t length = 10;
    comparison_count = sort_function(values, length);
    assert_sorted(values, length, function_name, comparison_count);

    // test on a large number of random strings.
    length = 4096;
    char** many_values = generate_random_strings(length, 8);
    char* string_buffer= many_values[0];
    comparison_count = sort_function(many_values, length);
    assert_sorted(many_values, length, function_name, comparison_count);
    free(string_buffer);
    free(many_values);
}

int main(int argc, char** argv) {
	test_sort(brute_sort, "brute_sort()");
	test_sort(bubble_sort, "bubble_sort()");
	test_sort(merge_sort, "merge_sort()");
	test_sort(quick_sort, "quick_sort()");
    return 0;
}
