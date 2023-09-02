#include <inttypes.h>
#include <stdio.h>

#include "instruction-info.h"
#include "opcodes.h"
#include "printer.h"
#include "uleb128.h"

// Forward Declaration
uint64_t print_instruction(const uint8_t *memory, uint64_t address);

uint64_t print_instructions(const uint8_t *memory, uint64_t address,
                            uint64_t number_of_instructions) {
  while (number_of_instructions-- > 0) {
    address = print_instruction(memory, address);
  }
  return address;
}

uint64_t print_instruction(const uint8_t *memory, uint64_t address) {
  fprintf(stderr, "0x%" PRIx64, address);

  // FIXME(jawilson): end address
  unsigned_decode_result opcode =
      decodeULEB128(memory + address, memory + address + 16);
  address += opcode.size;

  unsigned_decode_result arg1;
  unsigned_decode_result arg2;
  unsigned_decode_result arg3;

  instruction_info *info = get_instruction_info(opcode.number);

  if (info->number_of_arguments >= 1) {
    arg1 = decodeULEB128(memory + address, memory + address + 16);
    address += arg1.size;
  }
  if (info->number_of_arguments >= 2) {
    arg2 = decodeULEB128(memory + address, memory + address + 16);
    address += arg2.size;
  }
  if (info->number_of_arguments >= 3) {
    arg3 = decodeULEB128(memory + address, memory + address + 16);
    address += arg3.size;
  }

  if (opcode.number == IMM) {
    fprintf(stderr, "    %s gr%lu,0x%" PRIx64 "\n", info->opcode_name,
            arg1.number, arg2.number);
  } else if (opcode.number == SIMM) {
    fprintf(stderr, "    %s gr%lu,0x%" PRIx64 "\n", info->opcode_name,
            arg1.number, arg2.number);
  } else if (info->is_fp) {
    switch (info->number_of_arguments) {
    case 0:
      fprintf(stderr, "    %s\n", info->opcode_name);
      break;
    case 1:
      fprintf(stderr, "    %s fp%lu\n", info->opcode_name, arg1.number);
      break;
    case 2:
      fprintf(stderr, "    %s fp%lu,fp%lu\n", info->opcode_name, arg1.number,
              arg2.number);
      break;
    case 3:
      fprintf(stderr, "    %s fp%lu,fp%lu,fp%lu\n", info->opcode_name,
              arg1.number, arg2.number, arg3.number);
      break;
    default:
      break;
    }
  } else {
    switch (info->number_of_arguments) {
    case 0:
      fprintf(stderr, "    %s\n", info->opcode_name);
      break;
    case 1:
      fprintf(stderr, "    %s gr%lu\n", info->opcode_name, arg1.number);
      break;
    case 2:
      fprintf(stderr, "    %s gr%lu,gr%lu\n", info->opcode_name, arg1.number,
              arg2.number);
      break;
    case 3:
      fprintf(stderr, "    %s gr%lu,gr%lu,gr%lu\n", info->opcode_name,
              arg1.number, arg2.number, arg3.number);
      break;
    default:
      break;
    }
  }

  return address;
}
