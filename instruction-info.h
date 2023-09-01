#ifndef _INSTRUCTION_INFO_
#define _INSTRUCTION_INFO_

#include <stdint.h>
#include <stdlib.h>

#include "opcodes.h"

typedef struct {
  char *opcode_name;
  uint8_t number_of_arguments; // includes "targets"
} instruction_info;

extern uint8_t is_initialized;
extern instruction_info info[LAST_OPCODE];

extern void init_instruction_info();

inline instruction_info *get_info(uint64_t opcode) {
  if (!is_initialized) {
    init_instruction_info();
  }
  if (opcode <= LAST_OPCODE) {
    return &info[opcode];
  } else {
    // TODO(jawilson): create a better error mechanism...
    exit(1);
  }
}

#endif /* _INSTRUCTION_INFO_ */
