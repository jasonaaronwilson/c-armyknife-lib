#include "opcodes.h"

typedef struct {
  char *opcode_name;
  uint8_t number_of_arguments; // includes "targets"
} instruction_info;

extern uint8_t is_initialized;
extern instruction_info info[LAST_OPCODE];

extern get_info(uint64_t opcode) {
  if (opcode <= LAST_OPCODE) {
    return &info[opcode];
  } else {
    exit(1);
  }
}
