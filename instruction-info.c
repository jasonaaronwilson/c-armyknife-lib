#include "instruction-info.h"

uint8_t is_initialized;
instruction_info instruction_info_array[LAST_OPCODE];

void init_instruction_info() {
  // HERE: loop over and unitialized with unknown...

  instruction_info_array[BRK].opcode_name = "brk";
  instruction_info_array[BRK].number_of_arguments = 0;

  instruction_info_array[MOV].opcode_name = "mov";
  instruction_info_array[MOV].number_of_arguments = 2;

  is_initialized = 1;
}

instruction_info *get_instruction_info(uint64_t opcode) {
  if (!is_initialized) {
    init_instruction_info();
  }
  if (opcode <= LAST_OPCODE) {
    return &instruction_info_array[opcode];
  } else {
    // TODO(jawilson): create a better error mechanism...
    exit(1);
  }
}
