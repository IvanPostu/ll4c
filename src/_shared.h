#pragma once

#include <stdbool.h>

#define SAFE_DELETE(p) \
  if (p) {             \
    free(p);           \
    p = NULL;          \
  }

bool parseUnsignedInt(const char *str, unsigned int *value);
