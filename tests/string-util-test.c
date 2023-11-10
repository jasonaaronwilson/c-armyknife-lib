///
/// Test the additional string utilities.
///

#include <stdio.h>
#include <stdlib.h>

// Tell the library to use a smaller initial buffer size so that it is
// easier to test with small inputs.
#define STRING_PRINTF_INITIAL_BUFFER_SIZE 8

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {

  if (!(string_is_null_or_empty(NULL))) {
    test_fail("string_is_null_or_empty");
  }
  if (!(string_is_null_or_empty(""))) {
    test_fail("string_is_null_or_empty");
  }
  if (string_is_null_or_empty("not empty")) {
    test_fail("string_is_null_or_empty");
  }


  if (!(string_equal("abc", "abc"))) {
    test_fail("string_equal");
  }
  if (string_equal("abc", "ABC")) {
    test_fail("string_equal");
  }


  if (!(string_starts_with("The quick brown fox", "The quick"))) {
    test_fail("string_starts_with");
  }
  if (string_starts_with("The quick brown fox", "THE QUICK")) {
    test_fail("string_starts_with");
  }


  if (!(string_ends_with("The quick brown fox", "brown fox"))) {
    test_fail("string_ends_with");
  }
  if (string_ends_with("The quick brown fox", "red dog")) {
    test_fail("string_ends_with");
  }

  if (!(string_contains_char("The quick brown fox", 'q'))) {
    test_fail("string_contains_char");
  }
  if (string_contains_char("The quick brown fox", 'Z')) {
    test_fail("string_contains_char");
  }

  if (string_index_of_char("The quick brown fox", 'q') != 4) {
    test_fail("string_index_of_char");
  }
  if (string_index_of_char("The quick brown fox", 'Z') >= 0) {
    test_fail("string_index_of_char");
  }

  if (string_hash("The quick brown fox")
      == string_hash("The QUICK brown fox")) {
    test_fail("string_hash");
  }

  // Memory leak. So what, this is a test...
  if (!(string_equal(string_substring("The quick brown fox", 4, 9), "quick"))) {
    test_fail("string_substring");
  }

  check_memory_hashtable_padding();

  if (string_parse_uint64("0").u64 != 0) {
    test_fail("string_parse_uint64");
  }
  if (string_parse_uint64("1").u64 != 1) {
    test_fail("string_parse_uint64");
  }
  if (string_parse_uint64("0xf").u64 != 15) {
    test_fail("string_parse_uint64");
  }
  if (string_parse_uint64("0b0").u64 != 0) {
    test_fail("string_parse_uint64");
  }
  if (string_parse_uint64("0b1010").u64 != 10) {
    test_fail("string_parse_uint64");
  }

  if (!string_duplicate("The quick brown fox")) {
    test_fail("string_duplicate");
  }

  if (!string_equal("      ", string_left_pad("", 6, ' '))) {
    test_fail("string_left_pad");
  }

  if (!string_equal("   abc", string_left_pad("abc", 6, ' '))) {
    test_fail("string_left_pad");
  }

  if (!string_equal("abcxyz", string_left_pad("abcxyz", 6, ' '))) {
    test_fail("string_left_pad");
  }

  if (!string_equal("abcdefg", string_left_pad("abcdefg", 6, ' '))) {
    test_fail("string_left_pad");
  }

  if (!string_equal("Hello!", string_printf("%s!", "Hello"))) {
    test_fail("string_printf");
  }

  if (!string_equal("Hello World!", string_printf("%s!", "Hello World"))) {
    test_fail("string_printf");
  }

  if (!string_equal("42 OMG", string_printf("%d %s", 42, "OMG"))) {
    test_fail("string_printf");
  }

  if (!string_equal("42 A longer string 24",
                    string_printf("%d %s %d", 42, "A longer string", 24))) {
    test_fail("string_printf");
  }

  exit(0);
}
