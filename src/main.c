#include "string_list.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * - For each markdown file in `content` (command line arg)
 * - Generate html with `pandoc`
 * - Load `templates/base.html` (from command line arg)
 * - For each `content` html, replace sections
 * - Generate output html file equivalent
 */
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

void generate_pages(StringList *list) {
  int i = 0;

  while (list->strings[i] != NULL) {
    size_t command_len = strlen(list->strings[i]) + strlen("pandoc ") + 2;

    char command[command_len];

    snprintf(command, sizeof(command), "pandoc %s", list->strings[i]);

    printf("%s\n", command);

    FILE *fp = popen(command, "w");

    fclose(fp);

    i++;
  }
}

int main(int argc, char **args) {
  if (argc < 2) {
    fprintf(stderr, "Must provide content directory\n");

    return 1;
  }

  StringList *list = init_string_list(10); // start with capacity 10

  get_content_files(list, args[1]);

  generate_pages(list);

  return 0;
}
