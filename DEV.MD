# Dev notes

```sh
nix-shell --run bash dev.nix
```

## Build

```sh
mkdir build && cd build

cmake ..
cmake .. -DBUILD_TESTBED=1

cmake --build .
```
