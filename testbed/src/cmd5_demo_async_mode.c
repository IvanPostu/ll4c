#include "cmd5_demo_async_mode.h"
#include "ll4c.h"
#include <assert.h>
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

void handleCmd5(void) {
  ll4cRegisterLogCallback(logCallback);
  bool asyncEnabled = ll4cStartupAsyncMode();
  assert(asyncEnabled);

  int n = 123;
  L_TRACE("Example %d", n);
  L_DEBUG("Example %s", "test");
  L_INFO("Example %d", n);
  L_WARN("Example %d", n);
  L_ERROR("Example %d", n);
  L_FATAL("Example %d", n);

  ll4cShutdownAsyncMode();
  exit(EXIT_SUCCESS);
}
