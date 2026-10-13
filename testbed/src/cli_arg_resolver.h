#pragma once

#include <stdbool.h>

bool isOnlyCliArgPresent(const char *longForm, const char *shortForm, int argc, char **argv);
