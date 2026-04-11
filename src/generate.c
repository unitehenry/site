#include "strings.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>

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
