#include "interpreter.h"
#include <stdlib.h>

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

  // Single step

  while (1) {
    // print_instructions(state->memory_start, 1);
    interpret(state, 1);
  }

  exit(0);
}
