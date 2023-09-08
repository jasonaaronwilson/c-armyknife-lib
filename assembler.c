#include <stdio.h>
#include <string.h>

#include "assembler.h"
#include "instruction-info.h"
#include "string-util.h"
#include "tokenizer.h"
#include "uleb128.h"

/**
 * This is an interface for compiling a single instruction statement
 * to "memory" or updating the symbol table for a label.
 */

#include "opcodes.h"

assembly_result make_assembly_result(symbol_table *symbols, uint64_t address);
uint64_t parse_argument(symbol_table *symbols, char *str);

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

  if (string_starts_with(opcode, ".")) {
    fprintf(stderr, "WARNING: ignoring directive '%s'\n", opcode);
    return result;
  }

  instruction_info *info = find_instruction_info_by_name(opcode);
  if (info == NULL) {
    fprintf(stderr, "WARNING: opcode not found '%s'\n", opcode);
    return result;
  }

  address += encodeULEB128(memory, address, info->opcode_value);
  for (int i = 0; i < info->number_of_arguments; i++) {
    uint64_t value = parse_argument(symbols, token_list_get(tokens, i + 1));
    address += encodeULEB128(memory, address, value);
  }

  result.address_end = address;
  return result;
}

assembly_result make_assembly_result(symbol_table *symbols, uint64_t address) {
  assembly_result empty_statement_result;
  empty_statement_result.address_start = address;
  empty_statement_result.address_end = address;
  empty_statement_result.symbols = symbols;
  return empty_statement_result;
}

uint64_t parse_uint64(const char *string) {
  uint64_t integer = 0;
  uint64_t digit;

  while (*string != '\0') {
    digit = *string - '0';
    integer = integer * 10 + digit;
    string++;
  }

  return integer;
}

uint64_t parse_argument(symbol_table *symbols, char *str) {
  if (string_starts_with(str, "%gr")) {
    return parse_uint64(&str[3]);
  }
  return parse_uint64(str);
}
