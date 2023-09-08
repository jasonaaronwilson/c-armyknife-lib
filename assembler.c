#include <stdio.h>
#include <string.h>

#include "assembler.h"
#include "instruction-info.h"
#include "string-util.h"
#include "tokenizer.h"

/**
 * This is an interface for compiling a single instruction statement
 * to "memory" or updating the symbol table for a label.
 */

#include "opcodes.h"

assembly_result make_assembly_result(symbol_table *symbols, uint64_t address);

assembly_result assemble(paged_memory *memory, uint64_t address,
                         symbol_table *symbols, char *statement) {

  assembly_result result = make_assembly_result(symbols, address);

  token_list *tokens = tokenize(statement, " ,\n");

  if (tokens == NULL) {
    return result;
  }

  char *opcode = token_list_get(tokens, 0);
  if (string_ends_with(opcode, ":")) {
    char *label_name = string_substring(opcode, 0, strlen(opcode) - 1);
    fprintf(stderr, "DEBUG symbol name is '%s'\n", label_name);
    result.symbols = add_symbol(symbols, label_name, address);
    free(label_name);
    return result;
  }

  instruction_info *info = find_instruction_info_by_name(opcode);

  return result;
}

assembly_result make_assembly_result(symbol_table *symbols, uint64_t address) {
  assembly_result empty_statement_result;
  empty_statement_result.address_start = address;
  empty_statement_result.address_end = address;
  empty_statement_result.symbols = symbols;
  return empty_statement_result;
}
