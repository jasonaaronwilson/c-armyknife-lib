//
// Test some operations on byte-arrays
//

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_append_byte() {
  byte_array_t* byte_array = make_byte_array(1);
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
  free(contents);
  free(byte_array);
}

int main(int argc, char** argv) {
  test_append_byte();
  exit(0);
}
