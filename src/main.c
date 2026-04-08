#include <stdio.h>
#include <dirent.h>

/*
 * - For each markdown file in `content` (command line arg)
 * - Generate html with `pandoc`
 * - Load `templates/base.html` (from command line arg)
 * - For each `content` html, replace sections
 * - Generate output html file equivalent
 */
int main(int argc, char **args) {
  printf("hello world: %d\n", argc);

  if (argc < 2) {
    fprintf(stderr, "Must provide content directory\n");

    return 1;
  }

  DIR *dr = opendir("content");

  struct dirent *de;

  while((de = readdir(dr)) != NULL) {
    printf("%s\n", de->d_name);
  }

  closedir(dr);

  return 0;
}
