/**
 * @file debug-repl.c
 *
 * This file implements the repl for the built-in debugger.
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "byte-array.h"
#include "debug-repl.h"
#include "environment.h"
#include "evaluator.h"
#include "fatal-error.h"
#include "global-environment.h"
#include "interpreter.h"
#include "io.h"
#include "printer.h"
#include "reader.h"
#include "string-util.h"
#include "symbol-table.h"
#include "tagged-reference.h"
#include "tokenizer.h"

environment_t* make_debugger_env(environment_t* global_env);

/**
 * Parse an address for a command.
 *
 * Currently address must be either a constant number or a symbol.
 */
uint64_t parse_address(cpu_thread_state_t* state, char* str) {
  symbol_t* sym = find_symbol_by_name(state->symbols, str);
  if (!string_is_null_or_empty(sym->name)) {
    return sym->value;
  }
  return string_parse_uint64(str);
}

/**
 * Show help for the debugger.
 */
void debug_help_command(token_list_t* tokens) {
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

/**
 * Exit the VM.
 */
void debug_quit_command(token_list_t* tokens) {
  fprintf(stderr,
          "The user executed the debugger's quit command which "
          "terminates the VM.\nExiting early but with status code 0.\n");
  exit(0);
}

/**
 * Step one instruction by default. If an argument provided, it is
 * used to determine how many instructions to step.
 */
void debug_step_command(cpu_thread_state_t* state, token_list_t* tokens) {
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

void debug_disassemble_command(cpu_thread_state_t* state,
                               token_list_t* tokens) {
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

// TODO(jawilson): refactor to share code with assemble-file

void debug_assemble_command(cpu_thread_state_t* state, token_list_t* tokens) {
  char line[1024];
  array_t* statements = make_array(8);

  while (1) {
    fprintf(stderr, "(assembler) ");
    fgets(line, sizeof(line), stdin);
    if (string_equal(line, "\n") || string_starts_with(line, "#end-assemble")) {
      break;
    }
    statements = array_add(statements, (uint64_t) string_duplicate(line));
  }

  assembly_result_t asm_result = assemble_statements(
      state->memory, state->pc, state->symbols, statements);
  state->symbols = asm_result.symbols;
  fprintf(stderr, "assemble fragment is %lu bytes long\n",
          (asm_result.address_end - asm_result.address_start));

  for (int i = 0; i < array_length(statements); i++) {
    free((void*) array_get(statements, i));
  }
  free(statements);
}

void debug_assemble_file_command(cpu_thread_state_t* state,
                                 token_list_t* tokens) {
  array_t* statements = make_array(8);

  byte_array_t* contents = make_byte_array(1024);
  contents
      = byte_array_append_file_contents(contents, token_list_get(tokens, 1));

  uint64_t line_start = 0;
  while (line_start < byte_array_length(contents)) {
    int i = line_start;
    for (; (i < byte_array_length(contents)); i++) {
      uint8_t byte = byte_array_get(contents, i);
      if (byte == '\n') {
        i++;
        break;
      }
    }
    if (i > line_start) {
      char* str = (char*) byte_array_c_substring(contents, line_start, i);
      if (str == NULL) {
        fatal_error(ERROR_UKNOWN);
      }
      statements = array_add(statements, (uint64_t) str);
      line_start = i;
    }
  }

  free(contents);

  assembly_result_t asm_result = assemble_statements(
      state->memory, state->pc, state->symbols, statements);
  state->symbols = asm_result.symbols;
  fprintf(stderr, "assemble fragment is %lu bytes long\n",
          (asm_result.address_end - asm_result.address_start));

  for (int i = 0; i < array_length(statements); i++) {
    free((void*) array_get(statements, i));
  }

  free(statements);
}

void debug_address_command(cpu_thread_state_t* state, token_list_t* tokens) {
  if (token_list_length(tokens) < 2) {
    fprintf(stderr, "Error: not enough arguments (got %d tokens)",
            token_list_length(tokens));
    return;
  }
  char* symbol_name = token_list_get(tokens, 1);
  symbol_t* sym = find_symbol_by_name(state->symbols, symbol_name);
  if (sym != NULL) {
    fprintf(stderr, "address of %s is 0x%08x\n", sym->name,
            (uint32_t) sym->value);
  } else {
    fprintf(stderr, "%s was not found.\n", symbol_name);
  }
}

void debug_examine_command(cpu_thread_state_t* state, token_list_t* tokens) {
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

void debug_print_register_command(cpu_thread_state_t* state,
                                  token_list_t* tokens) {
  char* register_name = token_list_get(tokens, 1);
  if (string_starts_with(register_name, "r")) {
    uint64_t reg_num = parse_gr_argument(register_name);
    print_gr_register_name_and_value(state, reg_num);
    fprintf(stderr, "\n");
  } else {
    fprintf(stderr, "register named %s was not understood.\n", register_name);
  }
}

/**
 * Exit the process immediately with if the register is not the expected
 * value.
 */
void debug_expect_register_command(cpu_thread_state_t* state,
                                   token_list_t* tokens) {
  char* register_name = token_list_get(tokens, 1);
  if (string_starts_with(register_name, "r")) {
    uint64_t reg_num = parse_gr_argument(register_name);
    uint64_t expected = string_parse_uint64(token_list_get(tokens, 2));
    uint64_t value = state->register_storage[reg_num];
    if (value != expected) {
      fprintf(stderr,
              "FAIL: expected %s to be %" PRIu64 "but was %" PRIu64 "\n",
              register_name, value, expected);
      exit(ERROR_DEBUGGER_EXPECT_FAILURE);
    }
  }
}

/**
 * Read one or more lines from stdin.
 */
byte_array_t* read_expression_lines(char* prompt) {
  byte_array_t* result = make_byte_array(128);
  fputs(prompt, stderr);

  char line[1024];
  fgets(line, sizeof(line), stdin);

  // TODO(jawilson): read more lines if necessary to finish an
  // expression.

  result = byte_array_append_bytes(result, (uint8_t*) line, strlen(line));
  return result;
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
void debug_repl(cpu_thread_state_t* state) {

  environment_t* env = make_global_environment(); // make_debugger_env();
  environment_define(env, "*thread-state*",
                     tagged_reference(TAG_CPU_THREAD_STATE_T, state));

  while (1) {
    byte_array_t* input_array = read_expression_lines("(debug) ");
    char* input = byte_array_c_substring(input_array, 0,
                                         byte_array_length(input_array));

    if (0) {
      tagged_reference_t expr = read_expression(input, 0);
      byte_array_t* output = make_byte_array(128);
      output = print_tagged_reference_to_byte_arary(output, expr);
      continue;
    }

    token_list_t* tokens = tokenize(input, " \n");

    if (token_list_length(tokens) == 0) {
      continue;
    }

    char* command = token_list_get(tokens, 0);

    if (string_equal(command, "help")) {
      debug_help_command(tokens);
    } else if (string_equal(command, "quit")) {
      debug_quit_command(tokens);
    } else if (string_equal(command, "step")) {
      debug_step_command(state, tokens);
    } else if (string_equal(command, "disassemble")
               || string_starts_with(command, "dis")) {
      debug_disassemble_command(state, tokens);
    } else if (string_equal(command, "examine")
               || string_starts_with(command, "x")) {
      debug_examine_command(state, tokens);
    } else if (string_equal(command, "assemble")) {
      debug_assemble_command(state, tokens);
    } else if (string_equal(command, "address")) {
      debug_address_command(state, tokens);
    } else if (string_equal(command, "symbols")) {
      print_symbol_table(state->symbols);
    } else if (string_equal(command, "assemble-file")) {
      debug_assemble_file_command(state, tokens);
    } else if (string_equal(command, "print-register")) {
      debug_print_register_command(state, tokens);
    } else if (string_equal(command, "expect-register")) {
      debug_expect_register_command(state, tokens);
    } else {
      fprintf(stderr, "Uknown debug command. Ignoring.\n");
    }

    token_list_free_all(tokens);
  }
}

void add_debugger_repl_commands(environment_t* env);

environment_t* make_debugger_env(environment_t* global_env) {
  environment_t* result = make_environment(global_env);
  add_debugger_repl_commands(result);
  return result;
}

void add_debugger_repl_commands(environment_t* env) {}
