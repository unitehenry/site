#include "trim_whitespace.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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