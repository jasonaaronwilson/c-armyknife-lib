/**
 * @file terminal-input.c
 *
 * A little interactive test program for input from the terminal.
 */

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {
  value_array_t* FLAG_files = NULL;

  flag_program_name(argv[0]);
  flag_description(
      "A little interactive test program for testing terminal input");
  flag_file_args(&FLAG_files);

  char* error = flag_parse_command_line(argc, argv);
  if (error) {
    flag_print_help(stderr, error);
    exit(1);
  }

  if (FLAG_files->length > 0) {
    flag_print_help(stderr, error);
    exit(1);
  }

  struct termios oldt = term_echo_off();

  sleep(10);

  term_echo_restore(oldt);

  exit(0);
}
