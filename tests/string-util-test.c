///
/// Test the additional string utilities.
///

#include <stdio.h>
#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {

  if (!(string_is_null_or_empty(NULL))) {
    ARMYKNIFE_TEST_FAIL("string_is_null_or_empty");
  }
  if (!(string_is_null_or_empty(""))) {
    ARMYKNIFE_TEST_FAIL("string_is_null_or_empty");
  }
  if (string_is_null_or_empty("not empty")) {
    ARMYKNIFE_TEST_FAIL("string_is_null_or_empty");
  }


  if (!(string_equal("abc", "abc"))) {
    ARMYKNIFE_TEST_FAIL("string_equal");
  }
  if (string_equal("abc", "ABC")) {
    ARMYKNIFE_TEST_FAIL("string_equal");
  }


  if (!(string_starts_with("The quick brown fox", "The quick"))) {
    ARMYKNIFE_TEST_FAIL("string_starts_with");
  }
  if (string_starts_with("The quick brown fox", "THE QUICK")) {
    ARMYKNIFE_TEST_FAIL("string_starts_with");
  }


  if (!(string_ends_with("The quick brown fox", "brown fox"))) {
    ARMYKNIFE_TEST_FAIL("string_ends_with");
  }
  if (string_ends_with("The quick brown fox", "red dog")) {
    ARMYKNIFE_TEST_FAIL("string_ends_with");
  }


  if (!(string_contains_char("The quick brown fox", 'q'))) {
    ARMYKNIFE_TEST_FAIL("string_contains_char");
  }
  if (string_contains_char("The quick brown fox", 'Z')) {
    ARMYKNIFE_TEST_FAIL("string_contains_char");
  }

  if (string_hash("The quick brown fox")
      == string_hash("The QUICK brown fox")) {
    ARMYKNIFE_TEST_FAIL("string_hash");
  }


  // Memory leak. So what, this is a test...
  if (!(string_equal(string_substring("The quick brown fox", 4, 9), "quick"))) {
    ARMYKNIFE_TEST_FAIL("string_substring");
  }


  if (string_parse_uint64("0") != 0) {
    ARMYKNIFE_TEST_FAIL("string_parse_uint64");
  }
  if (string_parse_uint64("1") != 1) {
    ARMYKNIFE_TEST_FAIL("string_parse_uint64");
  }
  if (string_parse_uint64("0xf") != 15) {
    ARMYKNIFE_TEST_FAIL("string_parse_uint64");
  }
  if (string_parse_uint64("0b0") != 0) {
    ARMYKNIFE_TEST_FAIL("string_parse_uint64");
  }
  if (string_parse_uint64("0b1010") != 10) {
    ARMYKNIFE_TEST_FAIL("string_parse_uint64");
  }

  if (!string_duplicate("The quick brown fox")) {
    ARMYKNIFE_TEST_FAIL("string_duplicate");
  }

  exit(0);
}
