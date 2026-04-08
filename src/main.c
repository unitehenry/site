#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "string_list.h"

/*
 * - For each markdown file in `content` (command line arg)
 * - Generate html with `pandoc`
 * - Load `templates/base.html` (from command line arg)
 * - For each `content` html, replace sections
 * - Generate output html file equivalent
 */
char** get_content_files(char* content_path) {
  StringList* list = init_string_list(sizeof(size_t));

  struct dirent *de;

  DIR *dr = opendir(content_path);

  while((de = readdir(dr)) != NULL) {
    if (strcmp(de->d_name, ".") == 0) {
      continue;
    }

    if (strcmp(de->d_name, "..") == 0) {
      continue;
    }

    // TODO: recurse directories
    if (de->d_type == DT_DIR) {
      printf("filename %s; filetype: %i (DT_DIR)\n", de->d_name, de->d_type);
    }

    string_list_append(list, de->d_name);
  }

  closedir(dr);

  return list->strings;
}

int main(int argc, char **args) {
  if (argc < 2) {
    fprintf(stderr, "Must provide content directory\n");

    return 1;
  }

  char** files = get_content_files(args[1]);

  return 0;
}
