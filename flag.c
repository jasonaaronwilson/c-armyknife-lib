#line 2 "flag.c"
/**
 * @file flag.c
 *
 * A simple command line parser for commands, flags, and "file"
 * arguments with a declarative style configuration.
 *
 * There are two general modes for parsing. One permits "commands"
 * (allowing an interface similar to "git", "apt", "yum" and "docker")
 * and other tools and the more traditional non command mode. For now,
 * commands should not begin with a "-" character.
 *
 * In traditional mode, the configuration and then the actual parsing
 * might be as simple as:
 *
 * ```
 *   // Often these would be global variables...
 *   boolean_t FLAG_verbose = true; // default value
 *   array_t* FLAG_file_args = make_value_array(1);
 *
 *   flag_program_name("oarchive");
 *   flag_boolean("--verbose", &FLAG_verbose);
 *   flag_file_args(&FLAG_files);
 *
 *   buffer_t* error = parse_command_file(false, argc, argv);
 *   if (error) {
 *     // Automatically generated based on the current configuration.
 *     flag_print_help(error);
 *     exit(1);
 *   }
 * ```
 *
 * To make the auto generated help more helpful, a description can be
 * added to the program or the last flag declared using
 * flag_description(). To make the program easier to use, you can also
 * define alaises for flags (and commands).
 *
 * Parsing git style command based command lines are equally
 * easy. After all of the globally applicable flags have been
 * "declared", simply use flag_command(). You can then add a
 * description for the command as well as defining the flags that
 * should only apply when a particular command was seen. Obviously
 * flag_command() can be called multiple times. Sometimes it will be
 * necesary to define the same flag on multiple commands (say if 2/3
 * commands accept the flag but the other one doesn't). You should be
 * able to refactor the declaration code to make this somewhat more
 * pleasant.
 *
 * Note that once any command is provided, if a command is not
 * provided then an error will be returned while parsing so it is
 * currently not possible to sometimes use commands and sometimes not
 * use them. This may be a may benefit. For additional consistency,
 * the command is always required to be the first argument on the
 * command line, i.e., before any flags or "files".
 *
 * Besides the obvious command line types, it's possible to add flags
 * for enums (really just named integers) and to even use custom value
 * parsers in the future.
 *
 * This interface is not thread safe but generally a program will
 * define and parse command line arguments in the initial thread
 * before spawning threads so this isn't a terrible restriction.
 *
 * TODO(jawilson): strict mode and custom parsers.
 */

#ifndef _FLAG_H_
#define _FLAG_H_

/**
 * @enum command_line_flag_type_t
 */
typedef enum {
  command_line_flag_type_boolean,
  command_line_flag_type_string,
  command_line_flag_type_uint64,
  command_line_flag_type_int64,
  command_line_flag_type_double,
  command_line_flag_type_enum,
  command_line_flag_type_custom,
} command_line_flag_type_t;

struct program_descriptor_S {
  char* name;
  char* description;
  string_tree_t* flags;
  string_tree_t* commands;
}

struct command_descriptor_S {
  program_descriptor_t* program;
  char* name;
  char* description;
  char* write_back_ptr;
  string_tree_t* flags;
};

struct flag_descriptor_S {
  char* name;
  command_line_flag_type_t arg_type;
  char* help_string;
  void* write_back_ptr;
  // TODO(jawilson): add custom parser call back (and call back data).
};

extern void flag_program_name(char* name);
extern void flag_file_args(value_array_t** write_back_ptr);

extern void flag_boolean(char* name, boolean_t* write_back_ptr);
extern void flag_string(char* name, boolean_t* write_back_ptr);
extern void flag_uint64(char* name, boolean_t* write_back_ptr);
extern void flag_int64(char* name, boolean_t* write_back_ptr);
// TODO(jawilson): flag_enum, and flag_custom
extern void flag_description(char* description);
extern void flag_alias(char* alias);
extern buffer_t* flag_parse_command_line(int argc, char** argv);

#endif /* _FLAG_H_ */

program_descriptor_t* program;
command_descriptor_t* current_command;
flag_descriptor_t* current_flag;

/**
 * @function make_command_line_command_descriptor
 *
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
 * @function make_command_line_flag_descriptor
 *
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

_Noreturn void show_usage(command_line_parser_configuation_t* config,
                          char* message) {
  if (message) {
    fprintf(stdout, "%s\n\n", message);
  }
  if (config->command_descriptors) {
    fprintf(stdout, "Usage: %s <command> <flags*> <files*>\n",
            config->program_name);
  } else {
    fprintf(stdout, "Usage: %s <flags*> <files*>\n", config->program_name);
  }
  if (config->program_description) {
    fprintf(stdout, "\n%s\n", config->program_description);
  }
  if (config->command_descriptors) {
    fprintf(stdout, "\nCommands:\n");
    for (int i = 0; i < config->command_descriptors->length; i++) {
      command_line_command_descriptor_t* command_descriptor
          = value_array_get(config->command_descriptors, i).ptr;
      fprintf(stdout, "    %s [[%s]]\n", command_descriptor->name,
              command_descriptor->help_string);
    }
  }
  if (config->flag_descriptors) {
    fprintf(stdout, "\nFlags:\n");
    for (int i = 0; i < config->flag_descriptors->length; i++) {
      command_line_flag_descriptor_t* flag
          = value_array_get(config->flag_descriptors, i).ptr;
      fprintf(stdout, "    --%s=<flag-value> (%s)\n", flag->long_name,
              flag->help_string);
    }
  }
  fatal_error(ERROR_BAD_COMMAND_LINE);
}

/**
 * @function parse_command_line
 *
 * Given a command line such as:
 *
 * --count=10 --type=bar --no-arg file1.c file2.c
 *
 * Returns a map containing 3 keys and an array containing two values
 * (file1.c and file2.c)
 *
 * The map: "count" -> "10", "type" -> "bar", "no-arg" -> ""
 * The array: "file1.c" "file2.c"
 */
command_line_parse_result_t
    parse_command_line(int argc, char** argv,
                       command_line_parser_configuation_t* config) {

  value_array_t* command_descriptors = config->command_descriptors;
  value_array_t* flag_descriptors = config->flag_descriptors;


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
            // show_usage(string_printf("Unrecognized flag: %s\n", arg));
            show_usage(config, "Unrecognized flag");
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
      show_usage(config, "Must supply a command");
    }

    command = argv[1];

    if (command && string_starts_with(command, "-")) {
      show_usage(config, "Flags should not appear before a command.");
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
      show_usage(config, "Unrecognized command");
    }
  }

  return (command_line_parse_result_t){
      .program = argv[0],
      .command = command,
      .flags = flags,
      .files = files,
  };
}
