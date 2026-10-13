#include "cmd1_log_example.h"
#include "ll4c.h"
#include <assert.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdbool.h>
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

void handleCmd1(void) {
  PtrLogCallback logFuncPtr = logCallback;
  ll4cRegisterLogCallback(logFuncPtr);

  int n = 123;
  L_TRACE("Example %d", n);
  L_DEBUG("Example %d", n);
  L_INFO("Example %d", n);
  L_WARN("Example %d", n);
  L_ERROR("Example %d", n);
  L_FATAL("Example %d", n);

  char str[2051];
  memset(str, 'a', 2050);
  str[2050] = '\0';

  L_INFO("%s", str);
  L_INFO("");

  exit(EXIT_SUCCESS);
}
