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

  token_list *tokens = tokenize(statement, " ,");

  if (tokens == NULL) {
    return result;
  }

  char *opcode = token_list_get(tokens, 0);
  if (string_ends_with(opcode, ":")) {
    // TODO(jawilson): remove trailing ":"
    symbol *sym = find_symbol_by_name(symbols, opcode);
    if (sym == NULL) {
      result.symbols_modified++;
    } else if (sym->value != address) {
      result.symbols_modified++;
      result.symbols = add_symbol(symbols, opcode, address);
    }
    return result;
  }

  instruction_info *info = find_instruction_info_by_name(opcode);

  return result;
}

assembly_result make_assembly_result(symbol_table *symbols, uint64_t address) {
  assembly_result empty_statement_result;
  empty_statement_result.address_start = address;
  empty_statement_result.address_end = address;
  empty_statement_result.symbols_unknown = 0;
  empty_statement_result.symbols_modified = 0;
  empty_statement_result.symbols = symbols;
  return empty_statement_result;
}
