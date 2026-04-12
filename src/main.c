#include "generate.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Must provide content directory\n");

    return 1;
  }

  StringList *list = init_string_list(10);

  get_content_files(list, argv[1]);

  generate_pages(list);

  copy_static();

  return 0;
}
