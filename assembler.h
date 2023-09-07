#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include "paged-memory.h"
#include <stdint.h>

typedef struct {
  uint64_t address_start;
  uint64_t address_end;
  int symbols_unknown;
  int symbols_modified;
} assembly_result;

extern assembly_result assemble(paged_memory *memory, uint64_t addresss,
                                void *symbol_table, char *statement);

#endif /* _ASSEMBLER_H_ */
