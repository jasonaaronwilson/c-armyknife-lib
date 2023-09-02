#include <stdlib.h>

#include "interpreter.h"
#include "opcodes.h"
#include "printer.h"

void add_sample_program(cpu_thread_state *state, uint64_t address);

/**
 * This is a simple main routine for the interpreter.
 */
int main(int argc, char **argv) {

  int number_of_int_registers = 4096;
  int number_of_fp_registers = 4096;
  uint64_t memory_amount = 4096 * 128;
  uint64_t start_pc = 0x0;

  cpu_thread_state *state = (cpu_thread_state *)(malloc(
      sizeof(cpu_thread_state) + number_of_int_registers * 8 +
      number_of_fp_registers * 8));

  state->pc = start_pc;
  state->num_integer_regs = number_of_int_registers;
  state->num_fp_regs = number_of_fp_registers;
  state->memory_start = malloc(memory_amount);
  state->memory_end = state->memory_start + memory_amount;

  add_sample_program(state, 0);

  // Single step N times

  for (int i = 0; i < 2; i++) {
    print_instructions(state->memory_start, state->pc, 1);
    interpret(state, 1);
  }

  exit(0);
}

void add_sample_program(cpu_thread_state *state, uint64_t address) {
  state->memory_start[address++] = MOV;
  state->memory_start[address++] = GR1;
  state->memory_start[address++] = GR0;
  state->memory_start[address++] = BRK;
}
