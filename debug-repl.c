#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug-repl.h"
#include "interpreter.h"
#include "printer.h"
#include "tokenizer.h"

int starts_with(char *str1, char *str2) {
  return strncmp(str1, str2, strlen(str2)) == 0;
}

/**
 * Enter a debug repl. The list of available commands is available
 * below in the implementation of "help".
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

    token_list *tokens = tokenize(line, " ");
    char *command = token_list_get(tokens, 0);

    if (starts_with(command, "help")) {
      fprintf(stderr, "This is a debugger repl. The available commands are:\n");
      fprintf(stderr, "  address\n");
      fprintf(stderr, "  assemble\n");
      fprintf(stderr, "  break\n");
      fprintf(stderr, "  continue\n");
      fprintf(stderr, "  disassemble\n");
      fprintf(stderr, "  examine\n");
      fprintf(stderr, "  load\n");
      fprintf(stderr, "  next\n");
      fprintf(stderr, "  save\n");
      fprintf(stderr, "  step\n");
      fprintf(stderr, "  terminate\n");
      fprintf(stderr, "  unbreak\n");
    } else if (starts_with(command, "terminate")) {
      fprintf(stderr, "The debugger has terminated execution. Exiting...\n");
      exit(0);
    } else if (starts_with(command, "step")) {
      uint64_t before_pc = state->pc;
      print_instructions(state->memory, state->pc, 1);
      interpret(state, 1);
      print_registers(state, 16, 16);
      // might need is_mapped...
      // if ((before_pc == state->pc) && (load8(state->memory, state->pc) == 0))
      // { break;
      // }
    } else if (starts_with(command, "disassemble") ||
               starts_with(command, "dis")) {
      print_instructions(state->memory, state->pc, 16);
    } else if (starts_with(command, "examine") || starts_with(command, "x")) {
      print_data(state->memory, state->pc, state->pc + 16);
    } else {
      fprintf(stderr, "Uknown debug command. Ignoring.\n");
    }

    token_list_free_all(tokens);
  }
}
