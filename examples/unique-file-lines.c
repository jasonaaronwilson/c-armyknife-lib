/**
 * @file unique-file-lines.c
 *
 * This program reads one or more files specified on the command line
 * and "cats" them together but with duplicate (and empty) lines
 * removed (in some ways very similar to the histogram program we will
 * eventually provide).
 *
 * This is similar to using cat + sort + uniq on Unix except:
 *
 * 1. The input remains in the orginal order

 * 2. While I'm not sure how much memory *sort* uses on your flavor of
 * unix, uniq only has to keep two lines in memory at once, since it
 * only compares adjacent lines, so it's unfair to only compare us to
 * "uniq" without also comparing us to "sort". For this program,
 * "unique-file-lines", you can assume we use at least as much memory
 * as would be needed to keep all of the input files in memory.
 *
 * Besides potentially being useful, this tool stresses a bunch of
 * parts of c-arymknife-lib and since this program can easily be
 * written in other languages, it provides some potential for
 * benchmarking.
 *
 * While not currently implemented, probabilistic approaches could be
 * much more space efficient (and potentially more performant). For
 * example, with a decent quality 64bit hash, every "seen" line could
 * be represented in about 8 bytes and could use a better "set"
 * abstraction than what our library provides (since we assume you can
 * use a hashmap mapped to true/false which wastes space).
 *
 * The problem is that with some small probability, a non duplicate
 * line will be determined to be a duplicate. An even greater space
 * savings may be possible with Bloom filters, which will also
 * incorrectly classify some non duplicate lines as duplicates.
 */

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

// result is a hashtable of strings -> true
string_hashtable_t* initial_seen_hashtable() {
  // Currently hashtables don't grow so at least make sure we reduce
  // "scanning" by 1000% or more with a "large" initial hashtable
  // (which doesn't even seem that large...).
  string_hashtable_t* seen = make_string_hashtable(128 * 1024);
  return seen;
}

value_array_t* get_command_line_flag_descriptors() {
  value_array_t* result = make_value_array(1);
  // TODO(jawilson): make sure this also works when null!
  return result;
}

command_line_parser_configuation_t* get_command_line_parser_config() {
  command_line_parser_configuation_t* config
      = malloc_struct(command_line_parser_configuation_t);
  config->program_name = "unique-file-lines";
  config->program_description
      = "Similar to 'cat' but duplicate 'lines' are elided.";
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

  string_hashtable_t* seen = initial_seen_hashtable();

  for (int i = 0; i < args_and_files.files->length; i++) {
    char* file_name = value_array_get(args_and_files.files, i).str;
    buffer_t* buffer = make_buffer(1);
    buffer = buffer_append_file_contents(buffer, file_name);
    value_array_t* lines = buffer_tokenize(buffer, "\n");
    for (int j = 0; j < lines->length; j++) {
      char* line = value_array_get(lines, j).str;
      value_result_t find_result = string_ht_find(seen, line);
      if (!is_ok(find_result)) {
        seen = string_ht_insert(seen, line, boolean_to_value(true));
        fprintf(stdout, "%s\n", line);
      }
    }
  }

  exit(0);
}
