#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

void test_list() {
    // new
    printf("\nTest new_list()\n");
    List* list = new_list("First");
    if (list != NULL && strcmp(list->value, "First") == 0) {
        printf("PASS: List created with initial value '%s'\n", list->value);
    } else {
        printf("FAIL: List creation failed\n");
    }

    // length
    printf("\nTest list_length()\n");
    if ( 
        list_length(NULL) == 0 &&
        list_length(list) == 1
    ) {
        printf("PASS: list_length() for 0 or 1\n");
    } else {
        printf("FAIL: list_length() for 0 or 1\n");
    }


    // append 
    printf("\nTest list_append()\n");
    list_append(list, "Second");
    list_append(list, "Third");
    if (
        list->next != NULL && 
        strcmp(list->next->value, "Second") == 0 &&
        list->next->next != NULL && 
        strcmp(list->next->next->value, "Third") == 0
    ) {
        printf("PASS: Items appended correctly\n");
    } else {
        printf("FAIL: Items not appended correctly\n");
    }

    // append n
    printf("\nTest list_append_n()...\n");
    const char* test_string = "Four Five Six";
    list_append_n(list, test_string, 4);
    list_append_n(list, test_string + 5, 4);
    list_append_n(list, test_string + 10, 100);
    const char* answers[6] = { "First", "Second", "Third", "Four", "Five", "Six" };
    if ( list_length(list) == 6 ) {
        printf("PASS: string slices appended correctly\n");
    } else {
        printf("FAIL: List length %d != 6\n", list_length(list));
    }


    // get
    for ( int i=0; i < 6; i++ ) {
        char* ith_value = list_get(list, i);
        if ( strcmp(ith_value, answers[i]) == 0 ) {
            printf("PASS: got item %d correctly\n", i);
        } else {
            printf("FAIL: item %d: %s != %s\n", i, ith_value, answers[i]);
        }
    }

    // get last
    printf("\nTest list_get() last item...\n");
    char* last_value = list_get(list, -1);
    if ( strcmp(last_value, "Six") == 0 ) {
        printf("PASS: got last item correctly\n");
    } else {
        printf("FAIL: last item: %s != %s\n", last_value, "Six");
    }

    // invalid get
    printf("\nTest list_get() out-of-range...\n");
    char* bad_value = list_get(list, 99);
    if ( bad_value == NULL ) {
        printf("PASS: getting out-of-range index returns NULL\n");
    } else {
        printf("FAIL: getting out-of-range index returns non-NULL\n");
    }

    // print
    printf("\nTest fprint_list()...\n");
    printf("EXPECT: First, Second, Third, Four, Five, Six\n");
    printf("ACTUAL: ");
    fprint_list(stdout, list, ", ");
    printf("\n");

    // free
    printf("\nTest free_list()...\n");
    free_list(list);
    printf("PASS: List freed.\n");
}

void test_list_argv(int argc, char** argv) {
    List* args = new_list(argv[0]);
    
    for (int i = 1; i < argc; i++) {
        list_append(args, argv[i]);
    }

    fprint_list(stdout, args, ",\n");
    fputs("\n", stdout);

    free_list(args);
}

int main(int argc, char** argv) {
	test_list_argv(argc, argv);
	test_list();
    return 0;
}
