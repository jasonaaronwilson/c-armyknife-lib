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

// result is a hashtable of string -> uint64_t
string_hashtable_t* initial_counts_hashtable() {
  // Currently hashtables don't grow so at least make sure we reduce
  // "scanning" by 1000% or more with a "large" initial hashtable
  // (which doesn't even seem that large...).
  string_hashtable_t* counts = make_string_hashtable(128 * 1024);
  return counts;
}

value_array_t* get_command_line_flag_descriptors() {
  value_array_t* result = make_value_array(1);
  value_array_add(result, ptr_to_value(make_command_line_flag_descriptor(
                              "use-tree", command_line_flag_type_boolean,
                              "When true, use a tree instead of a hashtable")));
  return result;
}

command_line_parser_configuation_t* get_command_line_parser_config() {
  command_line_parser_configuation_t* config
      = malloc_struct(command_line_parser_configuation_t);
  config->program_name = "line-histogram";
  config->program_description = "Creates a histogram of lines and their counts";
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

  string_hashtable_t* counts = initial_counts_hashtable();
  string_tree_t* counts_tree = NULL;

  for (int i = 0; i < args_and_files.files->length; i++) {
    char* file_name = value_array_get(args_and_files.files, i).str;
    buffer_t* buffer = make_buffer(1);
    buffer = buffer_append_file_contents(buffer, file_name);
    value_array_t* lines = buffer_tokenize(buffer, "\n");
    for (int j = 0; j < lines->length; j++) {
      char* line = value_array_get(lines, j).str;
      if (use_tree) {
        value_result_t find_result = string_tree_find(counts_tree, line);
        if (is_ok(find_result)) {
          counts_tree = string_tree_insert(counts_tree, line,
                                           u64_to_value(find_result.u64 + 1));
        } else {
          counts_tree = string_tree_insert(counts_tree, line, u64_to_value(1));
        }
      } else {
        value_result_t find_result = string_ht_find(counts, line);
        if (is_ok(find_result)) {
          counts = string_ht_insert(counts, line,
                                    u64_to_value(find_result.u64 + 1));
        } else {
          counts = string_ht_insert(counts, line, u64_to_value(1));
        }
      }
    }
  }

  if (use_tree) {
    // clang-format off
    string_tree_foreach(counts_tree, key, value, {
	fprintf(stdout, "%d:%s\n", value.u64 & 0xfffffff, key);
      });
    // clang-format on
  } else {
    // clang-format off
    string_ht_foreach(counts, key, value, {
	fprintf(stdout, "%d:%s\n", value.u64 & 0xfffffff, key);
      });
    // clang-format on
  }

  exit(0);
}
