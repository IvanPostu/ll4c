#include "_shared.h"
#include <limits.h>
#include <stdlib.h>

bool parseUnsignedInt(const char *str, unsigned int *value) {
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
