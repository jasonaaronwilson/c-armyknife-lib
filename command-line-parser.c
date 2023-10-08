#line 2 "command-line-parser.c"
/**
 * @file command-line-parser.c
 *
 * A simple command line parser for GNU long style flags and file
 * arguments.
 *
 * Takes a command line and returns an array of "file" arguments and a
 * string_hashtable of command line flags.
 */

#ifndef _COMMAND_LINE_PARSER_H_
#define _COMMAND_LINE_PARSER_H_

struct command_line_parse_result_S {
  char* program;
  string_hashtable_t* flags;
  array_t(char*) * files;
};

typedef struct command_line_parse_result_S command_line_parse_result_t;

extern command_line_parse_result_t parse_command_line(int argc, char** argv);

#endif /* _COMMAND_LINE_PARSER_H_ */

/**
 * Given a command line such as:
 *
 * --count=10 --type=bar --no-arg file1.c file2.c
 *
 * Returns a map containing 3 keys and an array containing to values.
 *
 * The map: "count" -> "10", "type" -> "bar", "no-arg" -> ""
 * The array: "file1.c" "file2.c"
 */
command_line_parse_result_t parse_command_line(int argc, char** argv) {
  array_t(uint64_t)* files = make_array(char_ptr_type(), 16);
  string_hashtable_t* flags = make_string_hashtable(32);

  boolean_t parse_flags = true;
  for (int i = 1; i < argc; i++) {
    char* arg = argv[i];

    if (parse_flags) {
      if (string_starts_with(arg, "--")) {
        if (string_equal(arg, "--")) {
          parse_flags = true;
          continue;
        }
        int equal_sign_index = string_index_of_char(arg, '=');

        char* key = "";
        char* value = "";
        if (equal_sign_index >= 0) {
          key = string_substring(arg, 2, equal_sign_index);
          value = string_substring(arg, equal_sign_index + 1, strlen(arg));
        } else {
          key = string_substring(arg, 2, strlen(arg));
        }

        flags = string_ht_insert(flags, key, value);

        continue;
      }
    }

    files = array_add(files, reference_of_char_ptr(&arg));
  }

  return (command_line_parse_result_t){
      .program = argv[0],
      .flags = flags,
      .files = files,
  };
}
