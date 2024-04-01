/**
 * @file terminal-input.c
 *
 * A little interactive test program for input from the terminal.
 */

#include <stdlib.h>
#include <time.h>

#define C_ARMYKNIFE_LIB_IMPL
#include "../c-armyknife-lib.h"

int main(int argc, char** argv) {
  value_array_t* FLAG_files = NULL;

  flag_program_name(argv[0]);
  flag_description(
      "A little interactive test program for testing terminal input");
  flag_file_args(&FLAG_files);

  char* error = flag_parse_command_line(argc, argv);
  if (error) {
    flag_print_help(stderr, error);
    exit(1);
  }

  if (FLAG_files->length > 0) {
    flag_print_help(stderr, error);
    exit(1);
  }

  struct termios oldt = term_echo_off();

  time_t start_time = time(NULL);
  time_t end_time = start_time + 30; // 30 seconds from start

  buffer_t* buffer = make_buffer(10);
  while (time(NULL) < end_time) {
    buffer_clear(buffer);
    buffer = buffer_read_ready_bytes(buffer, stdin, 10);
    for (uint64_t i = 0; i < buffer_length(buffer); i++) {
      uint8_t byte = buffer_get(buffer, i);
      fputc(byte, stdout);
      fflush(stdout);
    }
    usleep(1000);
  }

  // TODO(jawilson): maybe use atexit to make this a little more robust.
  term_echo_restore(oldt);

  exit(0);
}