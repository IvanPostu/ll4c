# Changelog

All notable changes to this project will be documented in this file.

This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.5] - 2026-10-14

- Add asynchronous mode (backed by POSIX threads)
  - startup: `bool ll4cStartupAsyncMode(void);`
  - shutdown: `void ll4cShutdownAsyncMode(void);`
  - By default synchronous mode is used
  ---
- Expose source filename and source line in log callback
- Log level is not part of message anymore
  - If needed is should be appended manually on log callback
- expose `const char *ll4cLogLevelAsStr(LogLevel logLevel)` in API

## [0.0.4] - 2026-10-13

- Optimization: if log message length is \< 2048 then do not call `malloc`, but use cache buffer instead
- Build script: add flag `INCLUDE_DEBUG_SYMBOLS`

## [0.0.3] - 2026-10-12

- expose function for version info `ll4cGetVersionInfo(major, minor, patch)`

## [0.0.2] - 2026-10-12

- `--version` / `-v` flags for testbed executable
- `cmake` logic - parse version from `CHANGELOG.MD`

## [0.0.1] - 2026-10-12

- First working prototype
