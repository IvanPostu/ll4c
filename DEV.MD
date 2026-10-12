# Dev notes

## Setup dev env shell

```sh
nix-shell --run bash dev.nix
```

## Build and run

```sh
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_TESTBED=1 -B build

cmake --build ./build --config Release

./build/ll4c_testbed
```
