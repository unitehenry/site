#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void create_subdirectories(char *filepath) {
  char *path_copy = strdup(filepath);

  if (!path_copy)
    return;

  char *last_slash = strrchr(path_copy, '/');

  if (last_slash) {
    *last_slash = '\0';
  } else {
    free(path_copy);

    return;
  }

  char *p;

  for (p = path_copy + 1; *p; p++) {
    if (*p == '/') {
      *p = '\0';

      mkdir(path_copy, 0755);

      *p = '/';
    }
  }

  mkdir(path_copy, 0755);

  free(path_copy);
}
