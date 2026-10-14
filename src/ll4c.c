#include "ll4c.h"

#include "_shared.h"
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EXCLUDE_ASYNC_MODE
#  include "ll4c_async.h"
#endif

#ifdef _WIN64
#  define VSNPRINTF vsnprintf_s
#else
#  define VSNPRINTF vsnprintf
#endif

const char *LOG_LEVELS_AS_STRINGS[6] = {"FATAL",
                                        "ERROR",
                                        "WARN",
                                        "INFO",
                                        "DEBUG",
                                        "TRACE"};

#define CACHE_LENGTH 2048
static char CACHE_STR[CACHE_LENGTH];

static PtrLogCallback ptr_LogCallback = 0;

#ifndef EXCLUDE_ASYNC_MODE
static bool isAsyncModeActive = false;
#endif

static int string_formatted_len(const char *format, va_list va_listp) {
  if (!format) {
    return 0;
  }

  // Create a copy of the va_listp since vsnprintf can invalidate the elements
  // of the list while finding the required buffer length.
  va_list va_listp2;
  va_copy(va_listp2, va_listp);

  int len = VSNPRINTF(NULL, 0, format, va_listp2);

  va_end(va_listp2);

  return len;
}

static inline void internalLog(LogLevel level, const char *codeFilename, int codeLine, const char *format, va_list va_listp) {
  if (!ptr_LogCallback) {
    return;
  }
  if (!format) {
    return;
  }

  int formattedLength = string_formatted_len(format, va_listp);
  const int totalLen = formattedLength + 1; // str + '\0'
  char *buffer;

#ifdef EXCLUDE_ASYNC_MODE
#  define _isAsyncModeActive false
#  define _asyncPublishMsg(msg)
#else
#  define _asyncModeActive isAsyncModeActive
#  define _asyncPublishMsg(level, codeFilename, codeLine, msg) publishLogMessage(level, codeFilename, codeLine, msg)
#endif

  if (totalLen <= CACHE_LENGTH && !_asyncModeActive) {
    buffer = CACHE_STR;
  } else {
    buffer = malloc(totalLen);
  }

  VSNPRINTF(buffer, totalLen, format, va_listp);
  buffer[totalLen - 1] = '\0';

  if (_asyncModeActive) {
    _asyncPublishMsg(level, codeFilename, codeLine, buffer);
  } else {
    ptr_LogCallback(level, codeFilename, codeLine, buffer);

    if (&buffer[0] != CACHE_STR) {
      free(buffer);
    }
  }
#undef _asyncModeActive
}

void ll4cRegisterLogCallback(PtrLogCallback callback) {
  ptr_LogCallback = callback;
}

void _ll4cLogMessage(LogLevel level, const char *codeFilename, int codeLine, const char *format, ...) {
  __builtin_va_list arg_ptr;
  va_start(arg_ptr, format);
  internalLog(level, codeFilename, codeLine, format, arg_ptr);
  va_end(arg_ptr);
}

bool ll4cGetVersionInfo(unsigned int *major, unsigned int *minor, unsigned int *patch) {
  if (!parseUnsignedInt(APP_VERSION_MAJOR, major)) {
    return false;
  }
  if (!parseUnsignedInt(APP_VERSION_MINOR, minor)) {
    return false;
  }
  if (!parseUnsignedInt(APP_VERSION_PATCH, patch)) {
    return false;
  }
  return true;
}

bool ll4cStartupAsyncMode(void) {
#ifdef EXCLUDE_ASYNC_MODE
  return false;
#else
  if (!ptr_LogCallback) {
    return false;
  }
  startup_ThreadUnsafe(ptr_LogCallback, 500L);
  return (isAsyncModeActive = true);
#endif
}

void ll4cShutdownAsyncMode(void) {
#ifndef EXCLUDE_ASYNC_MODE
  shutdown_ThreadUnsafe();
#endif
}

const char *ll4cLogLevelAsStr(LogLevel logLevel) {
  return LOG_LEVELS_AS_STRINGS[logLevel];
}
