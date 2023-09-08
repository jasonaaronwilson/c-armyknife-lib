#ifndef _INSTRUCTION_INFO_
#define _INSTRUCTION_INFO_

#include <stdint.h>
#include <stdlib.h>

#include "opcodes.h"

typedef struct {
  char *opcode_name;
  uint8_t number_of_arguments; // includes "targets"
  uint8_t is_fp;
} instruction_info;

extern uint8_t is_initialized;
extern instruction_info instruction_info_array[LAST_OPCODE];

extern void init_instruction_info();
// TODO(jawilson): rename
extern instruction_info *get_instruction_info(uint64_t opcode);
extern instruction_info *find_instruction_info_by_name(const char *name);

#endif /* _INSTRUCTION_INFO_ */
