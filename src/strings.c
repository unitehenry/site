#include "strings.h"
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

StringList *init_string_list(size_t size) {
  StringList *string_list = malloc(sizeof(StringList));

  string_list->strings = malloc((size * sizeof(char *)));

  string_list->count = 0;

  string_list->size = size;

  if (size > 0) {
    string_list->strings[0] = NULL;
  }

  return string_list;
}

void string_list_append(StringList *string_list, char *string) {
  if (string_list->count + 1 > string_list->size) {
    string_list->strings =
        realloc(string_list->strings, (string_list->size * 2) * sizeof(char *));
    string_list->size = string_list->size * 2;
  }

  size_t len = strlen(string) + 1;
  char *str = malloc(len);

  strcpy(str, string);

  string_list->strings[string_list->count] = str;
  string_list->count++;
  string_list->strings[string_list->count] = NULL;
}

char *str_replace(const char *str, const char *old_sub, const char *new_sub) {
  size_t old_len = strlen(old_sub);

  if (old_len == 0) {
    return strdup(str);
  }

  size_t new_len = strlen(new_sub);

  size_t str_len = strlen(str);

  size_t count = 0;

  const char *p = str;

  while ((p = strstr(p, old_sub)) != NULL) {
    count++;
    p += old_len;
  }

  if (count == 0) {
    return strdup(str);
  }

  size_t new_str_len = str_len + count * (new_len - old_len) + 1;

  char *result = malloc(new_str_len);

  if (!result) {
    return NULL;
  }

  char *dest = result;

  const char *src = str;

  while (*src) {
    if (strstr(src, old_sub) == src) {
      strcpy(dest, new_sub);

      dest += new_len;

      src += old_len;
    } else {
      *dest++ = *src++;
    }
  }

  *dest = '\0';

  return result;
}

char *trim_whitespace(const char *str) {
  if (!str)
    return NULL;

  size_t len = strlen(str);

  if (len == 0)
    return strdup("");

  size_t start = 0;

  while (start < len && isspace(str[start]))
    start++;

  if (start == len)
    return strdup("");

  size_t end = len - 1;

  while (end > start && isspace(str[end]))
    end--;

  size_t new_len = end - start + 1;

  char *result = malloc(new_len + 1);

  if (!result)
    return NULL;

  memcpy(result, str + start, new_len);

  result[new_len] = '\0';

  return result;
}
