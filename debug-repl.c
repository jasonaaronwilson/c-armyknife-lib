#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug-repl.h"

int starts_with(char *str1, char *str2) {
  return strncmp(str1, str2, strlen(str2)) == 0;
}

/**
 * Enter a debug repl. The repl should eventually understand the
 * following commands:
 *
 * assemble [optional-address]
 * disassemble [optional-start-address] [optional-end-address]
 * dump [optional-start-address] [optional-end-address]
 * next [optional-count]
 * continue
 */
void debug_repl(cpu_thread_state *state) {
  char line[1024];

  while (1) {
    fputs("(debug) ", stderr);

    fgets(line, sizeof(line), stdin);

    if (starts_with(line, "terminate")) {
      fprintf(stderr, "The debugger has terminated execution. Exiting...\n");
      exit(0);
    } else {
      fprintf(stderr, "Uknown debug command. Ignoring.\n");
    }
  }
}
