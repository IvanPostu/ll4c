#pragma once

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

typedef void (*PtrLogCallback)(LogLevel level, const char *message);

API_LL4C void ll4cRegisterLogCallback(PtrLogCallback callback);
API_LL4C void ll4cLogMessage(LogLevel level, const char *message, ...);

#define L_FATAL(message, ...) ll4cLogMessage(LL4C_FATAL, message, ##__VA_ARGS__);
#define L_ERROR(message, ...) ll4cLogMessage(LL4C_ERROR, message, ##__VA_ARGS__);
#define L_WARN(message, ...) ll4cLogMessage(LL4C_WARN, message, ##__VA_ARGS__);
#define L_INFO(message, ...) ll4cLogMessage(LL4C_INFO, message, ##__VA_ARGS__);
#define L_DEBUG(message, ...) ll4cLogMessage(LL4C_DEBUG, message, ##__VA_ARGS__);
#define L_TRACE(message, ...) ll4cLogMessage(LL4C_TRACE, message, ##__VA_ARGS__);
