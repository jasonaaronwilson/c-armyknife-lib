## @constants command_line_flag_type_t
 
# @file command-line-parser.c

A simple command line parser for GNU long style flags and file
arguments.

Takes a command line and returns an array of "file" arguments and a
string_hashtable of command line flags.
 
## @function make_command_line_command_descriptor

Allocate a command_line_command_descriptor_t and fill in it's most
common fields.
 
## @function make_command_line_flag_descriptor

Allocate a command_line_flag_descriptor_t and fill in it's most
common fields.
 
## @function parse_command_line

Given a command line such as:

--count=10 --type=bar --no-arg file1.c file2.c

Returns a map containing 3 keys and an array containing two values
(file1.c and file2.c)

The map: "count" -> "10", "type" -> "bar", "no-arg" -> ""
The array: "file1.c" "file2.c"
 
