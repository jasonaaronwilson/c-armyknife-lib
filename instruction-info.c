#include "instruction-info.h"
#include "fatal-error.h"
#include "string-util.h"

uint8_t is_initialized;
instruction_info_t instruction_info_array[LAST_OPCODE];

void add_instruction_info_0(char* opcode_name, int opcode_number) {
  instruction_info_array[opcode_number].opcode_name = opcode_name;
  instruction_info_array[opcode_number].opcode_value = opcode_number;
  instruction_info_array[opcode_number].number_of_arguments = 0;
}

void add_instruction_info_1(char* opcode_name, int opcode_number,
                            uint8_t arg0_type) {
  add_instruction_info_0(opcode_name, opcode_number);
  instruction_info_array[opcode_number].arg0_type = arg0_type;
  instruction_info_array[opcode_number].number_of_arguments = 1;
}

void add_instruction_info_2(char* opcode_name, int opcode_number,
                            uint8_t arg0_type, uint8_t arg1_type) {
  add_instruction_info_1(opcode_name, opcode_number, arg0_type);
  instruction_info_array[opcode_number].arg1_type = arg1_type;
  instruction_info_array[opcode_number].number_of_arguments = 2;
}

void add_instruction_info_3(char* opcode_name, int opcode_number,
                            uint8_t arg0_type, uint8_t arg1_type,
                            uint8_t arg2_type) {
  add_instruction_info_2(opcode_name, opcode_number, arg0_type, arg1_type);
  instruction_info_array[opcode_number].arg2_type = arg2_type;
  instruction_info_array[opcode_number].number_of_arguments = 3;
}

void init_instruction_info() {
  for (int i = 0; i <= LAST_OPCODE; i++) {
    add_instruction_info_0("unknown", i);
  }

  add_instruction_info_0("brk", BRK);
  add_instruction_info_0("nop", NOP);
  add_instruction_info_3("brz", BRZ, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("imm", IMM, ARG_TYPE_GR, ARG_TYPE_IMM);
  add_instruction_info_2("simm", SIMM, ARG_TYPE_GR, ARG_TYPE_IMM);
  add_instruction_info_2("mov", MOV, ARG_TYPE_GR, ARG_TYPE_GR);

  add_instruction_info_2("ldu8", LD_U8, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("ldu16", LD_U16, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("ldu32", LD_U32, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("ldu64", LD_U64, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("lds8", LD_S8, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("lds16", LD_S16, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("lds32", LD_S32, ARG_TYPE_GR, ARG_TYPE_GR);

  add_instruction_info_2("clz", CLZ, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("ctz", CTZ, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_2("popcnt", POPCNT, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("add", ADD, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("sub", SUB, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("mul", MUL, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("div_u", DIV_U, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("div_s", DIV_S, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("rem_u", REM_U, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("rem_s", REM_S, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("and", AND, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("or", OR, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("xor", XOR, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("shl", SHL, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("shr_u", SHR_U, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);
  add_instruction_info_3("shr_s", SHR_S, ARG_TYPE_GR, ARG_TYPE_GR, ARG_TYPE_GR);

  is_initialized = 1;
}

instruction_info_t* find_instruction_info_by_opcode(uint64_t opcode) {
  if (!is_initialized) {
    init_instruction_info();
  }
  if (opcode <= LAST_OPCODE) {
    return &instruction_info_array[opcode];
  } else {
    fatal_error(ERROR_OPCODE_UNKNOWN);
  }
}

instruction_info_t* find_instruction_info_by_name(const char* name) {
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
