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
  array_t* tokens = NULL;

  // Lots of memory leaking below but this is only a test...

  tokens = tokenize("The quick brown fox", " ");
  if (tokens->length != 4) {
    FAIL("tokenize");
  }

  tokens = tokenize("The#quick#brown fox", " #");
  if (tokens->length != 4) {
    FAIL("tokenize");
  }

  tokens = tokenize("The#quick#brown fox", " #");
  if (tokens->length != 4
      || !string_equal("quick", reference_to_char_ptr(array_get_reference(tokens, 1)))) {
    FAIL("tokenize");
  }

  exit(0);
}
