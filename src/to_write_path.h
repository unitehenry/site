#ifndef TO_WRITE_PATH_H
#define TO_WRITE_PATH_H

#include "string_replace.h"
#include "path_root.h"

extern char *BUILD_DIRECTORY;

void to_write_path(char **write_path, char *read_path);

#endif