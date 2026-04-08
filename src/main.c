#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/*
 * - For each markdown file in `content` (command line arg)
 * - Generate html with `pandoc`
 * - Load `templates/base.html` (from command line arg)
 * - For each `content` html, replace sections
 * - Generate output html file equivalent
 */
char** get_content_files(char* content_path) {
  printf("content path: %s\n", content_path);

  struct dirent *de;

  DIR *dr = opendir("content");

  int file_count = 0;

  while((de = readdir(dr)) != NULL) {
    if (strcmp(de->d_name, ".") == 0) {
      continue;
    }
    if (strcmp(de->d_name, "..") == 0) {
      continue;
    }
    printf("filename %s; filetype: %i\n", de->d_name, de->d_type);
    file_count++;
  }

  printf("file count: %i\n", file_count);

  closedir(dr);

  char** files = malloc(file_count + 1);

  dr = opendir("content");

  int file_idx = 0;

  while((de = readdir(dr)) != NULL) {
    if (strcmp(de->d_name, ".") == 0) {
      continue;
    }
    if (strcmp(de->d_name, "..") == 0) {
      continue;
    }
    files[file_idx] = de->d_name;
    file_idx++;
  }

  files[file_count] = NULL;

  return files;
}

int main(int argc, char **args) {
  if (argc < 2) {
    fprintf(stderr, "Must provide content directory\n");

    return 1;
  }

  char** files = get_content_files(args[1]);

  int i = 0;

  while(files[i] != NULL) {
    printf("filename: %s\n", files[i]);

    i++;
  }

  return 0;
}
