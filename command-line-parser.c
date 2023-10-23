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

struct command_line_command_descriptor_S {
  char* name;
  char* help_string;
};

typedef struct command_line_command_descriptor_S
    command_line_command_descriptor_t;

typedef enum {
  command_line_flag_type_string,
  command_line_flag_type_boolean,
  command_line_flag_type_unsigned,
  command_line_flag_type_signed,
  command_line_flag_type_double,
} command_line_flag_type_t;

struct command_line_flag_descriptor_S {
  char* long_name;
  command_line_flag_type_t arg_type;
  char* help_string;
};

typedef struct command_line_flag_descriptor_S command_line_flag_descriptor_t;

struct command_line_parse_result_S {
  char* program;
  char* command;
  string_hashtable_t* flags;
  value_array_t* files;
};

typedef struct command_line_parse_result_S command_line_parse_result_t;

extern command_line_command_descriptor_t*
    make_command_line_command_descriptor(char* name, char* help_string);

extern command_line_flag_descriptor_t* make_command_line_flag_descriptor(
    char* long_name, command_line_flag_type_t arg_type, char* help_string);

extern command_line_parse_result_t
    parse_command_line(int argc, char** argv,
                       value_array_t* command_descriptors,
                       value_array_t* flag_descriptors);

#endif /* _COMMAND_LINE_PARSER_H_ */

/**
 * Allocate a command_line_command_descriptor_t and fill in it's most
 * common fields.
 */
command_line_command_descriptor_t*
    make_command_line_command_descriptor(char* name, char* help_string) {
  command_line_command_descriptor_t* result
      = malloc_struct(command_line_command_descriptor_t);
  result->name = name;
  result->help_string = help_string;
  return result;
}

/**
 * Allocate a command_line_flag_descriptor_t and fill in it's most
 * common fields.
 */
command_line_flag_descriptor_t* make_command_line_flag_descriptor(
    char* long_name, command_line_flag_type_t arg_type, char* help_string) {
  command_line_flag_descriptor_t* result
      = malloc_struct(command_line_flag_descriptor_t);
  result->long_name = long_name;
  result->arg_type = arg_type;
  result->help_string = help_string;
  return result;
}

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
command_line_parse_result_t
    parse_command_line(int argc, char** argv,
                       value_array_t* command_descriptors,
                       value_array_t* flag_descriptors) {
  value_array_t* files = make_value_array(argc);
  string_hashtable_t* flags = make_string_hashtable(32);

  boolean_t parse_flags = true;
  for (int i = (command_descriptors != NULL ? 2 : 1); i < argc; i++) {
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

        if (flag_descriptors != NULL) {
          boolean_t found = false;
          for (int j = 0; j < flag_descriptors->length; j++) {
            command_line_flag_descriptor_t* cl_flag
                = value_array_get(flag_descriptors, j).ptr;
            if (string_equal(cl_flag->long_name, key)) {
              found = true;
              break;
            }
          }
          if (!found) {
            log_fatal("Unrecognized flag: %s\n", arg);
            fatal_error(ERROR_BAD_COMMAND_LINE);
          }
        }

        flags = string_ht_insert(flags, key, str_to_value(value));

        continue;
      }
    }

    value_array_add(files, str_to_value(arg));
  }

  char* command = NULL;
  if (command_descriptors != NULL) {
    if (argc < 2) {
      fatal_error(ERROR_BAD_COMMAND_LINE);
    }

    command = argv[1];

    if (command && string_starts_with(command, "-")) {
      fprintf(stdout, "Flags should not appear before a command.");
      fatal_error(ERROR_BAD_COMMAND_LINE);
    }

    boolean_t found_command = false;
    for (int i = 0; i < command_descriptors->length; i++) {
      command_line_command_descriptor_t* command_descriptor
          = value_array_get(command_descriptors, i).ptr;
      if (string_equal(command, command_descriptor->name)) {
        found_command = true;
        break;
      }
    }
    if (!found_command) {
      fatal_error(ERROR_BAD_COMMAND_LINE);
    }
  }

  return (command_line_parse_result_t){
      .program = argv[0],
      .command = command,
      .flags = flags,
      .files = files,
  };
}
