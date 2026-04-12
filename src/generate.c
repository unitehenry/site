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
#define TITLE_TAG "{{ title }}"
#define STATIC_DIRECTORY "static"

void run_pandoc(FILE **fp, char *content_path) {
  const char *PANDOC_CMD = "pandoc";

  char *command;

  asprintf(&command, "%s %s", PANDOC_CMD, content_path);

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

void get_content_metadata(StringMap **metadata, char *content_path) {
  FILE *content_fp = fopen(content_path, "r");

  char *content_line = NULL;

  size_t content_len = 0;

  ssize_t content_read;

  bool metadata_start = false;

  while ((content_read = getline(&content_line, &content_len, content_fp)) !=
         -1) {
    if (strcmp(trim_whitespace(content_line), "---") == 0) {
      if (metadata_start) {
        break;
      } else {
        metadata_start = true;
        continue;
      }
    }

    if (metadata_start) {
      char *token;

      token = strtok(content_line, ":");

      char *metadata_key = NULL;
      char *metadata_value = NULL;

      while (token != NULL) {
        if (metadata_key != NULL && metadata_value != NULL) {
          break;
        }

        if (metadata_key == NULL) {
          asprintf(&metadata_key, "%s", token);
          token = strtok(NULL, ":");
          continue;
        }

        if (metadata_key != NULL && metadata_value == NULL) {
          asprintf(&metadata_value, "%s", token);
          token = strtok(NULL, ":");
          continue;
        }
      }

      if (metadata_key != NULL && metadata_value != NULL) {
        string_map_put(*metadata, trim_whitespace(metadata_key),
                       trim_whitespace(metadata_value));
      }
    }
  }

  fclose(content_fp);
}

void generate_pages(StringList *list) {
  int i = 0;

  while (list->strings[i] != NULL) {
    StringMap *metadata = string_map_create();

    get_content_metadata(&metadata, list->strings[i]);

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
      } else if (strcmp(trim_whitespace(template_line), TITLE_TAG) == 0) {
        if (string_map_contains(metadata, "title")) {
          char *title_tag;

          asprintf(&title_tag, "<title>%s</title>",
                   string_map_get(metadata, "title"));

          fputs(title_tag, write_fp);
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
  const char *CP_CMD = "cp";

  char *command = NULL;

  asprintf(&command, "%s -rf %s/* %s", CP_CMD, STATIC_DIRECTORY,
           BUILD_DIRECTORY);

  popen(command, "r");
}
