///
/// Test the additional string utilities.
///

#include <stdio.h>
#include <stdlib.h>

// Tell the library to use a smaller initial buffer size so that it is
// easier to test with small inputs but it turns out that 8 is a bad
// size because sizeof(array_name) doesn't work anywhere near expected
// and may be returning the sizeof a pointer which just happens to be
// 8 masking not finding bugs.

#define STRING_PRINTF_INITIAL_BUFFER_SIZE 16

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_string_printf(void) {
  test_assert(string_equal("Hello!", string_printf("%s!", "Hello")));
  test_assert(
      string_equal("Hello World!", string_printf("%s!", "Hello World")));
  test_assert(string_equal("42 OMG", string_printf("%d %s", 42, "OMG")));
  test_assert(string_equal("42 - OMG", string_printf("%d - %s", 42, "OMG")));
  test_assert(string_equal("42 -- OMG", string_printf("%d -- %s", 42, "OMG")));
  test_assert(
      string_equal("42 A longer string 24",
                   string_printf("%d %s %d", 42, "A longer string", 24)));
}

void check_legal(char* value, uint64_t expected) {
  log_warn("Checking %s", value);
  value_result_t result = string_parse_uint64(value);
  test_assert(is_ok(result) && result.u64 == expected);
}

void check_illegal(char* value) {
  log_warn("Checking %s", value);
  value_result_t result = string_parse_uint64(value);
  test_assert(is_not_ok(result));
}

void test_number_parsing() {
  check_legal("0", 0);
  check_legal("1", 1);
  check_legal("64", 64);
  check_legal("333", 333);
  check_legal("9123456789", 9123456789);
  check_legal("0x0", 0x0);
  check_legal("0xf", 0xf);
  check_legal("0x10", 0x10);
  check_legal("0xab", 0xab);
  check_legal("0xabcdef19", 0xabcdef19);
  check_legal("0b0", 0);
  check_legal("0b1010", 10);
  check_illegal("");
  check_illegal("INFO");
  check_illegal("0INFO");
  check_illegal("0b0INFO");
  check_illegal("0x0INFO");
  check_illegal("0x");
  check_illegal("0b");
}

void test_is_null_or_empty() {
  if (!(string_is_null_or_empty(NULL))) {
    test_fail("string_is_null_or_empty");
  }
  if (!(string_is_null_or_empty(""))) {
    test_fail("string_is_null_or_empty");
  }
  if (string_is_null_or_empty("not empty")) {
    test_fail("string_is_null_or_empty");
  }
}

void test_string_equal() {
  if (!(string_equal("abc", "abc"))) {
    test_fail("string_equal");
  }
  if (string_equal("abc", "ABC")) {
    test_fail("string_equal");
  }
}

void test_starts_with() {
  if (!(string_starts_with("The quick brown fox", "The quick"))) {
    test_fail("string_starts_with");
  }
  if (string_starts_with("The quick brown fox", "THE QUICK")) {
    test_fail("string_starts_with");
  }
}

void test_ends_with() {
  if (!(string_ends_with("The quick brown fox", "brown fox"))) {
    test_fail("string_ends_with");
  }
  if (string_ends_with("The quick brown fox", "red dog")) {
    test_fail("string_ends_with");
  }
}

void test_string_contains_char() {
  if (!(string_contains_char("The quick brown fox", 'q'))) {
    test_fail("string_contains_char");
  }
  if (string_contains_char("The quick brown fox", 'Z')) {
    test_fail("string_contains_char");
  }
}

void test_string_index_of_char() {
  if (string_index_of_char("The quick brown fox", 'q') != 4) {
    test_fail("string_index_of_char");
  }
  if (string_index_of_char("The quick brown fox", 'Z') >= 0) {
    test_fail("string_index_of_char");
  }
}

void test_string_left_pad() {
  test_assert_string_equal("      ", string_left_pad("", 6, ' '));
  test_assert_string_equal("   abc", string_left_pad("abc", 6, ' '));
  test_assert_string_equal("abcxyz", string_left_pad("abcxyz", 6, ' '));
  test_assert_string_equal("abcdefghijklmnop",
                           string_left_pad("abcdefghijklmnop", 6, ' '));
}

void test_string_right_pad() {
  test_assert_string_equal("      ", string_right_pad("", 6, ' '));
  test_assert_string_equal("abc   ", string_right_pad("abc", 6, ' '));
  test_assert_string_equal("abcxyz", string_right_pad("abcxyz", 6, ' '));
  test_assert_string_equal("abcdefghijklmnop",
                           string_right_pad("abcdefghijklmnop", 6, ' '));
}

int main(int argc, char** argv) {
  test_is_null_or_empty();
  test_string_equal();
  test_starts_with();
  test_ends_with();
  test_string_index_of_char();

  test_assert(string_hash("The quick brown fox")
              != string_hash("The QUICK brown fox"));

  test_number_parsing();

  // Memory leak. So what, this is a test...
  if (!(string_equal(string_substring("The quick brown fox", 4, 9), "quick"))) {
    test_fail("string_substring");
  }

  check_memory_hashtable_padding();

  test_assert(string_equal("The quick brown fox",
                           string_duplicate("The quick brown fox")));

  test_string_left_pad();
  test_string_right_pad();

  test_string_printf();

  exit(0);
}
