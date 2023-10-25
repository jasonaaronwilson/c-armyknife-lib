///
/// Test out some of the features of command line parsing.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

value_array_t* get_command_line_command_descriptors() {
  value_array_t* result = make_value_array(1);
  value_array_add(result, ptr_to_value(make_command_line_command_descriptor(
                              "a-command", "a random name for a command")));
  value_array_add(result,
                  ptr_to_value(make_command_line_command_descriptor(
                      "another-command", "another random name for a command")));
  return result;
}

value_array_t* get_command_line_flag_descriptors() {
  value_array_t* result = make_value_array(2);
  value_array_add(
      result, ptr_to_value(make_command_line_flag_descriptor(
                  "foo", command_line_flag_type_string, "sample flag foo")));
  value_array_add(result, ptr_to_value(make_command_line_flag_descriptor(
                              "bar", command_line_flag_type_string,
                              "another same flag named bar")));
  return result;
}

command_line_parser_configuation_t* get_command_line_parser_config() {
  command_line_parser_configuation_t* config
      = malloc_struct(command_line_parser_configuation_t);
  config->program_name = "command-line-parser-test";
  config->program_description
      = "This is a simple test of the armyknife command line parser.";
  config->command_descriptors = get_command_line_command_descriptors();
  config->flag_descriptors = get_command_line_flag_descriptors();
  return config;
}

int main(int argc, char** argv) {
  command_line_parse_result_t args_and_files
      = parse_command_line(argc, argv, get_command_line_parser_config());

  fprintf(stdout, "command is %s\n", args_and_files.command);

  value_result_t value = string_ht_find(args_and_files.flags, "foo");
  fprintf(stdout, "foo is %s\n", value.str);

  value = string_ht_find(args_and_files.flags, "bar");
  fprintf(stdout, "bar is %s\n", value.str);

  for (int i = 0; i < args_and_files.files->length; i++) {
    fprintf(stdout, "bar is %s\n",
            value_array_get(args_and_files.files, i).str);
  }

  exit(0);
}
