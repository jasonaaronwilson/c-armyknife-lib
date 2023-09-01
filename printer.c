#include <stdio.h>

#include "instruction-info.h"
#include "opcodes.h"
#include "printer.h"
#include "uleb128.h"

void print_instructions(const uint8_t *memory, uint64_t address,
                        uint64_t number_of_instructions) {
  while (number_of_instructions > 0) {
    number_of_instructions--;
    // FIXME(jawilson): end address
    unsigned_decode_result opcode =
        decodeULEB128(memory + address, memory + address + 16);
    instruction_info *info = get_instruction_info(opcode.number);
    fprintf(stderr, "    %s\n", info->opcode_name);
  }
}
