#pragma once

#include <stdbool.h>

#define SAFE_DELETE(p) \
  if (p) {             \
    free(p);           \
    p = NULL;          \
  }

// Intentionally \0 is omitted
const char *LOG_LEVELS_AS_STRINGS[6] = {"FATAL",
                                        "ERROR",
                                        "WARN",
                                        "INFO",
                                        "DEBUG",
                                        "TRACE"};

bool parseUnsignedInt(const char *str, unsigned int *value);
