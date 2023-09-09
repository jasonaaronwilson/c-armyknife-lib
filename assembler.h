#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <stdint.h>

#include "array.h"
#include "paged-memory.h"
#include "symbol-table.h"

typedef struct {
  uint64_t address_start;
  uint64_t address_end;
  symbol_table_t* symbols;
} assembly_result_t;

extern assembly_result_t assemble(paged_memory_t* memory, uint64_t addresss,
                                  symbol_table_t* symbols, char* statement);
extern assembly_result_t assemble_statements(paged_memory_t* memory,
                                             uint64_t addresss,
                                             symbol_table_t* symbols,
                                             array_t* statements);

#endif /* _ASSEMBLER_H_ */
