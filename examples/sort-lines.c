/**
 * @file sort-lines.c
 *
 * This program reads one or more files specified on the command line
 * and "cats" them together but in sorted order (with duplicate and
 * empty lines removed).
 *
 * This is similar to "sort" on Unix except:
 */

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

value_array_t* get_command_line_flag_descriptors() {
  value_array_t* result = make_value_array(1);
  return result;
}

command_line_parser_configuation_t* get_command_line_parser_config() {
  command_line_parser_configuation_t* config
      = malloc_struct(command_line_parser_configuation_t);
  config->program_name = "unique-file-lines";
  config->program_description
      = "Similar to 'sort' but duplicate 'lines' are elided.";
  config->command_descriptors = NULL;
  config->flag_descriptors = get_command_line_flag_descriptors();
  return config;
}

int main(int argc, char** argv) {
  command_line_parse_result_t args_and_files
      = parse_command_line(argc, argv, get_command_line_parser_config());

  // I'm not sure how this could even happen but cheap fast fail code,
  // especially that which may be cargo culted, doesn't upset me
  // unless inside an "inner" loop.
  if (args_and_files.command != NULL) {
    fatal_error(ERROR_BAD_COMMAND_LINE);
  }

  boolean_t use_tree = is_ok(string_ht_find(args_and_files.flags, "use-tree"));
  if (use_tree) {
    fprintf(stderr, "Using a tree instead of a hash-table");
  }

  string_tree_t* sorted = NULL;

  for (int i = 0; i < args_and_files.files->length; i++) {
    char* file_name = value_array_get(args_and_files.files, i).str;
    buffer_t* buffer = make_buffer(1);
    buffer = buffer_append_file_contents(buffer, file_name);
    value_array_t* lines = buffer_tokenize(buffer, "\n");
    for (int j = 0; j < lines->length; j++) {
      char* line = value_array_get(lines, j).str;
      sorted = string_tree_insert(sorted, line, boolean_to_value(true));
    }
  }

  // clang-format off
  string_tree_foreach(sorted, key, value_ignored, {
      fprintf(stdout, "%s\n", key);
    });
  // clang-format on

  exit(0);
}
