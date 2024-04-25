//
// Test some operations on byte-arrays
//

#include <stdlib.h>

#define BUFFER_PRINTF_INITIAL_BUFFER_SIZE 8

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_append_byte() {
  buffer_t* buffer = make_buffer(1);

  test_assert(buffer_length(buffer) == strlen(""));

  buffer = buffer_append_byte(buffer, 'a');
  buffer = buffer_append_byte(buffer, 'b');
  buffer = buffer_append_byte(buffer, 'c');
  buffer = buffer_append_byte(buffer, 'd');
  buffer = buffer_append_byte(buffer, 'e');
  buffer = buffer_append_byte(buffer, 'f');
  buffer = buffer_append_byte(buffer, 'g');

  char* contents = buffer_c_substring(buffer, 0, strlen("abcdefg"));
  test_assert(string_equal("abcdefg", contents));
  test_assert(buffer_length(buffer) == strlen("abcdefg"));
  test_assert(buffer_get(buffer, 2) == 'c');

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
  test_assert(string_equal("Hello World!", contents));

  free_bytes(contents);
  free_bytes(buffer);
}

void test_buffer_c_substring() {
  buffer_t* buffer = make_buffer(3);
  buffer = buffer_append_string(buffer, "abcdefghijklmnop");
  test_assert(string_equal("", buffer_c_substring(buffer, 0, 0)));
  test_assert(string_equal("a", buffer_c_substring(buffer, 0, 1)));
  test_assert(string_equal("ab", buffer_c_substring(buffer, 0, 2)));
  test_assert(string_equal("abc", buffer_c_substring(buffer, 0, 3)));
  test_assert(string_equal("abcd", buffer_c_substring(buffer, 0, 4)));
  test_assert(string_equal("abcde", buffer_c_substring(buffer, 0, 5)));
}

void test_buffer_small_printf(void) {
  buffer_t* buffer = make_buffer(1);
  buffer = buffer_printf(buffer, "%d %s", 42, "OMG");
  char* contents = buffer_to_c_string(buffer);
  test_assert(string_equal("42 OMG", contents));
  free_bytes(contents);
  free_bytes(buffer);
}

void test_buffer_large_printf(void) {
  buffer_t* buffer = make_buffer(1);
  buffer = buffer_printf(buffer, "%d %s %d", 42, "A longer string", 24);
  char* contents = buffer_to_c_string(buffer);
  test_assert(string_equal("42 A longer string 24", contents));
  free_bytes(contents);
  free_bytes(buffer);
}

void test_buffer_medium_printf(void) {
  buffer_t* buffer = make_buffer(1);
  buffer = buffer_printf(buffer, "%d %s %d", 42, "--", 24);
  char* contents = buffer_to_c_string(buffer);
  test_assert(string_equal("42 -- 24", contents));
  free_bytes(contents);
  free_bytes(buffer);
}

void test_buffer_string_printf(void) {
  buffer_t* buffer = make_buffer(1);
  buffer = buffer_printf(buffer, "* [%s](%s)\n", "foobar.c", "foobar.c");
  char* contents = buffer_to_c_string(buffer);
  test_assert(string_equal("* [foobar.c](foobar.c)\n", contents));
  free_bytes(contents);
  free_bytes(buffer);
}

void test_buffer_utf8_decode(void) {
  buffer_t* buffer = make_buffer(1);
  buffer = buffer_printf(buffer, "AB");

  utf8_decode_result_t decode_result_0 = buffer_utf8_decode(buffer, 0);
  test_assert(decode_result_0.code_point == 'A');
  test_assert(decode_result_0.num_bytes == 1);
  test_assert(!decode_result_0.error);

  utf8_decode_result_t decode_result_1 = buffer_utf8_decode(buffer, 1);
  test_assert(decode_result_1.code_point == 'B');
  test_assert(decode_result_1.num_bytes == 1);
  test_assert(!decode_result_1.error);

  utf8_decode_result_t decode_result_2 = buffer_utf8_decode(buffer, 2);
  test_assert(decode_result_2.error);
}

int main(int argc, char** argv) {
  test_buffer_c_substring();
  test_append_byte();
  test_append_string();
  test_buffer_small_printf();
  test_buffer_large_printf();
  test_buffer_medium_printf();
  test_buffer_string_printf();
  test_buffer_utf8_decode();
  exit(0);
}
