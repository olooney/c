#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include <stdbool.h>

int bubble_sort(char** values, size_t length);
int merge_sort(char** values, size_t length);
int quick_sort(char** values, size_t length);
bool check_sorted(char** values, size_t length);

#endif // SORT_H
