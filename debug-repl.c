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
 * address symbol-name
 * assemble [optional-address/symbol-name]
 * break address/symbol-name
 * disassemble [optional-start-address/symbol] [optional-end-address/symbol]
 * examine [optional-start-address/symbol] [optional-end-address/symbol]
 * next [optional-count]
 * continue
 */
void debug_repl(cpu_thread_state *state) {
  char line[1024];

  while (1) {
    fputs("(debug) ", stderr);

    fgets(line, sizeof(line), stdin);

    if (starts_with(line, "help")) {
      fprintf(stderr, "This is a debugger repl. The available commands are:\n");
      fprintf(stderr, "  address\n");
      fprintf(stderr, "  assemble\n");
      fprintf(stderr, "  break\n");
      fprintf(stderr, "  continue\n");
      fprintf(stderr, "  disassemble\n");
      fprintf(stderr, "  examine\n");
      fprintf(stderr, "  next\n");
      fprintf(stderr, "  unbreak\n");
    } else if (starts_with(line, "terminate")) {
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
    } else if (starts_with(line, "disassemble") || starts_with(line, "dis")) {
      print_instructions(state->memory, state->pc, 16);
    } else if (starts_with(line, "examine") || starts_with(line, "x")) {
      print_data(state->memory, state->pc, state->pc + 16);
    } else {
      fprintf(stderr, "Uknown debug command. Ignoring.\n");
    }
  }
}
