#include "instruction-info.h"

uint8_t is_initialized;
instruction_info info[LAST_OPCODE];

void init_instruction_info() {
  // HERE: loop over and unitialized with unknown...

  info[BRK].opcode_name = "brk";
  info[BRK].number_of_arguments = 0;

  is_initialized = 1;
}
