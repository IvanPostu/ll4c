# Logging library for C - (LL4C)

C logging library which I use for my other C projects.

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

```c
#include "ll4c.h"
// ...

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

void example(void) {
  ll4cRegisterLogCallback(logCallback);

  int n = 123;
  L_TRACE("Example %d", n);
  L_DEBUG("Example %d", n);
  L_INFO("Example %d", n);
  L_WARN("Example %d", n);
  L_ERROR("Example %d", n);
  L_FATAL("Example %d", n);

  exit(EXIT_SUCCESS);
}
```

### Usage - asynchronous mode

```c
#include "ll4c.h"
// ...

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

void example(void) {
  ll4cRegisterLogCallback(logCallback);
  bool asyncEnabled = ll4cStartupAsyncMode();
  assert(asyncEnabled);

  int n = 123;
  L_TRACE("Example %d", n);
  L_DEBUG("Example %d", n);
  L_INFO("Example %d", n);
  L_WARN("Example %d", n);
  L_ERROR("Example %d", n);
  L_FATAL("Example %d", n);

  exit(EXIT_SUCCESS);
}
```

## Changelog

See [CHANGELOG.MD](./CHANGELOG.MD)

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
./build/ll4c_testbed --cmd1
```
