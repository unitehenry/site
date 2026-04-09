#include "create_subdirectories.h"
#include "run_pandoc.h"
#include "string_list.h"
#include "to_write_path.h"
#include "trim_whitespace.h"
#include <stdio.h>
#include <string.h>

extern char *BASE_TEMPLATE;

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

    // begin writing template
    FILE *template_fp = fopen(BASE_TEMPLATE, "r");

    while ((template_read =
                getline(&template_line, &template_len, template_fp)) != -1) {
      if (strcmp(trim_whitespace(template_line), "{{ content }}") == 0) {
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
