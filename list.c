#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

List* new_list(char* value) {
    List* list = (List*) malloc(sizeof(List));
    list->value = (char*) malloc(strlen(value) + 1);
    strcpy(list->value, value);
    list->next = NULL;
    return list;
}

void list_append(List* list, char* value) {
    while (list->next) list = list->next;
    list->next = new_list(value);
}

void free_list(List* list) {
    if (list) {
        free_list(list->next);
        free(list->value);
        free(list);
    }
}

void fprint_list(FILE* stream, List* list, char* separator) {
    if (list) {
        fputs(list->value, stream);
        if (list->next) {
            fputs(separator, stream);
            fprint_list(stream, list->next, separator);
        }
    }
}

