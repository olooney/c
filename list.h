#ifndef LIST_H
#define LIST_H

#include <stdio.h>

// Define the structure for a linked list node
typedef struct List List;
struct List {
    char* value;
    List* next;
};

// Function declarations
List* new_list(const char* value);
List* new_list_n(const char* value, size_t length);
void free_list(List* list);

void list_append(List* list, const char* value);
void list_append_n(List* list, const char* value, size_t length);
char* list_get(List* list, int index);
void fprint_list(FILE* stream, List* list, const char* separator);


#endif // LIST_H

