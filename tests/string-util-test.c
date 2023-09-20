///
/// Test the additional string utilities.
///

#include <stdlib.h>
#include <stdio.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

#define FAIL(testcase_name) \
  do { \
  fprintf(stderr, "%s:%d: -- FAIL (testcase=%s)\n", __FILE__, __LINE__, testcase_name); \
  exit(1); \
  } while (0)
  

int main(int argc, char** argv) {


  if (!(string_is_null_or_empty(NULL))) {
    FAIL("string_is_null_or_empty");
  }
  if (!(string_is_null_or_empty(""))) {
    FAIL("string_is_null_or_empty");
  }
  if (string_is_null_or_empty("not empty")) {
    FAIL("string_is_null_or_empty");
  }


  if (!(string_equal("abc", "abc"))) {
    FAIL("string_equal");
  }
  if (string_equal("abc", "ABC")) {
    FAIL("string_equal");
  }


  if (!(string_starts_with("The quick brown fox", "The quick"))) {
    FAIL("string_starts_with");
  }
  if (string_starts_with("The quick brown fox", "THE QUICK")) {
    FAIL("string_starts_with");
  }


  if (!(string_ends_with("The quick brown fox", "brown fox"))) {
    FAIL("string_ends_with");
  }
  if (string_ends_with("The quick brown fox", "red dog")) {
    FAIL("string_ends_with");
  }
  

  if (!(string_contains_char("The quick brown fox", 'q'))) {
    FAIL("string_contains_char");
  }
  if (string_contains_char("The quick brown fox", 'Z')) {
    FAIL("string_contains_char");
  }

  if (string_hash("The quick brown fox") == string_hash("The QUICK brown fox")) {
    FAIL("string_hash");
  }


  // Memory leak. So what, this is a test...
  if (!(string_equal(string_substring("The quick brown fox", 4, 9), "quick"))) {
    FAIL("string_substring");
  }

  
  if (string_parse_uint64("0") != 0) {
    FAIL("string_parse_uint64");
  }
  if (string_parse_uint64("1") != 1) {
    FAIL("string_parse_uint64");
  }
  if (string_parse_uint64("0xf") != 15) {
    FAIL("string_parse_uint64");
  }
  if (string_parse_uint64("0b0") != 0) {
    FAIL("string_parse_uint64");
  }
  if (string_parse_uint64("0b1010") != 10) {
    FAIL("string_parse_uint64");
  }

  if(!string_duplicate("The quick brown fox")) {
    FAIL("string_duplicate");
  }

  exit(0);
}
