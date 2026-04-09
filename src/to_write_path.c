#include "path_root.h"
#include "string_replace.h"
#include <stdio.h>
#include <string.h>

extern char *BUILD_DIRECTORY;

void to_write_path(char **write_path, char *read_path) {
  char replace_path[strlen(read_path)];

  strcpy(replace_path, read_path);

  *write_path = str_replace(replace_path, ".md", ".html");

  char *root;

  path_root(&root, *write_path);

  char from_dir[strlen(root) + 2];

  snprintf(from_dir, strlen(root) + 2, "%s/", root);

  char to_dir[strlen(BUILD_DIRECTORY) + 2];

  snprintf(to_dir, strlen(BUILD_DIRECTORY) + 2, "%s/", BUILD_DIRECTORY);

  *write_path = str_replace(*write_path, from_dir, to_dir);
}
