///
/// Make an array of uint64 and add 16 elements to it.
///

#include <stdlib.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {
  command_line_parse_result_t args_and_files
      = parse_command_line(argc, argv, true);

  fprintf(stdout, "command is %s\n", args_and_files.command);

  void* value = string_ht_find(args_and_files.flags, "foo");
  fprintf(stdout, "foo is %s\n", (char*) value);

  value = string_ht_find(args_and_files.flags, "bar");
  fprintf(stdout, "bar is %s\n", (char*) value);

  for (int i = 0; i < args_and_files.files->length; i++) {
    fprintf(stdout, "bar is %s\n",
            (char*) ptr_array_get(args_and_files.files, i));
  }

  exit(0);
}
