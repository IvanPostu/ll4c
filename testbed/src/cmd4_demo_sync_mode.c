#include "cmd4_demo_sync_mode.h"
#include "ll4c.h"
#include <stdio.h>
#include <stdlib.h>

static void logCallback(
    LogLevel level,
    const char *codeFilename,
    int codeLine,
    const char *formattedMessage) {
  FILE *output = stdout;
  const char *logLevelStr = ll4cLogLevelAsStr(level);

  if (level == LL4C_FATAL || level == LL4C_ERROR) {
    output = stderr;
  }

  fprintf(output, "[%s] (%s:%d): %s\n", logLevelStr, codeFilename, codeLine, formattedMessage);
}

void handleCmd4(void) {
  ll4cRegisterLogCallback(logCallback);

  int n = 123;
  L_TRACE("Example %d", n);
  L_DEBUG("Example %s", "test");
  L_INFO("Example %d", n);
  L_WARN("Example %d", n);
  L_ERROR("Example %d", n);
  L_FATAL("Example %d", n);

  exit(EXIT_SUCCESS);
}
