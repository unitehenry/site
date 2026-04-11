#include "strings.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "to_write_path.h"
#include "run_pandoc.h"
#include <sys/stat.h>

extern char *CONTENT_TAG;
extern char *BASE_TEMPLATE;

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

void generate_pages(StringList *list) {
  int i = 0;

  while (list->strings[i] != NULL) {
    char *write_path;

    to_write_path(&write_path, list->strings[i]);

    printf("filepath: %s\n", write_path);

    create_subdirectories(write_path);

    FILE *write_fp = fopen(write_path, "w");

    FILE *read_fp;

    run_pandoc(&read_fp, list->strings[i]);

    char *template_line = NULL;

    size_t template_len = 0;

    ssize_t template_read;

    FILE *template_fp = fopen(BASE_TEMPLATE, "r");

    while ((template_read =
                getline(&template_line, &template_len, template_fp)) != -1) {
      if (strcmp(trim_whitespace(template_line), CONTENT_TAG) == 0) {
        char *content_line = NULL;

        size_t content_len = 0;

        ssize_t content_read;

        while ((content_read = getline(&content_line, &content_len, read_fp)) !=
               -1) {
          fputs(content_line, write_fp);
        }
      } else {
        fputs(template_line, write_fp);
      }
    }

    fclose(write_fp);

    fclose(read_fp);

    i++;
  }
}
