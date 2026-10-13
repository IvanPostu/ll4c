#include "cli_arg_resolver.h"
#include "cmd1_log_example.h"
#include "cmd2_async_log_worker.h"
#include "cmd_version.h"
#include <assert.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (isOnlyCliArgPresent("--version", "-v", argc, argv)) {
    handleVersionCommand();
  }
  if (isOnlyCliArgPresent("--cmd1", "-c1", argc, argv)) {
    handleCmd1();
  }
  if (isOnlyCliArgPresent("--cmd2", "-c2", argc, argv)) {
    handleCmd2();
  }

  return EXIT_SUCCESS;
}
