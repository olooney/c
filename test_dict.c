#include <stdio.h>
// #define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "dict.h"

void test_dict() { 
    // new
    printf("\nTest new_dict()\n");
    Dict* people = new_dict();
    printf("PASS: empty Dict created\n");

    // set
    printf("\nTest dict_set()\n");
    if ( 
        dict_set(people, "Oran", 37LU) &&
        dict_set(people, "Jonnie", 42LU)
    ) { 
        printf("PASS: Dict populated with two items.\n");
    } else {
        printf("FAIL: Dict item not set.\n");
    }

    // get
    printf("\nTest dict_get()\n");
    uint64_t value = 0;
    bool success;

    success = dict_get(people, "Oran", &value);
    if ( success ) {
        if ( value == 37LU ) {
            printf("PASS: Dict item Oran is 37.\n");
        } else {
            printf("FAIL: Dict item Oran is %" PRIu64 " != 37.\n", value);
        }
    } else {
        printf("FAIL: Dict item Oran not found.\n");
    }

    success = dict_get(people, "Jonnie", &value);
    if ( success ) {
        if ( value == 42LU ) {
            printf("PASS: Dict item Jonnie is 42.\n");
        } else {
            printf("FAIL: Dict item Jonnie is %" PRIu64 " != 42.\n", value);
        }
    } else {
        printf("FAIL: Dict item Jonnie not found.\n");
    }

    // free
    printf("\nTest free_dict()...\n");
    free_dict(people);
    printf("PASS: Dict freed.\n");
}

int main(int argc, char* argv[]) {
    test_dict();    
}
