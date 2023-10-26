//
// Test some operations on uint64_t
//

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_uint64_highest_bit_set() {
  if (0 != uint64_highest_bit_set(1ULL << 0)) {
    ARMYKNIFE_TEST_FAIL("0");
  }
  if (1 != uint64_highest_bit_set(1ULL << 1)) {
    ARMYKNIFE_TEST_FAIL("1");
  }

  if (8 != uint64_highest_bit_set(1ULL << 8)) {
    ARMYKNIFE_TEST_FAIL("8");
  }

  if (16 != uint64_highest_bit_set(1ULL << 16)) {
    ARMYKNIFE_TEST_FAIL("16");
  }

  if (32 != uint64_highest_bit_set(1ULL << 32)) {
    ARMYKNIFE_TEST_FAIL("16");
  }

  if (47 != uint64_highest_bit_set(1ULL << 47)) {
    ARMYKNIFE_TEST_FAIL("48");
  }
  if (48 != uint64_highest_bit_set(1ULL << 48)) {
    ARMYKNIFE_TEST_FAIL("48");
  }

  if (63 != uint64_highest_bit_set(1ULL << 63)) {
    ARMYKNIFE_TEST_FAIL("63");
  }

  for (int i = 0; i < 64; i++) {
    if (i != uint64_highest_bit_set(((uint64_t) 1) << i)) {
      ARMYKNIFE_TEST_FAIL("loop");
    }
  }
}

int main(int argc, char** argv) {
  test_uint64_highest_bit_set();
  exit(0);
}
