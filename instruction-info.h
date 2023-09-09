#ifndef _INSTRUCTION_INFO_
#define _INSTRUCTION_INFO_

#include <stdint.h>
#include <stdlib.h>

#include "opcodes.h"

#define ARG_TYPE_NONE 0
#define ARG_TYPE_GR 1
#define ARG_TYPE_FP 2
#define ARG_TYPE_IMM 3

typedef struct {
  char* opcode_name;
  uint64_t opcode_value;
  uint8_t number_of_arguments; // includes "targets"
  uint8_t arg0_type;
  uint8_t arg1_type;
  uint8_t arg2_type;
} instruction_info_t;

extern uint8_t is_initialized;
extern instruction_info_t instruction_info_array[LAST_OPCODE];

extern void init_instruction_info();
// TODO(jawilson): rename
extern instruction_info_t* find_instruction_info_by_opcode(uint64_t opcode);
extern instruction_info_t* find_instruction_info_by_name(const char* name);

#endif /* _INSTRUCTION_INFO_ */
