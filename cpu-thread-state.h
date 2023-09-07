#ifndef _CPU_THREAD_STATE_
#define _CPU_THREAD_STATE_

#include "paged-memory.h"
#include "symbol-table.h"
#include <stdint.h>

typedef struct {
  // This is the program counter.
  uint64_t pc;

  // A sparse memory space.
  paged_memory *memory;

  // Symbols associated with paged_memory
  symbol_table *symbols;

  // The number of integer registers that have been allocated for this
  // thread
  uint32_t num_integer_regs;

  // The number of floating point registers that have been allocated
  // for this thread
  uint32_t num_fp_regs;

  // Contains storage for the integer and floating point registers
  // (integer registers come first). Note: this must come last in the
  // cpu_state structure.
  uint64_t register_storage[0];
} cpu_thread_state;

#endif /* _CPU_THREAD_STATE_ */
