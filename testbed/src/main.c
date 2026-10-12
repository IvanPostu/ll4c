#include "ll4c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void logCallback(LogLevel level, const char *message) {
  FILE *out = stdout;
  FILE *err = stderr;
  if (level == LL4C_FATAL || level == LL4C_ERROR) {
    fprintf(err, "%s\n", message);
  } else {
    fprintf(out, "%s\n", message);
  }
}

static void handleVersionCommand(int argc, char **argv) {
  if (argc == 2 && (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)) {
    printf("v%s\n", APP_VERSION);
    exit(EXIT_SUCCESS);
  }
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
      fprintf(stderr, "Error: --version cannot be combined with other arguments.\n");
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char **argv) {
  handleVersionCommand(argc, argv);

  PtrLogCallback logFuncPtr = logCallback;
  ll4cRegisterLogCallback(logFuncPtr);

  int n = 2001901;
  L_TRACE("Example %d", n);
  L_DEBUG("Example %d", n);
  L_INFO("Example %d", n);
  L_WARN("Example %d", n);
  L_ERROR("Example %d", n);

  return EXIT_SUCCESS;
}
