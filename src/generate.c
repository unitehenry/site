#include "strings.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

void get_content_files(StringList *list, char *content_path) {
  struct dirent *de;

  DIR *dr = opendir(content_path);

  if (dr == NULL) {
    return;
  }

  while ((de = readdir(dr)) != NULL) {
    if (strcmp(de->d_name, ".") == 0) {
      continue;
    }

    if (strcmp(de->d_name, "..") == 0) {
      continue;
    }

    size_t filepath_len = strlen(content_path) + strlen(de->d_name) + 2;

    char filepath[filepath_len];

    snprintf(filepath, sizeof(filepath), "%s/%s", content_path, de->d_name);

    if (de->d_type == DT_DIR) {
      get_content_files(list, filepath);
    } else {
      string_list_append(list, filepath);
    }
  }

  closedir(dr);
}

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
