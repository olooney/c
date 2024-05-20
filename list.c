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
    list->value[length] = '\0';
    list->next = NULL;
    return list;
}

void free_list(List* list) {
    while ( list != NULL ) {
        List *temp = list;
        list = list->next;
        free(temp->value);
        free(temp);
    }
}

List* list_append(List* list, const char* value) {
    if ( list == NULL ) {
        return new_list(value);
    }

    while ( list->next != NULL ) {
        list = list->next;
    }
    list->next = new_list(value);
    return list;
}

List* list_append_n(List* list, const char* value, size_t length) {
    if ( list == NULL ) {
        return new_list_n(value, length);
    }

    while ( list->next != NULL ) {
        list = list->next;
    }
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

int list_length(List* list) {
    if ( list == NULL ) return 0;

    int length = 1;
    while ( list->next != NULL ) {
        length++;
        list = list->next;
    }

    return length;
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

