//
// Manual Test for our simple terminal support.
//

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {
  buffer_t* buffer = make_buffer(100);
  buffer = buffer_printf(buffer, "Hello ");
  buffer = term_set_foreground_color(buffer, 0xff0000);
  buffer = term_set_background_color(buffer, 0xffffff);
  buffer = buffer_printf(buffer, "World!\n");
  printf("%s", buffer_to_c_string(buffer));

  // TODO(jawilson): ability to query the initial foreground and
  // background color so we can reset the terminal back to it's
  // initial state?
  exit(0);
}
