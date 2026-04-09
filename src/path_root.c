#include "path_root.h"
#include <libgen.h>
#include <stdlib.h>
#include <string.h>

void path_root(char **root, char *path) {
  char *path_copy = strdup(path);

  if (!path_copy) {
    *root = NULL;
    return;
  }

  char *dir = dirname(path_copy);

  char *last_dir = malloc(strlen(dir) + 1);

  strcpy(last_dir, dir);

  while (strcmp(dir, ".") != 0) {
    dir = dirname(path_copy);

    if (strcmp(dir, ".") != 0) {
      free(last_dir);

      last_dir = malloc(strlen(dir) + 1);

      strcpy(last_dir, dir);
    }
  }

  *root = strdup(last_dir);

  free(last_dir);

  free(path_copy);
}
