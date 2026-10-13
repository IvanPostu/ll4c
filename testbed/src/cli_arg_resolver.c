#include "cli_arg_resolver.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isOnlyCliArgPresent(const char *longForm, const char *shortForm, int argc, char **argv) {
  if (argc == 2 && (strcmp(argv[1], longForm) == 0 || strcmp(argv[1], shortForm) == 0)) {
    return true;
  }
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], longForm) == 0 || strcmp(argv[i], shortForm) == 0) {
      fprintf(stderr, "Error: %s/%s cannot be combined with other arguments.\n", longForm, shortForm);
      exit(EXIT_FAILURE);
    }
  }
  return false;
}
