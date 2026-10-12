#include "ll4c.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN64
#  define VSNPRINTF vsnprintf_s
#else
#  define VSNPRINTF vsnprintf
#endif

#define LEVEL_STR_LENGTH 9
static const char levelStrings[6][LEVEL_STR_LENGTH] = {"[FATAL]: ",
                                                       "[ERROR]: ",
                                                       "[WARN]:  ",
                                                       "[INFO]:  ",
                                                       "[DEBUG]: ",
                                                       "[TRACE]: "};
static PtrLogCallback ptr_LogCallback = 0;

static char *string_format_v(LogLevel level, const char *format, va_list va_listp) {
  if (!format) {
    return 0;
  }

  // Create a copy of the va_listp since vsnprintf can invalidate the elements
  // of the list while finding the required buffer length.
  va_list va_listp2;
  va_copy(va_listp2, va_listp);

  int len = VSNPRINTF(NULL, 0, format, va_listp2);
  const int totalLen = LEVEL_STR_LENGTH + len + 1; // level + str + '\0'
  char *buffer = malloc(totalLen);
  if (!buffer) {
    return 0;
  }
  VSNPRINTF(LEVEL_STR_LENGTH + buffer, totalLen, format, va_listp);
  memcpy(buffer, levelStrings[level], LEVEL_STR_LENGTH);

  va_end(va_listp2);

  buffer[totalLen - 1] = '\0';
  return buffer;
}

void ll4cRegisterLogCallback(PtrLogCallback callback) {
  ptr_LogCallback = callback;
}

void ll4cLogMessage(LogLevel level, const char *message, ...) {
  __builtin_va_list arg_ptr;
  va_start(arg_ptr, message);
  if (ptr_LogCallback) {
    char *result = string_format_v(level, message, arg_ptr);
    ptr_LogCallback(level, result);
    free(result);
  }
  va_end(arg_ptr);
}
