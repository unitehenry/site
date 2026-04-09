#include "string_list.h"
#include "string_replace.h"
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

void run_pandoc(FILE *fp, char *content_path) {
  char *PANDOC_CMD = "pandoc ";

  size_t command_len = strlen(content_path) + strlen(PANDOC_CMD) + 2;

  char command[command_len];

  snprintf(command, sizeof(command), "%s %s", PANDOC_CMD, content_path);

  fp = popen(command, "w");
}

void to_write_path(char *write_path, char* read_path) {
    char replace_path[strlen(read_path)];

    strcpy(replace_path, read_path);

    strcpy(write_path, str_replace(replace_path, ".md", ".html"));

    strcpy(write_path, str_replace(write_path, "content/", "static/"));
}

void generate_pages(StringList *list) {
  int i = 0;

  while (list->strings[i] != NULL) {
    char write_path[40];

    to_write_path(write_path, list->strings[i]);

    printf("filepath: %s\n", write_path);

    // open file to write

    i++;

    continue;

    FILE *fp;

    run_pandoc(fp, list->strings[i]);

    char *line = NULL;

    size_t len = 0;

    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
      // printf("%s", line);
    }

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
