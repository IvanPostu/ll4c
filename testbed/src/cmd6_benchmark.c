#include "cmd6_benchmark.h"
#include "ll4c.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

static long long get_current_nanos(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (long long)(ts.tv_sec) * 1000000000LL + ts.tv_nsec;
}

void handleCmd6(void) {
  ll4cRegisterLogCallback(logCallback);
  bool asyncEnabled = ll4cStartupAsyncMode();
  assert(asyncEnabled);

  int n = 1000000;

  long long start = get_current_nanos();
  {
    for (int i = 0; i < n; i++) {
      L_TRACE("Example of number=%d and text=%s", n, "whatever");
      L_DEBUG("Example of number=%d and text=%s", n, "whatever");
      L_INFO("Example of number=%d and text=%s", n, "whatever");
      L_WARN("Example of number=%d and text=%s", n, "whatever");
      L_ERROR("Example of number=%d and text=%s", n, "whatever");
      L_FATAL("Example of number=%d and text=%s", n, "whatever");
    }
  }
  long long end = get_current_nanos();
  long long elapsed = end - start;

  // result is shown after all logs were printed
  ll4cShutdownAsyncMode();

  printf("Execution time: %lld ns\n", elapsed);
  double millis = (double)elapsed / 1000000.0;
  printf("Execution time: %.4f millis\n", millis);

  exit(EXIT_SUCCESS);
}
