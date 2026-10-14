#pragma once

#include <stdbool.h>

#ifndef API_LL4C
#  define API_LL4C
#endif

typedef enum LogLevel {
  LL4C_FATAL = 0,
  LL4C_ERROR = 1,
  LL4C_WARN = 2,
  LL4C_INFO = 3,
  LL4C_DEBUG = 4,
  LL4C_TRACE = 5
} LogLevel;

typedef void (*PtrLogCallback)(
    LogLevel level,
    const char *codeFilename,
    int codeLine,
    const char *formattedMessage);

API_LL4C void ll4cRegisterLogCallback(PtrLogCallback callback);
API_LL4C bool ll4cGetVersionInfo(unsigned int *major, unsigned int *minor, unsigned int *patch);
API_LL4C bool ll4cStartupAsyncMode(void);
API_LL4C void ll4cShutdownAsyncMode(void);
API_LL4C const char *ll4cLogLevelAsStr(LogLevel logLevel);

// Internal only!!! Shouldn't be used outside
API_LL4C void _ll4cLogMessage(LogLevel level, const char *codeFilename, int codeLine, const char *format, ...);

#define L_FATAL(format, ...) _ll4cLogMessage(LL4C_FATAL, __FILE_NAME__, __LINE__, format, ##__VA_ARGS__);
#define L_ERROR(format, ...) _ll4cLogMessage(LL4C_ERROR, __FILE_NAME__, __LINE__, format, ##__VA_ARGS__);
#define L_WARN(format, ...) _ll4cLogMessage(LL4C_WARN, __FILE_NAME__, __LINE__, format, ##__VA_ARGS__);
#define L_INFO(format, ...) _ll4cLogMessage(LL4C_INFO, __FILE_NAME__, __LINE__, format, ##__VA_ARGS__);
#define L_DEBUG(format, ...) _ll4cLogMessage(LL4C_DEBUG, __FILE_NAME__, __LINE__, format, ##__VA_ARGS__);
#define L_TRACE(format, ...) _ll4cLogMessage(LL4C_TRACE, __FILE_NAME__, __LINE__, format, ##__VA_ARGS__);
