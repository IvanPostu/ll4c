#include "ll4c.h"
#include <stdio.h>
#include <stdlib.h>

static void logCallback(LogLevel level, const char *message) {
  FILE *out = stdout;
  FILE *err = stderr;
  if (level == LL4C_FATAL || level == LL4C_ERROR) {
    fprintf(err, "%s\n", message);
  } else {
    fprintf(out, "%s\n", message);
  }
}

int main(void) {
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
