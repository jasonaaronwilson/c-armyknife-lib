#include "instruction-info.h"
#include "fatal-error.h"
#include "string-util.h"

uint8_t is_initialized;
instruction_info instruction_info_array[LAST_OPCODE];

void init_instruction_info() {
  for (int i = 0; i <= LAST_OPCODE; i++) {
    instruction_info_array[i].opcode_name = "unknown";
    instruction_info_array[i].number_of_arguments = 0;
  }

  instruction_info_array[BRK].opcode_name = "brk";
  instruction_info_array[BRK].number_of_arguments = 0;

  instruction_info_array[NOP].opcode_name = "nop";
  instruction_info_array[NOP].number_of_arguments = 0;

  instruction_info_array[BRZ].opcode_name = "brz";
  instruction_info_array[BRZ].number_of_arguments = 3;

  instruction_info_array[IMM].opcode_name = "imm";
  instruction_info_array[IMM].number_of_arguments = 2;

  instruction_info_array[SIMM].opcode_name = "simm";
  instruction_info_array[SIMM].number_of_arguments = 2;

  instruction_info_array[MOV].opcode_name = "mov";
  instruction_info_array[MOV].number_of_arguments = 2;

  instruction_info_array[LD_U8].opcode_name = "ldu8";
  instruction_info_array[LD_U8].number_of_arguments = 2;

  instruction_info_array[LD_U16].opcode_name = "ldu16";
  instruction_info_array[LD_U16].number_of_arguments = 2;

  instruction_info_array[LD_U32].opcode_name = "ldu32";
  instruction_info_array[LD_U32].number_of_arguments = 2;

  instruction_info_array[LD_U64].opcode_name = "ldu64";
  instruction_info_array[LD_U64].number_of_arguments = 2;

  instruction_info_array[LD_S8].opcode_name = "lds8";
  instruction_info_array[LD_S8].number_of_arguments = 2;

  instruction_info_array[LD_S16].opcode_name = "lds16";
  instruction_info_array[LD_S16].number_of_arguments = 2;

  instruction_info_array[LD_S32].opcode_name = "lds32";
  instruction_info_array[LD_S32].number_of_arguments = 2;

  // LD_S64 doesn't exist because LD_U64 would do the same thing

  instruction_info_array[CLZ].opcode_name = "clz";
  instruction_info_array[CLZ].number_of_arguments = 2;

  instruction_info_array[CTZ].opcode_name = "ctz";
  instruction_info_array[CTZ].number_of_arguments = 2;

  instruction_info_array[POPCNT].opcode_name = "popcnt";
  instruction_info_array[POPCNT].number_of_arguments = 2;

  instruction_info_array[ADD].opcode_name = "add";
  instruction_info_array[ADD].number_of_arguments = 3;

  instruction_info_array[SUB].opcode_name = "sub";
  instruction_info_array[SUB].number_of_arguments = 3;

  instruction_info_array[MUL].opcode_name = "mul";
  instruction_info_array[MUL].number_of_arguments = 3;

  instruction_info_array[DIV_U].opcode_name = "div_u";
  instruction_info_array[DIV_U].number_of_arguments = 3;

  instruction_info_array[DIV_S].opcode_name = "div_s";
  instruction_info_array[DIV_S].number_of_arguments = 3;

  instruction_info_array[REM_U].opcode_name = "rem_u";
  instruction_info_array[REM_U].number_of_arguments = 3;

  instruction_info_array[REM_S].opcode_name = "rem_s";
  instruction_info_array[REM_S].number_of_arguments = 3;

  instruction_info_array[AND].opcode_name = "and";
  instruction_info_array[AND].number_of_arguments = 3;

  instruction_info_array[OR].opcode_name = "or";
  instruction_info_array[OR].number_of_arguments = 3;

  instruction_info_array[XOR].opcode_name = "xor";
  instruction_info_array[XOR].number_of_arguments = 3;

  instruction_info_array[SHL].opcode_name = "shl";
  instruction_info_array[SHL].number_of_arguments = 3;

  instruction_info_array[SHR_U].opcode_name = "shr_u";
  instruction_info_array[SHR_U].number_of_arguments = 3;

  instruction_info_array[SHR_S].opcode_name = "shr_s";
  instruction_info_array[SHR_S].number_of_arguments = 3;

  is_initialized = 1;
}

instruction_info *get_instruction_info(uint64_t opcode) {
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
