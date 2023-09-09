#include <stdio.h>
#include <string.h>

#include "array.h"
#include "assembler.h"
#include "fatal-error.h"
#include "instruction-info.h"
#include "string-util.h"
#include "tokenizer.h"
#include "uleb128.h"

/**
 * This is an interface for compiling a single instruction statement
 * to "memory" or updating the symbol table for a label.
 */

#include "opcodes.h"

assembly_result make_assembly_result(symbol_table* symbols, uint64_t address);

uint64_t parse_imm_argument(symbol_table* symbols, char* str);
uint64_t parse_gr_argument(char* str);
uint64_t parse_fp_argument(char* str);

uint64_t parse_argument(uint8_t type, symbol_table* symbols, char* str) {
  switch (type) {
  case ARG_TYPE_GR:
    return parse_gr_argument(str);

  case ARG_TYPE_FP:
    return parse_fp_argument(str);

  case ARG_TYPE_IMM:
    return parse_imm_argument(symbols, str);
  }
  // TODO(jawilson): error!
  return 0;
}

assembly_result assemble_statements(paged_memory* memory, uint64_t address,
                                    symbol_table* symbols, array* statements) {
  uint64_t start_address = address;

  while (1) {
    address = start_address;
    for (int i = 0; i < array_length(statements); i++) {
      assembly_result result = assemble(memory, address, symbols,
                                        (char*) array_get(statements, i));
      address = result.address_end;
      symbols = result.symbols;
    }
    if (!is_dirty(symbols)) {
      break;
    }
  }

  assembly_result result = make_assembly_result(symbols, start_address);
  result.address_end = address;
  return result;
}

assembly_result assemble(paged_memory* memory, uint64_t address,
                         symbol_table* symbols, char* statement) {

  assembly_result result = make_assembly_result(symbols, address);

  if (string_starts_with(statement, "#")) {
    return result;
  }

  token_list* tokens = tokenize(statement, " ,\n");

  if (tokens == NULL) {
    return result;
  }

  char* opcode = token_list_get(tokens, 0);
  if (string_ends_with(opcode, ":")) {
    char* label_name = string_substring(opcode, 0, strlen(opcode) - 1);
    fprintf(stderr, "DEBUG symbol name is '%s'\n", label_name);
    result.symbols = add_symbol(symbols, label_name, address);
    free(label_name);
    return result;
  }

  if (string_starts_with(opcode, ".")) {
    fprintf(stderr, "WARNING: ignoring directive '%s'\n", opcode);
    return result;
  }

  instruction_info* info = find_instruction_info_by_name(opcode);
  if (info == NULL) {
    fprintf(stderr, "WARNING: opcode not found '%s'\n", opcode);
    return result;
  }

  address += encodeULEB128(memory, address, info->opcode_value);

  if (info->number_of_arguments >= 1) {
    uint64_t value
        = parse_argument(info->arg0_type, symbols, token_list_get(tokens, 1));
    address += encodeULEB128(memory, address, value);
  }
  if (info->number_of_arguments >= 2) {
    uint64_t value
        = parse_argument(info->arg1_type, symbols, token_list_get(tokens, 2));
    address += encodeULEB128(memory, address, value);
  }
  if (info->number_of_arguments >= 3) {
    uint64_t value
        = parse_argument(info->arg2_type, symbols, token_list_get(tokens, 3));
    address += encodeULEB128(memory, address, value);
  }

  result.address_end = address;
  return result;
}

assembly_result make_assembly_result(symbol_table* symbols, uint64_t address) {
  assembly_result empty_statement_result;
  empty_statement_result.address_start = address;
  empty_statement_result.address_end = address;
  empty_statement_result.symbols = symbols;
  return empty_statement_result;
}

uint64_t parse_imm_argument(symbol_table* symbols, char* str) {
  if (str[0] >= '0' && str[0] <= '9') {
    string_parse_uint64(str);
  }
  symbol* sym = find_symbol_by_name(symbols, str);
  if (sym) {
    return sym->value;
  } else {
    return 0;
  }
}

uint64_t parse_gr_argument(char* str) {
  if (!string_starts_with(str, "r")) {
    fatal_error(ERROR_EXPECTED_GENERAL_REGISTER);
  }
  return string_parse_uint64(&str[1]);
}

uint64_t parse_fp_argument(char* str) {
  if (!string_starts_with(str, "f")) {
    fatal_error(ERROR_EXPECTED_FLOATING_REGISTER);
  }
  return string_parse_uint64(&str[1]);
}
