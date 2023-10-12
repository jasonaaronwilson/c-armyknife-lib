///
/// Test the additional string utilities.
///

#include <stdio.h>
#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {
  ptr_array_t* tokens = NULL;

  // Lots of memory leaking below but this is only a test...

  tokens = tokenize("The quick brown fox", " ");
  if (tokens->length != 4) {
    ARMYKNIFE_TEST_FAIL("tokenize");
  }

  tokens = tokenize("The#quick#brown fox", " #");
  if (tokens->length != 4) {
    ARMYKNIFE_TEST_FAIL("tokenize");
  }

  tokens = tokenize("The#quick#brown fox", " #");
  if (tokens->length != 4
      || !string_equal("quick", (char*) ptr_array_get(tokens, 1))) {
    ARMYKNIFE_TEST_FAIL("tokenize");
  }

  exit(0);
}
