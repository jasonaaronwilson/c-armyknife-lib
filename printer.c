#include <inttypes.h>
#include <stdio.h>

#include "instruction-info.h"
#include "opcodes.h"
#include "printer.h"
#include "uleb128.h"

// Forward Declaration
uint64_t print_instruction(paged_memory_t* memory, uint64_t address);

uint64_t print_instructions(paged_memory_t* memory, uint64_t address,
                            uint64_t number_of_instructions) {
  while (number_of_instructions-- > 0) {
    address = print_instruction(memory, address);
  }
  return address;
}

void print_instruction_argument(uint8_t type, uint64_t value) {
  switch (type) {
  case ARG_TYPE_GR:
    fprintf(stderr, "r%lu", value & 0xffffffff);
    break;
  case ARG_TYPE_FP:
    fprintf(stderr, "f%lu", value & 0xffffffff);
    break;
  case ARG_TYPE_IMM:
    fprintf(stderr, "0x%" PRIx64, value);
    break;
  }
}

uint64_t print_instruction(paged_memory_t* memory, uint64_t address) {
  fprintf(stderr, "%08lx", (address & 0xffffffff));

  unsigned_decode_result opcode = decodeULEB128(memory, address);
  address += opcode.size;

  instruction_info_t* info = find_instruction_info_by_opcode(opcode.number);

  // TODO(jawilson): check result...

  fprintf(stderr, "    %s", info->opcode_name);

  if (info->number_of_arguments >= 1) {
    fprintf(stderr, " ");
    unsigned_decode_result arg = decodeULEB128(memory, address);
    address += arg.size;
    print_instruction_argument(info->arg0_type, arg.number);
  }

  if (info->number_of_arguments >= 2) {
    fprintf(stderr, ",");
    unsigned_decode_result arg = decodeULEB128(memory, address);
    address += arg.size;
    print_instruction_argument(info->arg1_type, arg.number);
  }

  if (info->number_of_arguments >= 3) {
    fprintf(stderr, ",");
    unsigned_decode_result arg = decodeULEB128(memory, address);
    address += arg.size;
    print_instruction_argument(info->arg2_type, arg.number);
  }
  fprintf(stderr, "\n");

  return address;
}

void print_registers(cpu_thread_state_t* state, int num_gr_registers,
                     int num_fp_registers) {
  for (int i = 0; i < num_gr_registers; i++) {
    if ((i > 0) && ((i & 3) == 0)) {
      fprintf(stderr, "\n");
    } else if (i > 0) {
      fprintf(stderr, " ");
    }
    fprintf(stderr, "gr%02d ", i);
    fprintf(stderr, "%08lx", ((state->register_storage[i]) >> 32) & 0xffffffff);
    fprintf(stderr, "%08lx", (state->register_storage[i]) & 0xffffffff);
  }
  fprintf(stderr, "\n");

  // TODO(jawilson): print out floating point registers too.
  for (int i = 0; i < num_fp_registers; i++) {
  }
}

void print_data(paged_memory_t* memory, uint64_t start_address,
                uint64_t end_address) {
  for (uint64_t address = start_address; address < end_address; address++) {
    fprintf(stderr, "%08lx %02x\n", (address & 0xffffffff),
            load8(memory, address));
  }
}

void print_symbol_table(symbol_table* symbols) {
  while (symbols) {
    fprintf(stderr, "%08lx %s\n", symbols->sym.value, symbols->sym.name);
    symbols = symbols->next;
  }
}
