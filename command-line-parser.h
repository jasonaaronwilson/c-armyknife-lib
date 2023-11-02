// SSCF generated file from: command-line-parser.c

#line 12 "command-line-parser.c"
#ifndef _COMMAND_LINE_PARSER_H_
#define _COMMAND_LINE_PARSER_H_

struct command_line_command_descriptor_S {
  char* name;
  char* help_string;
};

typedef struct command_line_command_descriptor_S
    command_line_command_descriptor_t;

/**
 * @constants command_line_flag_type_t
 */
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

struct command_line_parser_configuation_S {
  char* program_name;
  char* program_description;
  value_array_t* command_descriptors;
  value_array_t* flag_descriptors;
};

typedef struct command_line_parser_configuation_S
    command_line_parser_configuation_t;

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
                       command_line_parser_configuation_t* config);

#endif /* _COMMAND_LINE_PARSER_H_ */
