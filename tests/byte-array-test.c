//
// Test some operations on byte-arrays
//

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_append_byte() {
  byte_array_t* byte_array = make_byte_array(1);

  if (byte_array_length(byte_array) != strlen("")) {
    ARMYKNIFE_TEST_FAIL("length should be 0");
  }

  byte_array = byte_array_append_byte(byte_array, 'a');
  byte_array = byte_array_append_byte(byte_array, 'b');
  byte_array = byte_array_append_byte(byte_array, 'c');
  byte_array = byte_array_append_byte(byte_array, 'd');
  byte_array = byte_array_append_byte(byte_array, 'e');
  byte_array = byte_array_append_byte(byte_array, 'f');
  byte_array = byte_array_append_byte(byte_array, 'g');
  char* contents = byte_array_c_substring(byte_array, 0, strlen("abcdefg"));
  if (!string_equal("abcdefg", contents)) {
    fprintf(stderr, "contents='%s'\n", contents);
    ARMYKNIFE_TEST_FAIL(
        "stringified version of byte array does not match what we added");
  }
  if (byte_array_length(byte_array) != strlen("abcdefg")) {
    ARMYKNIFE_TEST_FAIL("length should be 7");
  }

  if (byte_array_get(byte_array, 2) != 'c') {
    ARMYKNIFE_TEST_FAIL("char at position 2 should be 'c'");
  }

  free(contents);
  free(byte_array);
}

void test_append_string() {
  byte_array_t* byte_array = make_byte_array(3);
  byte_array = byte_array_append_string(byte_array, "Hello");
  byte_array = byte_array_append_string(byte_array, " ");
  byte_array = byte_array_append_string(byte_array, "");
  byte_array = byte_array_append_string(byte_array, "World!");
  char* contents = byte_array_to_c_string(byte_array);
  if (!string_equal("Hello World!", contents)) {
    fprintf(stderr, "contents='%s'\n", contents);
    ARMYKNIFE_TEST_FAIL(
        "stringified version of byte array does not match what we added");
  }
  free(contents);
  free(byte_array);
}

int main(int argc, char** argv) {
  test_append_byte();
  test_append_string();
  exit(0);
}
