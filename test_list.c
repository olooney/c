#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

void test_list() {
    // Test 1: Create a new list with initial value
    printf("Test 1: Creating a new list...\n");
    List* testList = new_list("First");
    if (testList != NULL && strcmp(testList->value, "First") == 0) {
        printf("PASS: List created with initial value '%s'\n", testList->value);
    } else {
        printf("FAIL: List creation failed\n");
    }

    // Test 2: Append items to the list
    printf("Test 2: Appending items to the list...\n");
    list_append(testList, "Second");
    list_append(testList, "Third");
    if (testList->next != NULL && strcmp(testList->next->value, "Second") == 0 &&
        testList->next->next != NULL && strcmp(testList->next->next->value, "Third") == 0) {
        printf("PASS: Items appended correctly\n");
    } else {
        printf("FAIL: Items not appended correctly\n");
    }

    // Test 3: Print the list
    printf("Test 3: Printing the list...\n");
    printf("Expected output: First, Second, Third\n");
    printf("Actual output: ");
    fprint_list(stdout, testList, ", ");  // Using ", " as separator and no newline for continuous output
    printf("\n");

    // Test 4: Free the list and check for no access to freed memory (manually verify no crashes/errors)
    printf("Test 4: Freeing the list...\n");
    free_list(testList);
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
