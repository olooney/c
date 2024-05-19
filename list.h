#ifndef LIST_H
#define LIST_H

#include <stdio.h>  // For FILE*

// Define the structure for a linked list node
typedef struct List List;
struct List {
    char* value;
    List* next;
};

// Function declarations
List* new_list(char* value);
void list_append(List* list, char* value);
void free_list(List* list);
void fprint_list(FILE* stream, List* list, char* separator);

#endif // LIST_H

