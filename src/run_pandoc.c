#include <stdio.h>
#include <string.h>

void run_pandoc(FILE **fp, char *content_path) {
  char *PANDOC_CMD = "pandoc ";

  size_t command_len = strlen(content_path) + strlen(PANDOC_CMD) + 2;

  char command[command_len];

  snprintf(command, sizeof(command), "%s %s", PANDOC_CMD, content_path);

  *fp = popen(command, "r");
}