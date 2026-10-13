#include "cmd_version.h"
#include "ll4c.h"
#include <stdio.h>
#include <stdlib.h>

void handleVersionCommand(void) {
  unsigned int major;
  unsigned int minor;
  unsigned int patch;
  if (!ll4cGetVersionInfo(&major, &minor, &patch)) {
    fprintf(stderr, "Error: wasn't possible to get version info.\n");
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "v%d.%d.%d\n", major, minor, patch);
  exit(EXIT_SUCCESS);
}
