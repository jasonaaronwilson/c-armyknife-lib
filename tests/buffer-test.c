//
// Test some operations on byte-arrays
//

#include <stdlib.h>

#define BUFFER_PRINTF_INITIAL_BUFFER_SIZE 8

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_append_byte() {
  buffer_t* buffer = make_buffer(1);

  if (buffer_length(buffer) != strlen("")) {
    test_fail("length should be 0");
  }

  buffer = buffer_append_byte(buffer, 'a');
  buffer = buffer_append_byte(buffer, 'b');
  buffer = buffer_append_byte(buffer, 'c');
  buffer = buffer_append_byte(buffer, 'd');
  buffer = buffer_append_byte(buffer, 'e');
  buffer = buffer_append_byte(buffer, 'f');
  buffer = buffer_append_byte(buffer, 'g');
  char* contents = buffer_c_substring(buffer, 0, strlen("abcdefg"));
  if (!string_equal("abcdefg", contents)) {
    fprintf(stderr, "contents='%s'\n", contents);
    test_fail("stringified version of byte array does not match what we added");
  }
  if (buffer_length(buffer) != strlen("abcdefg")) {
    test_fail("length should be 7");
  }

  if (buffer_get(buffer, 2) != 'c') {
    test_fail("char at position 2 should be 'c'");
  }

  free_bytes(contents);
  free_bytes(buffer);
}

void test_append_string() {
  buffer_t* buffer = make_buffer(3);
  buffer = buffer_append_string(buffer, "Hello");
  buffer = buffer_append_string(buffer, " ");
  buffer = buffer_append_string(buffer, "");
  buffer = buffer_append_string(buffer, "World!");
  char* contents = buffer_to_c_string(buffer);
  if (!string_equal("Hello World!", contents)) {
    fprintf(stderr, "contents='%s'\n", contents);
    test_fail("stringified version of byte array does not match what we added");
  }
  free_bytes(contents);
  free_bytes(buffer);
}

void test_buffer_c_substring() {
  buffer_t* buffer = make_buffer(3);
  buffer = buffer_append_string(buffer, "abcdefghijklmnop");
  if (!string_equal("", buffer_c_substring(buffer, 0, 0))) {
    test_fail("buffer_c_string");
  }
  if (!string_equal("a", buffer_c_substring(buffer, 0, 1))) {
    test_fail("buffer_c_string");
  }
  if (!string_equal("ab", buffer_c_substring(buffer, 0, 2))) {
    test_fail("buffer_c_string");
  }
  if (!string_equal("abc", buffer_c_substring(buffer, 0, 3))) {
    test_fail("buffer_c_string");
  }
  if (!string_equal("abcd", buffer_c_substring(buffer, 0, 4))) {
    test_fail("buffer_c_string");
  }
  if (!string_equal("abcde", buffer_c_substring(buffer, 0, 5))) {
    test_fail("buffer_c_string");
  }
}

void test_buffer_small_printf(void) {
  buffer_t* buffer = make_buffer(1);
  buffer = buffer_printf(buffer, "%d %s", 42, "OMG");
  char* contents = buffer_to_c_string(buffer);
  if (!string_equal("42 OMG", contents)) {
    test_fail("buffer_printf contents was %s", contents);
  }
  free_bytes(contents);
  free_bytes(buffer);
}

void test_buffer_large_printf(void) {
  buffer_t* buffer = make_buffer(1);
  buffer = buffer_printf(buffer, "%d %s %d", 42, "A longer string", 24);
  char* contents = buffer_to_c_string(buffer);
  if (!string_equal("42 A longer string 24", contents)) {
    test_fail("buffer_printf contents was %s", contents);
  }
  free_bytes(contents);
  free_bytes(buffer);
}

int main(int argc, char** argv) {
  test_buffer_c_substring();
  test_append_byte();
  test_append_string();
  test_buffer_small_printf();
  test_buffer_large_printf();
  exit(0);
}
