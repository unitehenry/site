#include "path_root.h"
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

void path_root(char **root, char* path) {
    char *path_copy = strdup(path);
    if (!path_copy) {
        *root = NULL;
        return;
    }
    char *dir = dirname(path_copy);
    *root = strdup(dir);
    free(path_copy);
}