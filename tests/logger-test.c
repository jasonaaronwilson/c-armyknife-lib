//
// Tests the logger facility.
//

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

void test_logger() {
  log_trace("Hello World!");
  // logger_impl(__FILE__, __LINE__, LOGGER_TRACE, "Hello World #2!");
}

int main(int argc, char** argv) {
  logger_init();
  test_logger();
  exit(0);
}
