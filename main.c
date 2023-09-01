#include <stdlib.h>
#include "interpreter.h"

/**
 * This is a simple main routine for the interpreter.
 */
int main(int argc, char **argv) {

  int number_of_int_registers = 4096;
  int number_of_fp_registers = 4096;
  int memory_amount = 4096 * 128;

  cpu_thread_state *state = (cpu_thread_state *)(malloc(
      sizeof(cpu_thread_state) + number_of_int_registers * 8 +
      number_of_fp_registers * 8));

  exit(0);
}
