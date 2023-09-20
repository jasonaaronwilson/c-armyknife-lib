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

  exit(0);
}
