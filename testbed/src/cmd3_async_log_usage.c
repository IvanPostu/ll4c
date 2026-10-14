#include "cmd3_async_log_usage.h"
#include "ll4c.h"
#include <assert.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  fprintf(output, "[%s](%s:%d): %s\n", logLevelStr, codeFilename, codeLine, formattedMessage);
}

void handleCmd3(void) {
  ll4cRegisterLogCallback(logCallback);
  bool asyncEnabled = ll4cStartupAsyncMode();
  assert(asyncEnabled);

  char input[100];
  while (true) {
    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0) {
      for (int i = 0; i < 50; i++) {
        char *buff = malloc(256);
        snprintf(buff, 256, "%d) %s", i, "example");

        L_INFO("%s - aaa", buff);
      }
    }

    if (strcmp(input, "1") == 0) {
      break;
    }
  }

  exit(EXIT_SUCCESS);
}
