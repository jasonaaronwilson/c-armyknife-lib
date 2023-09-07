#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug-repl.h"
#include "interpreter.h"
#include "printer.h"

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
    } else if (starts_with(line, "step")) {
      uint64_t before_pc = state->pc;
      print_instructions(state->memory, state->pc, 1);
      interpret(state, 1);
      print_registers(state, 16, 16);
      // might need is_mapped...
      // if ((before_pc == state->pc) && (load8(state->memory, state->pc) == 0))
      // { break;
      // }
    } else if (starts_with(line, "disassemble")) {
      print_instructions(state->memory, state->pc, 16);
    } else {
      fprintf(stderr, "Uknown debug command. Ignoring.\n");
    }
  }
}
