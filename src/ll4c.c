#include "ll4c.h"

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN64
#  define VSNPRINTF vsnprintf_s
#else
#  define VSNPRINTF vsnprintf
#endif

#define CACHE_LENGTH 2048
static char CACHE_STR[CACHE_LENGTH];

#define LEVEL_STR_LENGTH 9
static const char levelStrings[6][LEVEL_STR_LENGTH] = {"[FATAL]: ",
                                                       "[ERROR]: ",
                                                       "[WARN]:  ",
                                                       "[INFO]:  ",
                                                       "[DEBUG]: ",
                                                       "[TRACE]: "};
static PtrLogCallback ptr_LogCallback = 0;

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

static bool parseUnsignedInt(const char *str, unsigned int *value) {
  char *endptr;
  unsigned long val = strtoul(str, &endptr, 10);

  if (endptr == str) {
    return false;
  } else if (val > UINT_MAX) {
    return false;
  } else {
    *value = (unsigned int)val;
    return true;
  }
}

static inline void internalLog(LogLevel level, const char *format, va_list va_listp) {
  if (!ptr_LogCallback) {
    return;
  }
  if (!format) {
    return;
  }

  int formattedLength = string_formatted_len(format, va_listp);
  const int totalLen = LEVEL_STR_LENGTH + formattedLength + 1; // level + str + '\0'
  char *buffer;

  if (totalLen <= CACHE_LENGTH) {
    buffer = CACHE_STR;
  } else {
    buffer = malloc(totalLen);
  }

  VSNPRINTF(LEVEL_STR_LENGTH + buffer, totalLen, format, va_listp);
  memcpy(buffer, levelStrings[level], LEVEL_STR_LENGTH);
  buffer[totalLen - 1] = '\0';

  ptr_LogCallback(level, buffer);

  if (&buffer[0] != CACHE_STR) {
    free(buffer);
  }
}

void ll4cRegisterLogCallback(PtrLogCallback callback) {
  ptr_LogCallback = callback;
}

void _ll4cLogMessage(LogLevel level, const char *message, ...) {
  __builtin_va_list arg_ptr;
  va_start(arg_ptr, message);
  internalLog(level, message, arg_ptr);
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
