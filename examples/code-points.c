/**
 * @file line-histogram.c
 *
 * This program reads one or more files specified on the command line
 * and counts the number of occurrences of each line (except empty
 * ones).
 */

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {
  // Flag Parsing

  boolean_t FLAG_use_tree = false;
  value_array_t* FLAG_files = NULL;

  flag_program_name(argv[0]);
  flag_description("Reads a utf8 encoded file and outputs the code-points");
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

  // The main program.
  buffer_t* buffer = make_buffer(1);
  buffer = buffer_append_all(buffer, stdin);
  uint8_t* str = (uint8_t*) buffer_to_c_string(buffer);
  int offset = 0;
  while (offset < buffer_length(buffer)) {
    utf8_decode_result_t result = utf8_decode_two(&str[offset]);
    if (result.error) {
      log_fatal("An error was encountered at file byte offset %d", offset);
      fatal_error(ERROR_ILLEGAL_INPUT);
    }
    fprintf(stdout, "%08x\n", result.code_point);
    offset += result.num_bytes;
  }

  exit(0);
}
