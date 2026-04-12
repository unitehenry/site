#include "libgen.h"
#include "strings.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUILD_DIRECTORY "build"
#define BASE_TEMPLATE "templates/base.html"
#define CONTENT_TAG "{{ content }}"
#define STATIC_DIRECTORY "static"

void run_pandoc(FILE **fp, char *content_path) {
  const char *PANDOC_CMD = "pandoc ";

  size_t command_len = strlen(content_path) + strlen(PANDOC_CMD) + 2;

  char command[command_len];

  snprintf(command, sizeof(command), "%s %s", PANDOC_CMD, content_path);

  *fp = popen(command, "r");
}

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

void copy_static() {
  const char *CP_CMD = "cp ";

  char *command = NULL;

  asprintf(&command, "%s -rf %s/* %s", CP_CMD, STATIC_DIRECTORY, BUILD_DIRECTORY);

  popen(command, "r");
}
