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
  if (error) {
    exit(1);
  }
  if (!FLAG_true_boolean || FLAG_false_boolean) {
    exit(1);
  }

  args[1] = "--true-boolean=1";
  args[2] = "--false-boolean=0";
  error = flag_parse_command_line(3, args);
  if (error) {
    exit(1);
  }
  if (!FLAG_true_boolean || FLAG_false_boolean) {
    exit(1);
  }

  args[1] = "--true-boolean=t";
  args[2] = "--false-boolean=f";
  error = flag_parse_command_line(3, args);
  if (error) {
    exit(1);
  }
  if (!FLAG_true_boolean || FLAG_false_boolean) {
    exit(1);
  }
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
  if (error) {
    exit(1);
  }
  if (FLAG_number != 0) {
    exit(1);
  }

  args[1] = "--number=0xf";
  error = flag_parse_command_line(2, args);
  if (error) {
    exit(1);
  }
  if (FLAG_number != 0xf) {
    exit(1);
  }
}

int main(int argc, char** argv) {
  test_boolean();
  test_uint64();
  exit(0);
}
