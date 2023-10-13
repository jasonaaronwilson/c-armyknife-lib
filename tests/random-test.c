//
// Test some operations on byte-arrays
//

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_random_next() {
  random_state_t state = random_state_for_test();
  for (int i = 0; i < 10; i++) {
    uint64_t next = random_next(&state);
    fprintf(stdout, "i=%d random=%lu\n", i, next);
  }
}

int main(int argc, char** argv) {
  test_random_next();
  exit(0);
}
