#line 2 "flag.c"
/**
 * @file flag.c
 *
 * A command line parser for flags (and the uninterpreted "file"
 * arguments, aka "left-over" arguments). We use a pragmatic
 * declarative configuration and unless you need to localize the
 * result right now, we also automatically generate the "usage" /
 * "help" documentation (via flag_print_help).
 *
 * Here is maybe the smallest example you might possibly use:
 *
 * ```
 *   // For simple tools, just use "global" variaables but you can place
 *   // stuff wherever you choose, for example an a struct you can pass around.
 *
 *   boolean_t FLAG_verbose = true; // default value
 *   array_t* FLAG_file_args = NULL;
 *
 *   flag_program_name("program-name");
 *   flag_boolean("--verbose", &FLAG_verbose);
 *   flag_file_args(&FLAG_files);
 *
 *   char* error = flag_parse_command_line(argc, argv);
 *   if (error) {
 *     flag_print_help(error);
 *     exit(1);
 *   }
 * ```
 *
 * To make the auto generated help be the most useful, a human
 * readable description can be added to the last mentioned "thing" via
 * flag_description().
 *
 * To make your program "easier" to use, you can also define aliases
 * for certain flags (or commands!).
 *
 * Another capability is Using "sub-commands" so that your tool can be
 * a little more like "git", "apt", "yum" or "docker") versus more
 * traditional command line tools. We are a little more specific about
 * where the sub-command should be located in the command (it *must*
 * currently be in the first position). You can easily work around my
 * opinionated stance in several ways for example by "bubbling up"
 * anything that looks like one of your commands to the beginning of
 * the argument array passed to flag_parse_command_line. The
 * sub-command functionality is automatically enabled once you define
 * *any* sub-command. It's possible to create flags that only apply
 * when the correct sub-command is used.
 *
 * Obviously flag_command() (just like flag_<type> can be called)
 * multiple times to define multiple "sub commands" using different
 * command names.
 *
 * Aliases for flags and commands are also supported.
 *
 * Besides the obvious basic types like integers and strings, it's
 * possible to add flags for enums (really just named integers with a
 * passed in sizeof) and eventually custom value parsers so that lists
 * and maps can be command line arguments. Want to parse a date? Even
 * though I'm not providing it, you can do it and catch errors before
 * something unexpected happens.
 *
 * The interface presented here is *not* thread safe but generally a
 * program will define and parse command line arguments in the initial
 * thread before spawning threads so this isn't a terrible
 * restriction.
 *
 * Currently, flags and "left-over" arguments are not allowed to be
 * freely mixed. Once something that doesn't start with a "-" is seen,
 * all remaining arguments are treated as "left-over" arguments. The
 * special flag "--" can be used by the user to seperate things that
 * might otherwise be misinterpred.
 *
 * Note that when an error is returned, some of the "write backs" may
 * have been partially applied changing the default value they may
 * already contain. Unless you are running a test, the best option
 * will be to show help and exit to prevent continuing execution from
 * an undefined state.
 *
 * I think adding a "switch" type may make sense since we currently
 * don't support flags like "-abCdefG" where each each letter is a
 * different flag alias nor do we support "--no-" long format which is
 * sometimes used to "negate" a switch.
 *
 * TODO(jawilson): strict mode and custom parsers.
 * TODO(jawilson): allow stdin, stdout, and stderr deescriptions.
 * TODO(jawilson): allow limits on numeric arguments.
 * TODO(jawilson): allow "switches" which are like boolean flags but
 * don't require an argument. --bubble-gum, --no-bubble-gum.
 */

#ifndef _FLAG_H_
#define _FLAG_H_

/**
 * @enum flag_type_t
 */
typedef enum {
  flag_type_none,
  flag_type_boolean,
  // TODO(jawilson): flag_type_switch,
  flag_type_string,
  flag_type_uint64,
  flag_type_int64,
  flag_type_double,
  flag_type_enum,
  flag_type_custom,
} flag_type_t;

struct program_descriptor_S {
  char* name;
  char* description;
  string_tree_t* flags;
  string_tree_t* commands;
  value_array_t** write_back_file_args_ptr;
};



struct command_descriptor_S {
  program_descriptor_t* program;
  char* name;
  char* description;
  char* write_back_ptr;
  value_array_t** write_back_file_args_ptr;
  string_tree_t* flags;
};

struct flag_descriptor_S {
  char* name;
  flag_type_t flag_type;
  char* help_string;
  void* write_back_ptr;
  // TODO(jawilson): add custom parser call back (and call back data).
};

extern void flag_program_name(char* name);
extern void flag_description(char* description);
extern void flag_file_args(value_array_t** write_back_ptr);

extern void flag_boolean(char* name, boolean_t* write_back_ptr);
extern void flag_string(char* name, boolean_t* write_back_ptr);
extern void flag_uint64(char* name, boolean_t* write_back_ptr);
extern void flag_int64(char* name, boolean_t* write_back_ptr);
// TODO(jawilson): flag_enum(name, size), and flag_custom
extern void flag_alias(char* alias);

extern char* flag_parse_command_line(int argc, char** argv);

#endif /* _FLAG_H_ */

program_descriptor_t* current_program;
command_descriptor_t* current_command;
flag_descriptor_t* current_flag;

/**
 * @function flag_program_name
 *
 * Starts the command line configuration builder process and sets the
 * program name to be used in the automatically generated help string.
 */
void flag_program_name(char* name) {
  current_program = malloc_struct(program_descriptor_t);
  current_program->name = name;
  current_command = NULL;
  current_flag = NULL;
}

/**
 * @function flag_description
 *
 * Sets a description for the last "thing" "started".
 */
void flag_description(char* description) {
  if (current_flag != NULL) {
    current_flag->description = description;
  } else if (current_command != NULL) {
    current_command->description = description;
  } else if (current_program != NULL) {
    current_program->description = description;
  } else {
    log_fatal("A current flag, program or command must be executed first");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
}

/**
 * @function flag_file_args
 *
 * Set where to write "left-over" arguments (which are usually file
 * paths, wild-cards, etc.) should be written. Any pointer to the
 * write back array is completely replaced with a new array so you
 * will typically initialize it to just NULL and everything just
 * works.
 *
 * Unless you have a very good reason call this sometime after calling
 * flag_program_name but not before you start processing "children" of
 * the program node.
 */
void flag_file_args(value_array_t** write_back_file_args_ptr) {
  if (current_command != NULL) {
    current_command->write_back_file_args_ptr = write_back_file_args_ptr;
  } else if (current_program != NULL) {
    current_program->write_back_file_args_ptr = write_back_file_args_ptr;
  } else {
    log_fatal("A current program or command must be executed first");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
}

// Place a flag in either the current_command or current_program. The
// name is passed in explicitly to allow aliases.
void add_flag(char* name) {
  if (current_command != NULL) {
    current_command->flags = string_tree_insert(current_command->flags, name, current_flag);
  } else if (current_program != NULL) {
    current_program->write_back_file_args_ptr = write_back_file_args_ptr;
  } else {
    log_fatal("A current program or command must be executed first");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
}

void flag_boolean(char* name, boolean_t* write_back_ptr) {
  // TODO(jawilson): check for a flag with the same name?
  current_flag = malloc_struct(flag_descriptor_t);
  current_flag->flag_type = flag_type_boolean;
  current_flag->name = name;
  current_flag->write_back_ptr = write_back_ptr;
  add_flag();
}

void flags_show_usage(FILE* out) {
  /*
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
  */
  fprintf(out, "flags_show_usage() is not yet implemented!");
}

/**
 * @function flag_parse_command_line
 *
 * Parses a command line writing back the actual parsed values so they
 * can be used after command line parsing.
 *
 * When an error occurs, return a string with an error message OR NULL
 * if parsing was successfull.
 */
char* flag_parse_command_line(int argc, char** argv) {
  if (current_program == NULL) {
    log_fatal("flag_parse_command_line can't be called unless flag_program_name() is first called.");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
  
  int start = 1;
  command_descriptor_t* command = NULL;
  if (current_program->commands) {
    char* name = argv[1];
    command = flag_find_command_descriptor(name);
    if (command == NULL) {
      return string_printf("The first command line argument is not a known command: %s", name);
    }
    start = 2;
  }
  
  value_array_t* files = make_value_array(argc);
  boolean_t parse_flags = true;

  for (int i = start; i < argc; i++) {
    char* arg = argv[i];
    if (parse_flags) {
      if (string_equal(arg, "--")) {
        parse_flags = false;
        continue;
      }

      if (string_starts_with(arg, "-")) {
        flag_key_value_t key_value = flag_split_argument(arg);
        if (key_value.key == NULL) {
          return string_printf("This argument is not a well formed flag: %s", arg);
        }
        flag_descriptor_t* flag = flag_find_flag_descriptor(command, key_value.key);
        if (flag == NULL) {
          return string_printf("The argument looks like a flag but was not found: '%s'\n"
                               "You may want to use ' -- ' to seperate flags from non flag arguments.",
                               arg);
        }
        // If the value is NULL, that means there was no "=" sign
        // which means we should grab the next argument directly. When
        // the argument ends with a trailing "=", we get back an empty
        // string which is legal for purely string arguments but other
        // argument types will generaly error out during parsing.
        if (key_value.value == NULL) {
          // TODO(jawilson): bounds checking!
          i++;
          key_value.value = argv[i];
        }
        parse_and_write_value(flag, key_value);
        continue;
      }
    }
    // Either it doesn't look like a flag or else we are no longer
    // parsing flags because we saw "--".
    value_array_add(files, str_to_value(arg));
  }

  // write back left-overs...
  return NULL;
}

// Search the implicit "current_program" to see if anyone define
// "name" as a commmand.,
command_descriptor_t* flag_find_command_descriptor(char* name) {
  if (current_program->commands == NULL) {
    log_fatal("flag_get_command() shouldn't not be called when we don't have any defined commands.");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
  value_result_t command_value = string_tree_find(current_program->coommands, name);
  if (is_ok(command_value)) {
    return ((command_descriptor_t*) (command_value.ptr));
  } else {
    return NULL;
  }
}

// Search the command for a matching flag and return that first. If
// the command doesn't have a definition for flag, then the the
// "program" might have a definition for the flag so we also search
// it.
flag_descriptor_t flag_find_flag_descriptor(command_descriptor_t* command, char* name) {
  /*
    WRONG

  value_result_t command_value = string_tree_find(current_program->coommands, name);
  if (is_ok(command_value)) {
    return ((command_descriptor_t*) (command_value.ptr));
  } else {
    return NULL;
  }
  */
}

// The returned key will start with one or more "-" characters. 
//
// BTW, we are way down in the call stack and are not prepared to
// properly deal with say "---", and the caller currently actually
// benefits from seeing either "-" or "--" prepended to the key so we
// don't even bother to detect if there are more than two dashes. If
// this is illegal, the key (aka flag) is not found and better error
// reporting will happen in the caller.
//
// This actually looks like it could be a useful addition to the
// library if it can be given a descriptive generic name. Split on
// first?
//
// TODO(jawilson): Nothing says that error handler couldn't do as part
// of fuzzy matching notice this and be more helpful as a special
// case. Long command lines get broken at weird places so that might
// be useful to look out for.
flag_key_value_t flag_split_argument(char* arg) {
  int equal_sign_index = string_index_of_char(arg, '=');
  char* value = NULL;
  if (equal_sign_index >= 0) {
    char* key = string_substring(arg, 0, equal_sign_index);
    char* value = NULL;
    // We know there is an "=". If nothing comes after it, we want to
    // set value to "" instead of NULL so that we don't try to process
    // the next argument. So --foo and --foo=, will *not* be treeated
    // the same way.
    char* value = string_substring(arg, equal_sign_index + 1, strlen(arg));
    return (flag_key_value_t) { .key = key, .value = value };
  }
  return (flag_key_value_t) { .key = arg, .value = NULL };
}
