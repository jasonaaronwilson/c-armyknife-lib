#include "assembler.h"

/**
 * This is an interface for compiling a single instruction statement
 * to "memory" or updating the symbol table for a label.
 */

#include "opcodes.h"

typedef struct {
  char *token;
  const char *remaining;
} tokenizer_result;

tokenizer_result tokenize(const char *str);

assembly_result assemble(paged_memory *memory, uint64_t address,
                         void *symbol_table, char *statement) {
  assembly_result result = {address, address, 0, 0};
  return result;
}
