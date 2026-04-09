#include <stdio.h>
#include "string_list.h"
#include "to_write_path.h"
#include "create_subdirectories.h"
#include "run_pandoc.h"

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

    char *line = NULL;

    size_t len = 0;

    ssize_t read;

    while ((read = getline(&line, &len, read_fp)) != -1) {
      fputs(line, write_fp);
    }

    fclose(write_fp);

    fclose(read_fp);

    i++;
  }
}