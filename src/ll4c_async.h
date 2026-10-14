#pragma once

#include "ll4c.h"

void startup_ThreadUnsafe(PtrLogCallback logCallback, unsigned long waitMs);
void shutdown_ThreadUnsafe(void);

// Note: caller should NOT free it
void publishLogMessage(
    LogLevel level,
    const char *codeFilename,
    int codeLine,
    char *formattedMessage);
