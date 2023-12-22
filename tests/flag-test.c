///
/// Test out command line parsing.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_boolean() {
  char* args[3];
  args[0] = "boolean-test";
  args[1] = "--true-boolean=true";
  args[2] = "--false-boolean=false";

  boolean_t FLAG_true_boolean = false;
  boolean_t FLAG_false_boolean = true;
  value_array_t* FLAG_files = NULL;

  flag_program_name("boolean-test");
  flag_boolean("--true-boolean", &FLAG_true_boolean);
  flag_boolean("--false-boolean", &FLAG_false_boolean);
  flag_file_args(&FLAG_files);

  char* error = flag_parse_command_line(3, args);
  test_assert(!error);
  test_assert(FLAG_true_boolean);
  test_assert(!FLAG_false_boolean);

  // ----------------------------------------------------------------------

  FLAG_true_boolean = false;
  FLAG_false_boolean = true;
  args[1] = "--true-boolean=1";
  args[2] = "--false-boolean=0";
  error = flag_parse_command_line(3, args);
  test_assert(!error);
  test_assert(FLAG_true_boolean);
  test_assert(!FLAG_false_boolean);

  // ----------------------------------------------------------------------

  FLAG_true_boolean = false;
  FLAG_false_boolean = true;
  args[1] = "--true-boolean=t";
  args[2] = "--false-boolean=f";
  error = flag_parse_command_line(3, args);
  test_assert(!error);
  test_assert(FLAG_true_boolean);
  test_assert(!FLAG_false_boolean);

  // ----------------------------------------------------------------------

  args[1] = "--true-boolean=BAD";
  error = flag_parse_command_line(3, args);
  test_assert(error);
}

void test_uint64() {
  char* args[2];
  args[0] = "uint64-test";
  args[1] = "--number=0";

  uint64_t FLAG_number = 0;
  value_array_t* FLAG_files = NULL;

  flag_program_name("uint64-test");
  flag_uint64("--number", &FLAG_number);
  flag_file_args(&FLAG_files);

  char* error = flag_parse_command_line(2, args);
  test_assert(!error);
  test_assert(FLAG_number == 0);

  // ----------------------------------------------------------------------

  args[1] = "--number=0xf";
  error = flag_parse_command_line(2, args);
  test_assert(!error);
  test_assert(FLAG_number == 0xf);

  // ----------------------------------------------------------------------

  args[1] = "--number=0b110";
  error = flag_parse_command_line(2, args);
  test_assert(!error);
  test_assert(FLAG_number == 6);
}

void test_string() {
  char* args[2];
  args[0] = "string-test";
  args[1] = "--string=0";

  char* FLAG_string = NULL;
  value_array_t* FLAG_files = NULL;

  flag_program_name("string-test");
  flag_string("--string", &FLAG_string);
  flag_file_args(&FLAG_files);

  char* error = flag_parse_command_line(2, args);
  test_assert(!error);
  test_assert(string_equal("0", FLAG_string));

  // ----------------------------------------------------------------------

  args[1] = "--string=hello world";
  error = flag_parse_command_line(2, args);
  test_assert(!error);
  test_assert(string_equal("hello world", FLAG_string));
}

void test_leftovers() {
  char* args[4];
  args[0] = "leftovers-test";
  args[1] = "--string=0";
  args[2] = "file1";
  args[3] = "dir/file2";

  char* FLAG_string = NULL;
  value_array_t* FLAG_files = NULL;

  flag_program_name("leftovers-test");
  flag_string("--string", &FLAG_string);
  flag_file_args(&FLAG_files);

  char* error = flag_parse_command_line(4, args);
  test_assert(!error);
  test_assert(string_equal("0", FLAG_string));
  test_assert(FLAG_files);
  test_assert(string_equal("file1", value_array_get(FLAG_files, 0).str));
  test_assert(string_equal("dir/file2", value_array_get(FLAG_files, 1).str));
}

typedef enum {
  test_enum_unknown,
  test_enum_a,
  test_enum_b,
  test_enum_c
} test_enum_t;

void test_enum_64() {
  char* args[2];
  args[0] = "enum-64-test";
  args[1] = "--enum=a";

  uint64_t FLAG_enum = test_enum_unknown;
  value_array_t* FLAG_files = NULL;

  flag_program_name("enum-64-test");
  flag_enum_64("--enum", &FLAG_enum);
  flag_enum_value("a", test_enum_a);
  flag_enum_value("A", test_enum_a);
  flag_enum_value("b", test_enum_b);
  flag_enum_value("c", test_enum_c);

  flag_file_args(&FLAG_files);

  char* error = flag_parse_command_line(2, args);
  test_assert(!error);
  test_assert(FLAG_enum == test_enum_a);

  // ----------------------------------------------------------------------

  args[1] = "--enum=b";
  error = flag_parse_command_line(2, args);
  test_assert(!error);
  test_assert(FLAG_enum == test_enum_b);

  // ----------------------------------------------------------------------

  args[1] = "--enum=BAD";
  error = flag_parse_command_line(2, args);
  test_assert(error);
}

int main(int argc, char** argv) {
  test_boolean();
  test_uint64();
  test_string();
  test_enum_64();
  test_leftovers();
  exit(0);
}
