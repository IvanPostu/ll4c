# Logging library for C - (LL4C)

Minimal C logging library which I use for my other C projects.

## Usage

1. Build static library (`build/libll4c.a`)

    ```sh
    cmake -B build
    cmake --build ./build --config Release
    ```

1. Statically link `build/libll4c.a` to the executable program
    - Use header file: `include/ll4c.h`

1. Example of usage:

    ```c
    static void logCallback(LogLevel level, const char *message) {
      FILE *out = stdout;
      FILE *err = stderr;
      if (level == LL4C_FATAL || level == LL4C_ERROR) {
        fprintf(err, "%s\n", message);
      } else {
        fprintf(out, "%s\n", message);
      }
    }

    int main(int argc, char **argv) {
      PtrLogCallback logFuncPtr = logCallback;
      ll4cRegisterLogCallback(logFuncPtr);

      int n = 123;
      L_TRACE("Example %d", n);
      L_DEBUG("Example %d", n);
      L_INFO("Example %d", n);
      L_WARN("Example %d", n);
      L_ERROR("Example %d", n);
      L_FATAL("Example %d", n);

      return EXIT_SUCCESS;
    }
    ```

## Setup dev env

```sh
# cmake, gcc-v13, clangd(C language LSP), clang-formatter ...
nix-shell --run bash dev.nix

cmake \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DBUILD_TESTBED=1 \
  -DIGNORE_WARNINGS=1 \
  -B build

cmake --build ./build --config Debug
cmake --build ./build --config Debug --clean-first --verbose

./build/ll4c_testbed
```

## Build release

```sh
cmake \
  -B build

cmake --build ./build --config Release --clean-first
```
