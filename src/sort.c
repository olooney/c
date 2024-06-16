#include "sort.h"
#include <string.h>
#include <stdlib.h>

void swap(char** values, size_t i, size_t j) {
    char* temp_value = values[i];
    values[i] = values[j];
    values[j] = temp_value;
}

int brute_sort(char** values, size_t length) {
    int comparison_count = 0;

    for ( size_t i=0; i+1 < length; i++ ) {
        for ( size_t j=i+1; j < length; j++ ) {
            int comparison = strcmp(values[i], values[j]);
            comparison_count++;
            if ( comparison > 0 ) {
                swap(values, i, j);
            }
        }
    }

    return comparison_count;
}

int bubble_sort(char** values, size_t length) {
    int comparison_count = 0;
    bool swap_performed;

    // Scan through the array swapping any adjacent pair that are out-of-order.
    // Keep track of whether or not any swaps were performed; if we can get
    // through the entire array without performing any swaps we can stop as
    // we have now verified that the array is sorted.
    do {
        swap_performed = false;

        for ( size_t i=1; i< length; i++ ) {
            // compare adjacent elements
            int comparison = strcmp(values[i-1], values[i]);
            comparison_count++;

            // swap if out-of-order
            if ( comparison > 0 ) {
                swap(values, i-1, i);
                swap_performed = true;
            }
        }
    } while ( swap_performed );

    return comparison_count;
}

int merge_sort(char** values, size_t length) {
    if ( length < 2 ) return 0;

    int comparison_count = 0;
    size_t left_half_length = length / 2;
    size_t right_half_length = length - left_half_length;
    comparison_count += merge_sort(values, left_half_length);
    comparison_count += merge_sort(values + left_half_length, right_half_length);

    char** output = (char**)malloc(sizeof(char*) * length);
    
    char** left_half = values;
    char** right_half = values + left_half_length;

    size_t i = 0; // read index for left side
    size_t j = 0;     // read index for right side
    size_t k = 0;     // write index in output
    while ( i < left_half_length || j < right_half_length ) {
        
        // if one side is empty, take the other. Otherwise,
        // take the lesser head.
        bool take_left;
        if ( i == left_half_length ) {
            take_left = false;
        } else if ( j == right_half_length ) {
            take_left = true;
        } else {
            take_left = ( strcmp(left_half[i], right_half[j]) < 0 );
            comparison_count++;
        }

        // take the value from the appropriate side and advance all
        // the read/write indices.
        if ( take_left ) {
            output[k] = values[i];
            i++;
        } else {
            output[k] = values[j];
            j++;
        }
        k++;
    }

    // copy the output back onto values and free it.
    for ( i=0; i<length; i++ ) {
        values[i] = output[i];
    }
    free(output);

    return comparison_count;
}

size_t partition(char** values, size_t length, int* comparison_count) {
    if ( length <= 1 ) return 0;

    size_t pivot_index = rand() % length;
    char* pivot_value = values[pivot_index];

    swap(values, pivot_index, length-1);
    size_t current_pivot = 0;
    for ( size_t i=0; i < length-1; i++ ) {
        (*comparison_count)++;
        if ( strcmp(values[i], pivot_value) < 0 ) {
            swap(values, i, current_pivot);
            current_pivot++;
        }
    }
    swap(values, current_pivot, length-1);
    return current_pivot;
    
}

int quick_sort(char** values, size_t length) {
    if ( length < 2 ) return 0;

    int comparison_count = 0;
    size_t pivot = partition(values, length, &comparison_count);

    comparison_count += quick_sort(values, pivot);

    if ( pivot + 1 < length ) {
        comparison_count += quick_sort(values + pivot + 1, length - pivot - 1);
    }
    
    return comparison_count;
}


bool check_sorted(char** values, size_t length) {
    if ( length < 2 ) return true;

    for ( size_t i=1; i<length; i++ ) {
        int comparison = strcmp(values[i-1], values[i]);
        if ( comparison > 0 ) {
            return false;
        }
    }
    return true;
}
