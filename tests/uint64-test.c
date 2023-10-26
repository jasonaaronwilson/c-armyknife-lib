//
// Test some operations on uint64_t
//

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_uint64_highest_bit_set() {
  if (0 != uint64_highest_bit_set(1ULL << 0)) {
    test_fail("0");
  }
  if (1 != uint64_highest_bit_set(1ULL << 1)) {
    test_fail("1");
  }

  if (8 != uint64_highest_bit_set(1ULL << 8)) {
    test_fail("8");
  }

  if (16 != uint64_highest_bit_set(1ULL << 16)) {
    test_fail("16");
  }

  if (32 != uint64_highest_bit_set(1ULL << 32)) {
    test_fail("16");
  }

  if (47 != uint64_highest_bit_set(1ULL << 47)) {
    test_fail("48");
  }
  if (48 != uint64_highest_bit_set(1ULL << 48)) {
    test_fail("48");
  }

  if (63 != uint64_highest_bit_set(1ULL << 63)) {
    test_fail("63");
  }

  for (int i = 0; i < 64; i++) {
    if (i != uint64_highest_bit_set(((uint64_t) 1) << i)) {
      test_fail("loop %d", i);
    }
  }
}

int main(int argc, char** argv) {
  test_uint64_highest_bit_set();
  exit(0);
}
