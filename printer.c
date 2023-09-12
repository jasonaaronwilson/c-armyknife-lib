/**
 * @file printer.c
 *
 * This file contains all kinds of routines print stuff.
 */

#include <inttypes.h>
#include <stdio.h>

#include "byte-array.h"
#include "instruction-info.h"
#include "opcodes.h"
#include "pair.h"
#include "printer.h"
#include "string-util.h"
#include "tagged-reference.h"
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

  unsigned_decode_result_t opcode = decodeULEB128(memory, address);
  address += opcode.size;

  instruction_info_t* info = find_instruction_info_by_opcode(opcode.number);

  // TODO(jawilson): check result...

  fprintf(stderr, "    %s", info->opcode_name);

  if (info->number_of_arguments >= 1) {
    fprintf(stderr, " ");
    unsigned_decode_result_t arg = decodeULEB128(memory, address);
    address += arg.size;
    print_instruction_argument(info->arg0_type, arg.number);
  }

  if (info->number_of_arguments >= 2) {
    fprintf(stderr, ",");
    unsigned_decode_result_t arg = decodeULEB128(memory, address);
    address += arg.size;
    print_instruction_argument(info->arg1_type, arg.number);
  }

  if (info->number_of_arguments >= 3) {
    fprintf(stderr, ",");
    unsigned_decode_result_t arg = decodeULEB128(memory, address);
    address += arg.size;
    print_instruction_argument(info->arg2_type, arg.number);
  }
  fprintf(stderr, "\n");

  return address;
}

void print_gr_register_name_and_value(cpu_thread_state_t* state,
                                      uint64_t reg_number) {
  fprintf(stderr, "r%02lu ", reg_number);
  fprintf(stderr, "%08lx",
          ((state->register_storage[reg_number]) >> 32) & 0xffffffff);
  fprintf(stderr, "%08lx", (state->register_storage[reg_number]) & 0xffffffff);
}

void print_registers(cpu_thread_state_t* state, int num_gr_registers,
                     int num_fp_registers) {
  for (int i = 0; i < num_gr_registers; i++) {
    if ((i > 0) && ((i & 3) == 0)) {
      fprintf(stderr, "\n");
    } else if (i > 0) {
      fprintf(stderr, " ");
    }
    print_gr_register_name_and_value(state, i);
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

void print_symbol_table(symbol_table_t* symbols) {
  while (symbols) {
    fprintf(stderr, "%08lx %s\n", symbols->sym.value, symbols->sym.name);
    symbols = symbols->next;
  }
}

byte_array_t*
    print_tagged_reference_to_byte_arary(byte_array_t* destination,
                                         tagged_reference_t reference) {
  char* prefix = NULL;
  char* str = NULL;
  char* suffix = NULL;
  char buffer[64];

  switch (reference.tag) {
  case TAG_NULL:
    str = "()";
    break;

  case TAG_PAIR_T:
    // Use dot notation (which we can't even parse ourselves)
    byte_array_append_byte(destination, '(');
    destination = print_tagged_reference_to_byte_arary(
        destination, untag_pair(reference)->head);
    byte_array_append_byte(destination, ' ');
    byte_array_append_byte(destination, '.');
    byte_array_append_byte(destination, ' ');
    destination = print_tagged_reference_to_byte_arary(
        destination, untag_pair(reference)->tail);
    break;

  case TAG_STRING:
    prefix = "\"";
    str = untag_string(reference);
    suffix = "\"";
    break;

  case TAG_READER_SYMBOL:
    str = untag_reader_symbol(reference);
    break;

  case TAG_UINT64_T:
    snprintf(buffer, sizeof(buffer), "%lu", untag_uint64_t(reference));
    str = &buffer[0];
    break;

  case TAG_ERROR_T:
    prefix = "#<error-code-";
    snprintf(buffer, sizeof(buffer), "%lu", reference.data);
    str = &buffer[0];
    suffix = ">";
    break;

  case TAG_BOOLEAN_T:
    if (is_false(reference)) {
      str = "#f";
    }
    if (is_true(reference)) {
      str = "#t";
    } else {
      str = "#<illegal-boolean-value>";
    }
    break;

  case TAG_PRIMITIVE:
    str = "#<primitive-procedure>";
    break;

  case TAG_CPU_THREAD_STATE_T:
    str = "#<thread-state>";
    break;
  }

  return destination;
}
