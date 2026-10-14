# Logging library for C - (LL4C)

Logging library for C with async and sync modes which I use for my other C projects, especially for graphic programming

## Problem which this project solves

Logging logic inside render function in graphic programming (OpenGL/Vulkan). Default approach with `printf` is extremely bad inside render function because it is an IO operation and triggers context switching. This project solves this problem with async mode. See [Benchmark](#benchmark)

## Integration

### Build

1. Build static library (`build/libll4c.a`)

    ```sh
    cmake -B build

    cmake --build ./build --config Release
    ```

1. Use API: `include/ll4c.h`
1. Statically link `build/libll4c.a` to the executable program

### Usage - synchronous mode

[./testbed/src/cmd4_demo_sync_mode.c](./testbed/src/cmd4_demo_sync_mode.c)

```c
#include "cmd4_demo_sync_mode.h"
#include "ll4c.h"
#include <stdio.h>
#include <stdlib.h>

static void logCallback(
    LogLevel level,
    const char *codeFilename,
    int codeLine,
    const char *formattedMessage) {
  FILE *output = stdout;
  const char *logLevelStr = ll4cLogLevelAsStr(level);

  if (level == LL4C_FATAL || level == LL4C_ERROR) {
    output = stderr;
  }

  fprintf(output, "[%s](%s:%d): %s\n", logLevelStr, codeFilename, codeLine, formattedMessage);
}

void handleCmd4(void) {
  ll4cRegisterLogCallback(logCallback);

  int n = 123;
  L_TRACE("Example %d", n);
  L_DEBUG("Example %s", "test");
  L_INFO("Example %d", n);
  L_WARN("Example %d", n);
  L_ERROR("Example %d", n);
  L_FATAL("Example %d", n);

  exit(EXIT_SUCCESS);
}
```

Output:

```txt
[TRACE] (cmd4_demo_sync_mode.c:25): Example 123
[DEBUG] (cmd4_demo_sync_mode.c:26): Example test
...
```

### Usage - asynchronous mode

[testbed/src/cmd5_demo_async_mode.c](./testbed/src/cmd5_demo_async_mode.c)

```c
#include "cmd5_demo_async_mode.h"
#include "ll4c.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void logCallback(
    LogLevel level,
    const char *codeFilename,
    int codeLine,
    const char *formattedMessage) {
  FILE *output = stdout;
  const char *logLevelStr = ll4cLogLevelAsStr(level);

  if (level == LL4C_FATAL || level == LL4C_ERROR) {
    output = stderr;
  }

  fprintf(output, "[%s] (%s:%d): %s\n", logLevelStr, codeFilename, codeLine, formattedMessage);
}

void handleCmd5(void) {
  ll4cRegisterLogCallback(logCallback);
  bool asyncEnabled = ll4cStartupAsyncMode();
  assert(asyncEnabled);

  int n = 123;
  L_TRACE("Example %d", n);
  L_DEBUG("Example %s", "test");
  L_INFO("Example %d", n);
  L_WARN("Example %d", n);
  L_ERROR("Example %d", n);
  L_FATAL("Example %d", n);

  ll4cShutdownAsyncMode();
  exit(EXIT_SUCCESS);
}
```

Output:

```txt
[TRACE] (cmd4_demo_sync_mode.c:25): Example 123
[DEBUG] (cmd4_demo_sync_mode.c:26): Example test
...
```

## Changelog

See [CHANGELOG.MD](./CHANGELOG.MD)

## Benchmark

### Benchmark 1 sync vs. async execution time - caller's perspective

[./testbed/src/cmd6_benchmark.c](./testbed/src/cmd6_benchmark.c)

| Mode | Iterations | Time taken in millis |
| :--- | :--- | :--- |
| Sync | 1000000 | 19374.9005 |
| Async | 1000000 | 1771.0027 |
| Sync | 500000 | 11056.9147 |
| Async | 500000 | 856.4132 |
| Sync | 100000 | 2388.6567 |
| Async | 100000 | 180.3441 |

## Setup dev env

```sh
# cmake, gcc-v13, clangd(C language LSP), clang-formatter ...
nix-shell --run bash dev.nix

# build - dev mode
cmake \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DBUILD_TESTBED=1 \
  -DIGNORE_WARNINGS=1 \
  -DINCLUDE_DEBUG_SYMBOLS=1 \
  -B build

cmake --build ./build --config Debug
cmake --build ./build --config Debug --clean-first --verbose

# run
./build/ll4c_testbed --cmd4
```
