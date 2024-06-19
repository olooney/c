#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"


void print_values(char** values, size_t length) {
    printf("[");
    if ( length > 0 ) {
        printf("\"%s\"", values[0]);
    }
    for ( size_t i=1; i<length; i++ ) {
        printf(", \"%s\"", values[i]);
    }
    printf("]\n");
}


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
    int comparison_count,
    int verbose
) {
    bool is_sorted = check_sorted(values, length);
    if ( is_sorted ) {
        if ( verbose >= 2 ) {
            printf("PASS: %zu values sorted by %s after %d comparisons\n", length, name, comparison_count);
        }
    } else {
        if ( verbose >= 1 ) {
            printf("FAIL: %zu values *not* sorted by %s after %d comparisons\n", length, name, comparison_count);
        }
        if ( verbose >= 2 ) {
            print_values(values, length);
        }
    }
}

void test_sort(sort_func_t sort_function, const char* function_name, int num_epochs) {
    printf("TEST: %s\n", function_name);

    int comparison_count;
    // test on an empty array.
    char* empty_values[] = { };
    comparison_count = sort_function(empty_values, 0);
    assert_sorted(empty_values, 0, function_name, comparison_count, 2);

    // test on exactly 1 value.
    char* short_values[] = { "thing" };
    comparison_count = sort_function(short_values, 1);
    assert_sorted(short_values, 1, function_name, comparison_count, 2);

    // test on exactly 2 values.
    char* two_values[] = { "thing2", "thing1" };
    comparison_count = sort_function(two_values, 2);
    assert_sorted(two_values, 2, function_name, comparison_count, 2);

    // test on a fixed set of 10 strings
    char* values[] = { 
        "cat", "dog", "parakeet", "iguana", "rock",
        "fish", "alpaca", "monster", "hamster", "pig"
    };
    size_t length = 10;
    comparison_count = sort_function(values, length);
    assert_sorted(values, length, function_name, comparison_count, 2);

    // test on a large number of random strings.
    length = 1024;
    clock_t start_time = clock();
    for ( int epoch=0; epoch<num_epochs; epoch++ ) {
        // generate random strings
        char** many_values = generate_random_strings(length, 8);
        char* string_buffer = many_values[0];
        comparison_count = sort_function(many_values, length);

        int verbose = (epoch >= 3 ? 1 : 2);
        assert_sorted(many_values, length, function_name, comparison_count, verbose);

        // clean up random strings.
        free(string_buffer);
        free(many_values);
    }
    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    double milliseconds_per_epoch = 1e6 * duration / (double)num_epochs;
    printf("Time spent in %s: %0.3f ms\n\n", function_name, milliseconds_per_epoch);
}

void test_binary_search() { 
    printf("TEST: binary_search()\n");

    char* empty_values[] = {};
    char** found = binary_search(empty_values, 0, "key", true);
    if ( found == NULL ) {
        printf("PASS: exact binary_search() returned NULL for non-existant key \"key\" in empty array.\n");
    } else {
        size_t index = found - empty_values;
        printf("FAIL: exact binary_search() returned index %zu for key \"key\" in empty array.\n", index);
    }

    found = binary_search(empty_values, 0, "key", false);
    if ( found == NULL ) {
        printf("PASS: non-exact binary_search() returned NULL for non-existant key \"key\" in empty array.\n");
    } else {
        size_t index = found - empty_values;
        printf("FAIL: non-exact binary_search() returned index %zu for key \"key\" in empty array.\n", index);
    }


    char* pets[] = { 
        "alpaca", "cat", "dog", "fish", "hamster", 
        "iguana", "monster", "parakeet", "pig", "rock"
    };

    // exact search, key exists
    char** dog = binary_search(pets, 10, "dog", true);
    if ( dog - pets == 2 ) {
        printf("PASS: exact binary_search() found \"dog\" at position 2.\n");
    } else {
        printf("FAIL: exact binary_search() failed to find \"dog\". ");
        if ( dog == NULL ) {
            printf("Returned NULL instead.\n");
        } else {
            size_t index = dog - pets;
            printf("Returned index %zu instead.\n", index);
        }
    }

    // exact search, key does not exist
    char** doggo = binary_search(pets, 10, "doggo", true);
    if ( doggo == NULL ) {
        printf("PASS: exact binary_search() returned NULL for non-existant key \"doggo\".\n");
    } else {
        size_t index = doggo - pets;
        printf("FAIL: exact binary_search() returned index %zu for non-existant key \"doggo\".\n", index);
    }

    // exact search, key does not exist, after last element
    char** zebra = binary_search(pets, 10, "zebra", true);
    if ( zebra == NULL ) {
        printf("PASS: exact binary_search() returned NULL for non-existant key \"zebra\".\n");
    } else {
        size_t index = zebra - pets;
        printf("FAIL: exact binary_search() returned index %zu for non-existant key \"zebra\".\n", index);
    }

    // exact search, key does not exist, before first element
    char** aardvark = binary_search(pets, 10, "aardvark", true);
    if ( aardvark == NULL ) {
        printf("PASS: exact binary_search() returned NULL for non-existant key \"aardvark\".\n");
    } else {
        size_t index = aardvark - pets;
        printf("FAIL: exact binary_search() returned index %zu for non-existant key \"aardvark\".\n", index);
    }

    // non-exact search, key exists
    char** fish = binary_search(pets, 10, "fish", false);
    if ( fish - pets == 3 ) {
        printf("PASS: non-exact binary_search() found \"fish\" at position 3.\n");
    } else {
        printf("FAIL: non-exact binary_search() failed to find \"fish\". ");
        if ( fish == NULL ) {
            printf("Returned NULL instead.\n");
        } else {
            size_t index = fish - pets;
            printf("Returned index %zu instead.\n", index);
        }
    }

    // non-exact search, key does not exists, insert after
    char** elephant = binary_search(pets, 10, "elephant", false);
    if ( elephant - pets == 3 ) {
        printf("PASS: non-exact binary_search() suggested inserting \"elephant\" at position 3.\n");
    } else {
        printf("FAIL: non-exact binary_search() failed to find insert position for \"elephant\". ");
        if ( elephant == NULL ) {
            printf("Returned NULL instead.\n");
        } else {
            size_t index = elephant - pets;
            printf("Returned index %zu instead.\n", index);
        }
    }

    // non-exact search, key does not exists, insert before
    aardvark = binary_search(pets, 10, "aardvark", false);
    if ( aardvark - pets == 0 ) {
        printf("PASS: non-exact binary_search() suggested inserting \"aardvark\" at position 0.\n");
    } else {
        printf("FAIL: non-exact binary_search() failed to find insert position for \"aardvark\". ");
        if ( aardvark == NULL ) {
            printf("Returned NULL instead.\n");
        } else {
            size_t index = aardvark - pets;
            printf("Returned index %zu instead.\n", index);
        }
    }

    // non-exact search, key does not exists, insert at end
    zebra = binary_search(pets, 10, "zebra", false);
    if ( zebra - pets == 10 ) {
        printf("PASS: non-exact binary_search() suggested inserting \"zebra\" at position 10.\n");
    } else {
        printf("FAIL: non-exact binary_search() failed to find insert position for \"zebra\". ");
        if ( zebra == NULL ) {
            printf("Returned NULL instead.\n");
        } else {
            size_t index = zebra - pets;
            printf("Returned index %zu instead.\n", index);
        }
    }

    // generate random data
    size_t length = 1024;
    char** many_values = generate_random_strings(length, 8);
    char* string_buffer = many_values[0];
    quick_sort(many_values, length);

    char key[9];
    int failures = 0;
    for ( size_t i=0; i<length; i++ ) {
        char** key_pointer = many_values + i;
        strcpy(key, *key_pointer);
        char** found = binary_search(many_values, length, key, true);
        if ( found != key_pointer ) {
            size_t true_index = key_pointer - many_values;
            printf("FAIL: exact binary_search() failed to find %s at index %zu. ", key, true_index);
            failures++;
            if ( found == NULL ) {
                printf("Returned NULL instead.\n");
            } else {
                size_t found_index = found - many_values;
                printf("Returned index %zu instead.\n", found_index);
            }
        }
    }
    if ( failures == 0 ) {
        printf("PASS: exact binary_search() found correct index for all %zu values.\n", length);
    }

    // clean up random strings.
    free(string_buffer);
    free(many_values);
}


int main(int argc, char** argv) {
    srand(time(NULL));
	test_sort(brute_sort, "brute_sort()", 8);
	test_sort(bubble_sort, "bubble_sort()", 8);
	test_sort(merge_sort, "merge_sort()", 1024);
	test_sort(quick_sort, "quick_sort()", 1024);
    test_binary_search();

    return 0;
}
