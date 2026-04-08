#include <stddef.h>
#include <stdlib.h>
#include "string_list.h"

StringList* init_string_list(size_t size) {
  StringList* string_list = malloc(sizeof(StringList));

  string_list->strings = malloc(size * sizeof(char*));

  string_list->count = 0;

  string_list->size = size;

  return string_list;
}

void string_list_append(StringList* string_list, char* string) {
  if (string_list->count + 1 > string_list->size) {
    string_list->strings = realloc(string_list->strings, string_list->size * 2);
    string_list->size = string_list->size * 2;
  }

  string_list->strings[string_list->count] = string;
  string_list->count++;
}
