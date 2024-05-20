#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

void test_list() {
    // Test 1: Create a new list with initial value
    printf("Test 1: new_list()\n");
    List* test_list = new_list("First");
    if (test_list != NULL && strcmp(test_list->value, "First") == 0) {
        printf("PASS: List created with initial value '%s'\n", test_list->value);
    } else {
        printf("FAIL: List creation failed\n");
    }

    // Test 2: Append items to the list
    printf("Test 2: list_append()\n");
    list_append(test_list, "Second");
    list_append(test_list, "Third");
    if (test_list->next != NULL && strcmp(test_list->next->value, "Second") == 0 &&
        test_list->next->next != NULL && strcmp(test_list->next->next->value, "Third") == 0) {
        printf("PASS: Items appended correctly\n");
    } else {
        printf("FAIL: Items not appended correctly\n");
    }

    // append n
    printf("Test 3: list_append_n()...\n");
    const char* test_string = "Four Five Six";
    list_append_n(test_list, test_string, 4);
    list_append_n(test_list, test_string + 5, 4);
    list_append_n(test_list, test_string + 10, 100);
    const char* answers[6] = { "First", "Second", "Third", "Four", "Five", "Six" };
    for ( int i=0; i < 6; i++ ) {
        char* ith_value = list_get(test_list, i);
        if ( strcmp(ith_value, answers[i]) == 0 ) {
            printf("PASS: got item %d correctly\n", i);
        } else {
            printf("FAIL: item %d: %s != %s\n", i, ith_value, answers[i]);
        }
    }

    // get last
    char* last_value = list_get(test_list, -1);
    if ( strcmp(last_value, "Six") == 0 ) {
        printf("PASS: got last item correctly\n");
    } else {
        printf("FAIL: last item: %s != %s\n", last_value, "Six");
    }

    // invalid get
    char* bad_value = list_get(test_list, 99);
    if ( bad_value == NULL ) {
        printf("PASS: getting out-of-range index returns NULL\n");
    } else {
        printf("FAIL: getting out-of-range index returns non-NULL\n");
    }


    // Test 3: Print the list
    printf("Test 3: Printing the list...\n");
    printf("Expected output: First, Second, Third, Four, Five, Six\n");
    printf("Actual output  : ");
    fprint_list(stdout, test_list, ", ");  // Using ", " as separator and no newline for continuous output
    printf("\n");

    // Test 4: Free the list and check for no access to freed memory (manually verify no crashes/errors)
    printf("Test 4: Freeing the list...\n");
    free_list(test_list);
    printf("List freed. (Manual check required for no crashes due to use-after-free errors)\n");
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