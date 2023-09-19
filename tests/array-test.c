///
/// Make an array of uint64 and add 16 elements to it.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {
  array_t* array = make_array(uint64_type(), 1);
  for (int i = 0; i < 16; i++) {
    uint64_t value = i + 100;
    array = array_add(array, reference_of(uint64_type(), &value));
  }
  exit(0);
}
