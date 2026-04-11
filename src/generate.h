#ifndef GENERATE_H
#define GENERATE_H

#include "strings.h"

void get_content_files(StringList *list, char *content_path);
void create_subdirectories(char *filepath);
void generate_pages(StringList *list);

#endif
