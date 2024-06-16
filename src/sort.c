#include "sort.h"
#include <string.h>
#include <stdlib.h>

int bubble_sort(char** values, size_t length) {
    int comparison_count = 0;

    for ( size_t i=0; i < length-1; i++ ) {
        for ( size_t j=i+1; j < length; j++ ) {
            int comparison = strcmp(values[i], values[j]);
            comparison_count++;
            if ( comparison > 0 ) {
                char* swap = values[i];
                values[i] = values[j];
                values[j] = swap;
            }
        }
    }

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

int quick_sort(char** values, size_t length) {
    return 0;
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
