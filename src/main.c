#include "generate_pages.h"
#include "get_content_files.h"
#include <stdio.h>
#include <stdlib.h>

char *BUILD_DIRECTORY = "build";
char *BASE_TEMPLATE = "templates/base.html";
char *CONTENT_TAG = "{{ content }}";

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Must provide content directory\n");

    return 1;
  }

  StringList *list = init_string_list(10); // start with capacity 10

  get_content_files(list, argv[1]);

  generate_pages(list);

  return 0;
}
