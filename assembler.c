#include "assembler.h"

#include "opcodes.h"

typedef struct {
  char *token;
  const char *remaining;
} tokenizer_result;

tokenizer_result tokenize(const char *str);

uint64_t assemble(char **statements, uint8_t *memory_start, uint8_t *memory_end,
                  uint64_t memory_start_address) {
  memory_start[0] = BRK;
  return 1;
}
