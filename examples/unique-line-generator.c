/**
 * @file unique-line-generator.c
 *
 * This program outputs lines containing (mostly) unique
 * numbers. Using a simple for loop these lines would appear in the
 * expected order from smallest to largest which is rather predictable
 * and thus not very useful as a testing input for testing certain
 * algorithms.
 *
 * While many solutions exist, we will begin with just using a PRNG
 * (psuedo-random number generator) to generate N numbers (modulo a
 * maximum number) and assume they are unique. For short sequences and
 * very large maximums, this is probably true but longer sequences
 * with small maximums will accidentally generate more duplicate
 * lines. This isn't a problem with the PRNG since we would expect the
 * same from a truly random number generator.
 */

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

value_array_t* get_command_line_flag_descriptors() {
  value_array_t* result = make_value_array(1);
  value_array_add(result,
                  ptr_to_value(make_command_line_flag_descriptor(
                      "number-of-lines", command_line_flag_type_unsigned,
                      "Number of lines to generate")));
  value_array_add(result, ptr_to_value(make_command_line_flag_descriptor(
                              "max-number", command_line_flag_type_unsigned,
                              "The maximum number to generate")));
  return result;
}

command_line_parser_configuation_t* get_command_line_parser_config() {
  command_line_parser_configuation_t* config
      = malloc_struct(command_line_parser_configuation_t);
  config->program_name = "unique-line-generator";
  config->program_description
      = "Generate large numbers of unique lines (sent to stdout)";
  config->command_descriptors = NULL;
  config->flag_descriptors = get_command_line_flag_descriptors();
  return config;
}

int main(int argc, char** argv) {
  configure_fatal_errors((fatal_error_config_t){
      .catch_sigsegv = true,
  });
  command_line_parse_result_t args_and_files
      = parse_command_line(argc, argv, get_command_line_parser_config());

  // I'm not sure how this could even happen but cheap fast fail code,
  // especially that which may be cargo culted, doesn't upset me
  // unless inside an "inner" loop.
  if (args_and_files.command != NULL) {
    fatal_error(ERROR_BAD_COMMAND_LINE);
  }

  uint64_t number_of_lines
      = string_parse_uint64(
            string_ht_find(args_and_files.flags, "number-of-lines").str)
            .u64;
  uint64_t max_number
      = string_parse_uint64(
            string_ht_find(args_and_files.flags, "max-number").str)
            .u64;

  fprintf(stdout, "# number-of-lines=%d max-number=%d\n",
          number_of_lines & 0xfffffff, max_number & 0xfffffff);

  random_state_t state = random_state_for_test();

  for (int i = 0; i < number_of_lines; i++) {
    uint64_t random_n = random_next_uint64_below(&state, max_number);
    fprintf(stdout, "%d\n", random_n & 0xfffffff);
  }

  exit(0);
}
