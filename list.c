#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

List* new_list(const char* value) {
    return new_list_n(value, strlen(value));
}

List* new_list_n(const char* value, size_t length) {
    List* list = (List*) malloc(sizeof(List));
    list->value = (char*) malloc(length + 1);
    strncpy(list->value, value, length);
    list->next = NULL;
    return list;
}

void free_list(List* list) {
    if (list) {
        free_list(list->next);
        free(list->value);
        free(list);
    }
}

void list_append(List* list, const char* value) {
    while (list->next) list = list->next;
    list->next = new_list(value);
}

void list_append_n(List* list, const char* value, size_t length) {
    while (list->next) list = list->next;
    list->next = new_list_n(value, length);
}

// pass index -1 as a special case to get the last value.
char* list_get(List* list, int index) {
    if ( list == NULL ) return NULL;

    if ( index == -1 ) {
        while ( list->next != NULL ) {
            list = list->next;
        }
        return list->value;
    }

    for ( int i=0; i < index; i++ ) {
        list = list->next;
        if ( list == NULL ) return NULL;
    }

    return list->value;
}


void fprint_list(FILE* stream, List* list, const char* separator) {
    if (list) {
        fputs(list->value, stream);
        if (list->next) {
            fputs(separator, stream);
            fprint_list(stream, list->next, separator);
        }
    }
}

