#include "cli_arg_resolver.h"
#include "cmd1_log_example.h"
#include "cmd2_async_log_worker.h"
#include "cmd3_async_log_usage.h"
#include "cmd4_demo_sync_mode.h"
#include "cmd5_demo_async_mode.h"
#include "cmd_version.h"
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
  if (isOnlyCliArgPresent("--cmd3", "-c3", argc, argv)) {
    handleCmd3();
  }
  if (isOnlyCliArgPresent("--cmd4", "-c4", argc, argv)) {
    handleCmd4();
  }
  if (isOnlyCliArgPresent("--cmd5", "-c5", argc, argv)) {
    handleCmd5();
  }

  return EXIT_SUCCESS;
}
