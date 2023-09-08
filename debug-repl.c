#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "debug-repl.h"
#include "interpreter.h"
#include "printer.h"
#include "string-util.h"
#include "symbol-table.h"
#include "tokenizer.h"

// int string_starts_with(char *str1, char *str2) {
//  return strncmp(str1, str2, strlen(str2)) == 0;
// }

uint64_t parse_address(cpu_thread_state *state, char *str) {
  if (str[0] == '%') {
    // FIXME
    return 42;
  }
  symbol *sym = find_symbol_by_name(state->symbols, str);
  if (!string_is_null_or_empty(sym->name)) {
    return sym->value;
  }
  // FIXME parse here!
  return 0;
}

void debug_help_command(token_list *tokens) {
  fprintf(stderr, "This is a debugger repl. The available commands are:\n");
  fprintf(stderr, "  add-address\n"); // TODO(jawilson)
  fprintf(stderr, "  address\n");
  fprintf(stderr, "  assemble\n");
  fprintf(stderr, "  break\n");    // TODO(jawilson)
  fprintf(stderr, "  continue\n"); // TODO(jawilson)
  fprintf(stderr, "  disassemble\n");
  fprintf(stderr, "  examine\n");
  fprintf(stderr, "  load\n"); // TODO(jawilson)
  fprintf(stderr, "  save\n"); // TODO(jawilson)
  fprintf(stderr, "  step\n");
  fprintf(stderr, "  quit\n");
  fprintf(stderr, "  symbols\n");
  fprintf(stderr, "  unbreak\n");
}

void debug_quit_command(token_list *tokens) {
  fprintf(stderr, "The debugger has terminated execution. Exiting...\n");
  exit(0);
}

void debug_step_command(cpu_thread_state *state, token_list *tokens) {
  uint64_t amount = 1;
  if (token_list_length(tokens) >= 2) {
    amount = string_parse_uint64(token_list_get(tokens, 1));
  }
  uint64_t before_pc = state->pc;
  interpret(state, amount);
  if (amount == 1) {
    print_instructions(state->memory, before_pc, 1);
  }
  print_registers(state, 16, 16);
  // might need is_mapped...
  // if ((before_pc == state->pc) && (load8(state->memory, state->pc) == 0))
  // { break;
  // }
}

void debug_disassemble_command(cpu_thread_state *state, token_list *tokens) {
  uint64_t address = state->pc;
  if (token_list_length(tokens) > 1) {
    address = parse_address(state, token_list_get(tokens, 1));
  }
  uint64_t number_of_instructions = 16;
  if (token_list_length(tokens) > 2) {
    number_of_instructions = string_parse_uint64(token_list_get(tokens, 2));
  }
  print_instructions(state->memory, address, number_of_instructions);
}

void debug_assemble_command(cpu_thread_state *state, token_list *tokens) {
  char line[1024];
  array *statements = make_array(8);

  while (1) {
    fprintf(stderr, "(assembler) ");
    fgets(line, sizeof(line), stdin);
    if (string_equal(line, "\n")) {
      break;
    }
    statements = array_add(statements, (uint64_t)string_duplicate(line));
  }

  assembly_result asm_result =
      assemble_statements(state->memory, state->pc, state->symbols, statements);
  state->symbols = asm_result.symbols;
  fprintf(stderr, "assemble fragment is %lu bytes long\n",
          (asm_result.address_end - asm_result.address_start));

  for (int i = 0; i < array_length(statements); i++) {
    free((void *)array_get(statements, i));
  }
  free(statements);
}

void debug_address_command(cpu_thread_state *state, token_list *tokens) {
  if (token_list_length(tokens) < 2) {
    fprintf(stderr, "Error: not enough arguments (got %d tokens)",
            token_list_length(tokens));
    return;
  }
  char *symbol_name = token_list_get(tokens, 1);
  symbol *sym = find_symbol_by_name(state->symbols, symbol_name);
  if (sym != NULL) {
    fprintf(stderr, "address of %s is 0x%08x\n", sym->name,
            (uint32_t)sym->value);
  } else {
    fprintf(stderr, "%s was not found.\n", symbol_name);
  }
}

void debug_examine_command(cpu_thread_state *state, token_list *tokens) {
  uint64_t start_address = state->pc;
  if (token_list_length(tokens) >= 2) {
    // TODO(jawilson): allow symbols!
    start_address = string_parse_uint64(token_list_get(tokens, 1));
  }
  uint64_t amount = 16;
  if (token_list_length(tokens) >= 3) {
    amount = string_parse_uint64(token_list_get(tokens, 2));
  }

  print_data(state->memory, start_address, start_address + amount);
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
 * continue
 */
void debug_repl(cpu_thread_state *state) {
  char line[1024];

  while (1) {
    fputs("(debug) ", stderr);

    fgets(line, sizeof(line), stdin);

    token_list *tokens = tokenize(line, " \n");
    char *command = token_list_get(tokens, 0);

    if (string_equal(command, "help")) {
      debug_help_command(tokens);
    } else if (string_equal(command, "quit")) {
      debug_quit_command(tokens);
    } else if (string_equal(command, "step")) {
      debug_step_command(state, tokens);
    } else if (string_equal(command, "disassemble") ||
               string_starts_with(command, "dis")) {
      debug_disassemble_command(state, tokens);
    } else if (string_equal(command, "examine") ||
               string_starts_with(command, "x")) {
      debug_examine_command(state, tokens);
    } else if (string_equal(command, "assemble")) {
      debug_assemble_command(state, tokens);
    } else if (string_equal(command, "address")) {
      debug_address_command(state, tokens);
    } else if (string_equal(command, "symbols")) {
      print_symbol_table(state->symbols);
    } else {
      fprintf(stderr, "Uknown debug command. Ignoring.\n");
    }

    token_list_free_all(tokens);
  }
}
