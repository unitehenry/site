#ifndef STRINGS_H
#define STRINGS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  size_t size;
  size_t count;
  char **strings;
} StringList;

typedef struct StringMapEntry {
  char *key;
  char *value;
  struct StringMapEntry *next;
} StringMapEntry;

typedef struct {
  StringMapEntry *head;
} StringMap;

StringList *init_string_list(size_t size);
void string_list_append(StringList *string_list, char *string);
char *str_replace(const char *str, const char *old_sub, const char *new_sub);
char *trim_whitespace(const char *str);
StringMap *string_map_create(void);
void string_map_put(StringMap *map, char *key, char *value);
char *string_map_get(StringMap *map, char *key);
bool string_map_contains(StringMap *map, char *key);

#endif
