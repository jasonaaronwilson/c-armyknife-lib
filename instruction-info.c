#include "instruction-info.h"
#include "fatal-error.h"
#include "string-util.h"

uint8_t is_initialized;
instruction_info instruction_info_array[LAST_OPCODE];

void add_instruction_info(char *opcode_name, int opcode_number, int num_args) {
  instruction_info_array[opcode_number].opcode_name = "brk";
  instruction_info_array[opcode_number].opcode_value = opcode_number;
  instruction_info_array[opcode_number].number_of_arguments = num_args;
}

void init_instruction_info() {
  for (int i = 0; i <= LAST_OPCODE; i++) {
    instruction_info_array[i].opcode_name = "unknown";
    instruction_info_array[i].number_of_arguments = 0;
  }

  add_instruction_info("brk", BRK, 0);
  add_instruction_info("nop", NOP, 0);
  add_instruction_info("brz", BRZ, 3);
  add_instruction_info("imm", IMM, 2);
  add_instruction_info("simm", SIMM, 2);
  add_instruction_info("mov", MOV, 2);
  add_instruction_info("ldu8", LD_U8, 2);
  add_instruction_info("ldu16", LD_U16, 2);
  add_instruction_info("ldu32", LD_U32, 2);
  add_instruction_info("ldu64", LD_U64, 2);
  add_instruction_info("lds8", LD_S8, 2);
  add_instruction_info("lds16", LD_S16, 2);
  add_instruction_info("lds32", LD_S32, 2);
  // add_instruction_info("lds64", LD_S64, 2);

  add_instruction_info("clz", CLZ, 2);
  add_instruction_info("ctz", CTZ, 2);
  add_instruction_info("popcnt", POPCNT, 2);
  add_instruction_info("add", ADD, 3);
  add_instruction_info("sub", SUB, 3);
  add_instruction_info("mul", MUL, 3);
  add_instruction_info("div_u", DIV_U, 3);
  add_instruction_info("div_s", DIV_S, 3);
  add_instruction_info("rem_u", REM_U, 3);
  add_instruction_info("rem_s", REM_S, 3);
  add_instruction_info("and", AND, 3);
  add_instruction_info("or", OR, 3);
  add_instruction_info("xor", XOR, 3);
  add_instruction_info("shl", SHL, 3);
  add_instruction_info("shr_u", SHR_U, 3);
  add_instruction_info("shr_s", SHR_S, 3);

  is_initialized = 1;
}

instruction_info *find_instruction_info_by_opcode(uint64_t opcode) {
  if (!is_initialized) {
    init_instruction_info();
  }
  if (opcode <= LAST_OPCODE) {
    return &instruction_info_array[opcode];
  } else {
    fatal_error(ERROR_OPCODE_UNKNOWN);
  }
}

instruction_info *find_instruction_info_by_name(const char *name) {
  if (!is_initialized) {
    init_instruction_info();
  }

  for (int i = 0; i < LAST_OPCODE; i++) {
    if (string_equal(instruction_info_array[i].opcode_name, name)) {
      return &instruction_info_array[i];
    }
  }

  return NULL;
}
