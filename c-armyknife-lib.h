/**
 * This is a single header file C library providing a small set of
 * critical functions on existing C data-structures as well as a few
 * critical collections like hashtables. This library only depends on
 * the C standard library (and probably some POSIX functions).
 *
 * At least one ".c" file in your project must include
 * c-armyknife-lib.h after defining C_ARMYKNIFE_LIB_IMPL which
 * provides the definitions of the code and data rather than just
 * prototypes, inlined functions, macros, and type definitions.
 */

// SSCF generated file from: fatal-error.c

#line 19 "fatal-error.c"
#ifndef _FATAL_ERROR_H_
#define _FATAL_ERROR_H_

typedef enum {
  ERROR_UKNOWN,
  ERROR_MEMORY_ALLOCATION,
  ERROR_MEMORY_FREE_NULL,
  ERROR_REFERENCE_NOT_EXPECTED_TYPE,
  ERROR_ILLEGAL_INITIAL_CAPACITY,
  ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER,
  ERROR_ACCESS_OUT_OF_BOUNDS,
  ERROR_NOT_REACHED,
  ERROR_ILLEGAL_ZERO_HASHCODE_VALUE,
  ERROR_UNIMPLEMENTED,
  ERROR_ILLEGAL_NULL_ARGUMENT,
  ERROR_BAD_COMMAND_LINE,
  ERROR_ILLEGAL_ENUM_VALUE
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);

#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */
// SSCF generated file from: boolean.c

#line 13 "boolean.c"
#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

#include <stdbool.h>

typedef bool boolean_t;

// #define true ((boolean_t) 1)
// #define false ((boolean_t) 0)

#endif /* _BOOLEAN_H_ */
// SSCF generated file from: value.c

#line 3 "value.c"
#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>

typedef union {
  uint64_t u64;
  uint64_t i64;
  char* str;
  void* ptr;
  void* dbl;
} value_t;

typedef struct {
  union {
    uint64_t u64;
    uint64_t i64;
    char* str;
    void* ptr;
    void* dbl;
    value_t val;
  };
  boolean_t found;
} value_result_t;

#define u64_to_value(x) ((value_t){.u64 = x})
#define i64_to_value(x) ((value_t){.i64 = x})
#define str_to_value(x) ((value_t){.str = x})
#define ptr_to_value(x) ((value_t){.ptr = x})
#define dbl_to_value(x) ((value_t){.dbl = x})

#endif /* _VALUE_H_ */
// SSCF generated file from: trace.c

#line 2 "trace.c"
#ifndef _TRACE_H_
#define _TRACE_H_

#include <stdio.h>

#define TRACE()                                                                \
  do {                                                                         \
    fprintf(stderr, "TRACE file=%s line=%d\n", __FILE__, __LINE__);            \
  } while (0)

#define WARN(msg)                                                              \
  do {                                                                         \
    fprintf(stderr, "WARN file=%s line=%d '%s'\n", __FILE__, __LINE__, msg);   \
  } while (0)

#endif /* _TRACE_H_ */
// SSCF generated file from: allocate.c

#line 13 "allocate.c"
#ifndef _ALLOCATE_H_
#define _ALLOCATE_H_

#include <stdint.h>

extern uint8_t* checked_malloc(char* file, int line, uint64_t amount);
extern uint8_t* checked_malloc_copy_of(char* file, int line, uint8_t* source,
                                       uint64_t amount);
extern void checked_free(char* file, int line, void* pointer);

#define malloc_bytes(amount) (checked_malloc(__FILE__, __LINE__, amount))
#define free_bytes(ptr) (checked_free(__FILE__, __LINE__, ptr))

#define malloc_struct(struct_name)                                             \
  ((struct_name*) (checked_malloc(__FILE__, __LINE__, sizeof(struct_name))))

#define malloc_copy_of(source, number_of_bytes)                                \
  (checked_malloc_copy_of(__FILE__, __LINE__, source, number_of_bytes))

#endif /* _ALLOCATE_H_ */
// SSCF generated file from: string-util.c

#line 13 "string-util.c"
#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <stdint.h>

extern int string_is_null_or_empty(const char* str1);
extern int string_equal(const char* str1, const char* str2);
extern int string_starts_with(const char* str1, const char* str2);
extern int string_ends_with(const char* str1, const char* str2);
extern boolean_t string_contains_char(const char* str, char ch);
extern int string_index_of_char(const char* a, char ch);
extern uint64_t string_to_uint64(const char* str);
extern char* uint64_to_string(uint64_t number);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern uint64_t string_parse_uint64(const char* string);
extern char* string_duplicate(const char* src);
extern char* string_append(const char* a, const char* b);

#endif /* _STRING_UTIL_H_ */
// SSCF generated file from: buffer.c

#line 10 "buffer.c"
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdint.h>
#include <string.h>

struct buffer_S {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
};

typedef struct buffer_S buffer_t;

extern buffer_t* make_buffer(uint32_t initial_capacity);

extern uint64_t buffer_length(buffer_t* buffer);

extern uint8_t buffer_get(buffer_t* buffer, uint64_t position);

extern char* buffer_c_substring(buffer_t* buffer, uint64_t start, uint64_t end);

extern char* buffer_to_c_string(buffer_t* buffer);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_increase_capacity(buffer_t* buffer, uint64_t capacity);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_byte(buffer_t* buffer, uint8_t byte);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_bytes(buffer_t* buffer, uint8_t* bytes, uint64_t n_bytes);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_string(buffer_t* buffer, const char* str);

#endif /* _BUFFER_H_ */
// SSCF generated file from: value-array.c

#line 11 "value-array.c"
#ifndef _VALUE_ARRAY_H_
#define _VALUE_ARRAY_H_

struct value_array_S {
  uint32_t length;
  uint32_t capacity;
  value_t* elements;
};

typedef struct value_array_S value_array_t;

extern value_array_t* make_value_array(uint32_t initial_capacity);
extern value_t value_array_get(value_array_t* array, uint32_t index);
extern void value_array_add(value_array_t* array, value_t element);

#endif /* _VALUE_ARRAY_H_ */
// SSCF generated file from: string-alist.c

#line 9 "string-alist.c"
#ifndef _STRING_ALIST_H_
#define _STRING_ALIST_H_

struct string_alist_S {
  struct string_alist_S* next;
  char* key;
  value_t value;
};

typedef struct string_alist_S string_alist_t;

extern value_result_t alist_find(string_alist_t* list, char* key);

__attribute__((warn_unused_result)) extern string_alist_t*
    alist_insert(string_alist_t* list, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_alist_t*
    alist_delete(string_alist_t* list, char* key);

#define string_alist_foreach(alist, key_var, value_var, statements)            \
  do {                                                                         \
    string_alist_t* head = alist;                                              \
    while (head) {                                                             \
      char* key_var = head->key;                                               \
      value_t value_var = head->value;                                         \
      statements;                                                              \
      head = head->next;                                                       \
    }                                                                          \
  } while (0)

#endif /* _STRING_ALIST_H_ */
// SSCF generated file from: string-hashtable.c

#line 8 "string-hashtable.c"
#ifndef _STRING_HASHTABLE_H_
#define _STRING_HASHTABLE_H_

struct string_hashtable_S {
  uint64_t n_buckets;
  // TODO(jawilson): keep track of the number of entries in the
  // hashtable so we can automtically grow the hashtable.
  string_alist_t* buckets[0];
};

typedef struct string_hashtable_S string_hashtable_t;

extern string_hashtable_t* make_string_hashtable(uint64_t n_buckets);

extern string_hashtable_t* string_ht_insert(string_hashtable_t* ht, char* key,
                                            value_t value);

extern string_hashtable_t* string_ht_delete(string_hashtable_t* ht, char* key);

extern value_result_t string_ht_find(string_hashtable_t* ht, char* key);

#define string_ht_foreach(ht, key_var, value_var, statements)                  \
  do {                                                                         \
    for (int ht_index = 0; ht_index < ht->n_buckets; ht_index++) {             \
      string_alist_t* alist = ht->buckets[ht_index];                           \
      if (alist != NULL) {                                                     \
        string_alist_foreach(alist, key_var, value_var, statements);           \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif /* _STRING_HASHTABLE_H_ */
// SSCF generated file from: string-tree.c

#line 17 "string-tree.c"
#ifndef _STRING_TREE_H_
#define _STRING_TREE_H_

struct string_tree_S {
  char* key;
  value_t value;
  uint32_t level;
  struct string_tree_S* left;
  struct string_tree_S* right;
};

typedef struct string_tree_S string_tree_t;

extern value_result_t string_tree_find(string_tree_t* t, char* key);

__attribute__((warn_unused_result)) extern string_tree_t*
    string_tree_insert(string_tree_t* t, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_tree_t*
    string_tree_delete(string_tree_t* t, char* key);

// TODO(jawilson): in order traversal, aka string_tree_foreach

#endif /* _STRING_TREE_H_ */
// SSCF generated file from: command-line-parser.c

#line 12 "command-line-parser.c"
#ifndef _COMMAND_LINE_PARSER_H_
#define _COMMAND_LINE_PARSER_H_

struct command_line_parse_result_S {
  char* program;
  char* command;
  string_hashtable_t* flags;
  value_array_t* files;
};

typedef struct command_line_parse_result_S command_line_parse_result_t;

extern command_line_parse_result_t parse_command_line(int argc, char** argv,
                                                      boolean_t has_command);

#endif /* _COMMAND_LINE_PARSER_H_ */
// SSCF generated file from: io.c

#line 13 "io.c"
#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name);
extern void buffer_write_file(buffer_t* bytes, char* file_name);

#endif /* _IO_H_ */
// SSCF generated file from: tokenizer.c

#line 9 "tokenizer.c"
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

extern value_array_t* tokenize(const char* str, const char* delimiters);

#endif /* _TOKENIZER_H_ */
// SSCF generated file from: random.c

#line 14 "random.c"
#ifndef _RANDOM_H_
#define _RANDOM_H_

struct random_state_S {
  uint64_t a;
  uint64_t b;
};

typedef struct random_state_S random_state_t;

extern random_state_t random_state_for_test(void);
extern uint64_t random_next(random_state_t* state);

#endif /* _RANDOM_H_ */
// SSCF generated file from: test.c

#line 13 "test.c"
#ifndef _TEST_H_
#define _TEST_H_

#define ARMYKNIFE_TEST_FAIL(msg)                                               \
  do {                                                                         \
    fprintf(stderr, "%s:%d: -- FAIL (fn=%s, msg='%s')\n", __FILE__, __LINE__,  \
            __func__, msg);                                                    \
    armyknife_test_fail_exit();                                                \
  } while (0)

#endif /* _TEST_H_ */
#ifdef C_ARMYKNIFE_LIB_IMPL
#line 2 "allocate.c"
/**
 * @file allocate.c
 *
 * This file contains wrappers around malloc to make it more
 * convenient and possibly safer.
 */

// ======================================================================
// This is block is extraced to allocate.h
// ======================================================================

#ifndef _ALLOCATE_H_
#define _ALLOCATE_H_

#include <stdint.h>

extern uint8_t* checked_malloc(char* file, int line, uint64_t amount);
extern uint8_t* checked_malloc_copy_of(char* file, int line, uint8_t* source,
                                       uint64_t amount);
extern void checked_free(char* file, int line, void* pointer);

#define malloc_bytes(amount) (checked_malloc(__FILE__, __LINE__, amount))
#define free_bytes(ptr) (checked_free(__FILE__, __LINE__, ptr))

#define malloc_struct(struct_name)                                             \
  ((struct_name*) (checked_malloc(__FILE__, __LINE__, sizeof(struct_name))))

#define malloc_copy_of(source, number_of_bytes)                                \
  (checked_malloc_copy_of(__FILE__, __LINE__, source, number_of_bytes))

#endif /* _ALLOCATE_H_ */

// ======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

boolean_t is_initialized = false;
boolean_t should_log_value = false;

uint64_t number_of_bytes_allocated = 0;
uint64_t number_of_malloc_calls = 0;
uint64_t number_of_free_calls = 0;

static inline boolean_t should_log() {
  if (is_initialized) {
    return should_log_value;
  }
  char* var = getenv("ARMYKNIFE_LOG_MEMORY_ALLOCATION");
  is_initialized = true;
  if (var != NULL && strcmp(var, "true") == 0) {
    should_log_value = true;
  }
  return should_log_value;
}

/**
 * Allocate amount bytes or cause a fatal error. The memory is also
 * zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
uint8_t* checked_malloc(char* file, int line, uint64_t amount) {
  if (should_log()) {
    fprintf(stderr, "ALLOCATE %s:%d -- %lu\n", file, line, amount);
  }
  uint8_t* result = malloc(amount);
  if (result == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_ALLOCATION);
  }
  number_of_bytes_allocated += amount;
  number_of_malloc_calls++;
  memset(result, 0, amount);
  return result;
}

/**
 * Allocate amount bytes and initialize it with a copy of that many
 * bytes from source.
 */
uint8_t* checked_malloc_copy_of(char* file, int line, uint8_t* source,
                                uint64_t amount) {
  uint8_t* result = checked_malloc(file, line, amount);
  memcpy(result, source, amount);
  return result;
}

/**
 * Allocate amount bytes or cause a fatal error. The memory is also
 * zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
void checked_free(char* file, int line, void* pointer) {
  if (should_log()) {
    fprintf(stderr, "DEALLOCATE %s:%d -- %lu\n", file, line,
            (uint64_t) pointer);
  }
  if (pointer == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_FREE_NULL);
  }
  number_of_free_calls++;
  free(pointer);
}
#line 2 "boolean.c"
/**
 * @file boolean.c
 *
 * Provides a simple typdef and true/false which sometimes makes code
 * more readable.
 */

// ======================================================================
// This section is extraced to boolean.h
// ======================================================================

#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

#include <stdbool.h>

typedef bool boolean_t;

// #define true ((boolean_t) 1)
// #define false ((boolean_t) 0)

#endif /* _BOOLEAN_H_ */
#line 2 "buffer.c"
/**
 * @file buffer.c
 */

// ======================================================================
// This is block is extraced to byte-array.h
// ======================================================================

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdint.h>
#include <string.h>

struct buffer_S {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
};

typedef struct buffer_S buffer_t;

extern buffer_t* make_buffer(uint32_t initial_capacity);

extern uint64_t buffer_length(buffer_t* buffer);

extern uint8_t buffer_get(buffer_t* buffer, uint64_t position);

extern char* buffer_c_substring(buffer_t* buffer, uint64_t start, uint64_t end);

extern char* buffer_to_c_string(buffer_t* buffer);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_increase_capacity(buffer_t* buffer, uint64_t capacity);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_byte(buffer_t* buffer, uint8_t byte);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_bytes(buffer_t* buffer, uint8_t* bytes, uint64_t n_bytes);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_string(buffer_t* buffer, const char* str);

#endif /* _BUFFER_H_ */

// ======================================================================

#include <stdlib.h>

/**
 * Make an empty byte array with the given initial capacity.
 */
buffer_t* make_buffer(uint32_t initial_capacity) {

  if (initial_capacity < 1) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }

  buffer_t* result
      = (buffer_t*) (malloc_bytes(initial_capacity + sizeof(buffer_t)));
  result->capacity = initial_capacity;
  return result;
}

/**
 * Return the number of bytes that have been added to this byte array.
 */
uint64_t buffer_length(buffer_t* array) { return array->length; }

/**
 * Get a single byte from a byte array.
 */
uint8_t buffer_get(buffer_t* buffer, uint64_t position) {
  if (position < buffer->length) {
    return buffer->elements[position];
  } else {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Extract a newly allocated string that contain the bytes from start
 * to end (appending a zero byte to make sure it's a legal C string).
 */
char* buffer_c_substring(buffer_t* buffer, uint64_t start, uint64_t end) {
  // Add one extra byte for a NUL string terminator byte
  char* result = (char*) (malloc_bytes((end - start) + 1));
  for (int i = start; i < end; i++) {
    result[i - start] = buffer->elements[i];
  }
  // This should not be necessary. malloc_bytes is supposed to zero
  // initialize bytes. yet this seems to have fixed a bug!
  result[end - start] = '\0';
  return result;
}

/**
 * Extract a newly allocated string that contain all of the bytes in the byte
 * buffer as a NU * terminated C string.
 */
char* buffer_to_c_string(buffer_t* buffer) {
  return buffer_c_substring(buffer, 0, buffer->length);
}

/**
 * Append a single byte to the byte array.
 */
__attribute__((warn_unused_result)) buffer_t*
    buffer_append_byte(buffer_t* buffer, uint8_t element) {
  if (buffer->length < buffer->capacity) {
    buffer->elements[buffer->length] = element;
    buffer->length++;
    return buffer;
  }
  buffer = buffer_increase_capacity(buffer, buffer->capacity * 2);
  return buffer_append_byte(buffer, element);
}

/**
 * Append multiple bytes to the byte array.
 */
__attribute__((warn_unused_result)) buffer_t*
    buffer_append_bytes(buffer_t* buffer, uint8_t* bytes, uint64_t n_bytes) {
  // Obviously this can be optimized...
  for (int i = 0; i < n_bytes; i++) {
    buffer = buffer_append_byte(buffer, bytes[i]);
  }
  return buffer;
}

/**
 * Append all of the bytes from a C string (except the ending NUL
 * char).
 */
__attribute__((warn_unused_result)) buffer_t*
    buffer_append_string(buffer_t* buffer, const char* str) {
  return buffer_append_bytes(buffer, (uint8_t*) str, strlen(str));
}

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_increase_capacity(buffer_t* buffer, uint64_t capacity) {
  if (buffer->capacity < capacity) {
    buffer_t* result = make_buffer(capacity);
    for (int i = 0; i < buffer->length; i++) {
      result = buffer_append_byte(result, buffer_get(buffer, i));
    }
    free_bytes(buffer);
    return result;
  }
  return buffer;
}
#line 2 "command-line-parser.c"
/**
 * @file command-line-parser.c
 *
 * A simple command line parser for GNU long style flags and file
 * arguments.
 *
 * Takes a command line and returns an array of "file" arguments and a
 * string_hashtable of command line flags.
 */

#ifndef _COMMAND_LINE_PARSER_H_
#define _COMMAND_LINE_PARSER_H_

struct command_line_parse_result_S {
  char* program;
  char* command;
  string_hashtable_t* flags;
  value_array_t* files;
};

typedef struct command_line_parse_result_S command_line_parse_result_t;

extern command_line_parse_result_t parse_command_line(int argc, char** argv,
                                                      boolean_t has_command);

#endif /* _COMMAND_LINE_PARSER_H_ */

/**
 * Given a command line such as:
 *
 * --count=10 --type=bar --no-arg file1.c file2.c
 *
 * Returns a map containing 3 keys and an array containing to values.
 *
 * The map: "count" -> "10", "type" -> "bar", "no-arg" -> ""
 * The array: "file1.c" "file2.c"
 */
command_line_parse_result_t parse_command_line(int argc, char** argv,
                                               boolean_t has_command) {
  value_array_t* files = make_value_array(argc);
  string_hashtable_t* flags = make_string_hashtable(32);

  boolean_t parse_flags = true;
  for (int i = (has_command ? 2 : 1); i < argc; i++) {
    char* arg = argv[i];

    if (parse_flags) {
      if (string_starts_with(arg, "--")) {
        if (string_equal(arg, "--")) {
          parse_flags = true;
          continue;
        }
        int equal_sign_index = string_index_of_char(arg, '=');

        char* key = "";
        char* value = "";
        if (equal_sign_index >= 0) {
          key = string_substring(arg, 2, equal_sign_index);
          value = string_substring(arg, equal_sign_index + 1, strlen(arg));
        } else {
          key = string_substring(arg, 2, strlen(arg));
        }

        flags = string_ht_insert(flags, key, str_to_value(value));

        continue;
      }
    }

    value_array_add(files, str_to_value(arg));
  }

  char* command = has_command && argc >= 2 ? argv[1] : NULL;
  if (command && string_starts_with(command, "-")) {
    fprintf(stdout, "Flags should not appear before a command.");
    fatal_error(ERROR_BAD_COMMAND_LINE);
  }

  return (command_line_parse_result_t){
      .program = argv[0],
      .command = command,
      .flags = flags,
      .files = files,
  };
}
#line 2 "fatal-error.c"
/**
 * @file: fatal-error.c
 *
 * The intent is that everything but a normal program exit will end up
 * here. (Currently, we don't catch any signals so this is definitely
 * not true.)
 *
 * In this case C's macros are paying off as the file and line number
 * are easy to obtain.
 *
 * TODO(jawilson): command line flag to be quieter...
 */

// ======================================================================
// This is block is extraced to fatal-error.h
// ======================================================================

#ifndef _FATAL_ERROR_H_
#define _FATAL_ERROR_H_

typedef enum {
  ERROR_UKNOWN,
  ERROR_MEMORY_ALLOCATION,
  ERROR_MEMORY_FREE_NULL,
  ERROR_REFERENCE_NOT_EXPECTED_TYPE,
  ERROR_ILLEGAL_INITIAL_CAPACITY,
  ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER,
  ERROR_ACCESS_OUT_OF_BOUNDS,
  ERROR_NOT_REACHED,
  ERROR_ILLEGAL_ZERO_HASHCODE_VALUE,
  ERROR_UNIMPLEMENTED,
  ERROR_ILLEGAL_NULL_ARGUMENT,
  ERROR_BAD_COMMAND_LINE,
  ERROR_ILLEGAL_ENUM_VALUE
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);

#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */

// ======================================================================

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

void print_fatal_error_banner();
void print_backtrace();
void print_error_code_name(int error_code);

void _Noreturn fatal_error_impl(char* file, int line, int error_code) {
  print_fatal_error_banner();
  print_backtrace();
  fprintf(stderr, "%s:%d: FATAL ERROR %d", file, line, error_code);
  print_error_code_name(error_code);
  fprintf(stderr, "Necessaria Morte Mori...\n");
  exit(-(error_code + 100));
}

void print_fatal_error_banner() {
  // As the first thing we print, also responsible for at least one
  // newline to start a new line if we may not be at one.
  fprintf(stderr, "\n========== FATAL_ERROR ==========\n");
}

void print_backtrace() {
#ifndef NO_VM_BACKTRACE_ON_FATAL_ERROR
  do {
    void* array[10];
    int size = backtrace(array, 10);
    char** strings = backtrace_symbols(array, size);

    // Print the backtrace
    for (int i = 0; i < size; i++) {
      printf("#%d %s\n", i, strings[i]);
    }
  } while (0);
#endif /* NO_VM_BACKTRACE_ON_FATAL_ERROR */
}

const char* fatal_error_code_to_string(int error_code) {
  switch (error_code) {
  case ERROR_UKNOWN:
    return "ERROR_UKNOWN";
  case ERROR_MEMORY_ALLOCATION:
    return "ERROR_MEMORY_ALLOCATION";
  case ERROR_MEMORY_FREE_NULL:
    return "ERROR_MEMORY_FREE_NULL";
  case ERROR_REFERENCE_NOT_EXPECTED_TYPE:
    return "ERROR_REFERENCE_NOT_EXPECTED_TYPE";
  case ERROR_ILLEGAL_INITIAL_CAPACITY:
    return "ERROR_ILLEGAL_INITIAL_CAPACITY";
  case ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER:
    return "ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER";
  case ERROR_ACCESS_OUT_OF_BOUNDS:
    return "ERROR_ACCESS_OUT_OF_BOUNDS";
  case ERROR_NOT_REACHED:
    return "ERROR_NOT_REACHED";
  case ERROR_ILLEGAL_ZERO_HASHCODE_VALUE:
    return "ERROR_ILLEGAL_ZERO_HASHCODE_VALUE";
  case ERROR_UNIMPLEMENTED:
    return "ERROR_UNIMPLEMENTED";
  case ERROR_ILLEGAL_NULL_ARGUMENT:
    return "ERROR_ILLEGAL_NULL_ARGUMENT";
  default:
    return "error";
  }
}

void print_error_code_name(int error_code) {
  fprintf(stderr, " ");
  fprintf(stderr, "*** ");
  fprintf(stderr, "%s", fatal_error_code_to_string(error_code));
  fprintf(stderr, " ***\n");
}
#line 2 "io.c"
/**
 * @file io.c
 *
 * This contains routines to read the contents of a file or write a
 * new file.
 */

// ======================================================================
// This is block is extraced to allocate.h
// ======================================================================

#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name);
extern void buffer_write_file(buffer_t* bytes, char* file_name);

#endif /* _IO_H_ */

// ======================================================================

#include <stdio.h>

// This is optional...
#include <sys/stat.h>

__attribute__((warn_unused_result)) buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name) {

  uint64_t capacity = bytes->capacity;

  // This is optional
  {
    struct stat st;
    stat(file_name, &st);
    capacity = st.st_size;
  }

  bytes = buffer_increase_capacity(bytes, capacity);

  FILE* file = fopen(file_name, "r");
  uint8_t buffer[1024];

  while (1) {
    uint64_t n_read = fread(buffer, 1, sizeof(buffer), file);
    if (n_read == 0) {
      break;
    }
    bytes = buffer_append_bytes(bytes, buffer, n_read);
  }

  fclose(file);

  return bytes;
}

void buffer_write_file(buffer_t* bytes, char* file_name) {
  FILE* file = fopen(file_name, "w");
  fwrite(&bytes->elements, 1, bytes->length, file);
  fclose(file);
}
#line 2 "random.c"

/**
 * An implementation of "xorshiro128**", a pseudo-random number
 * generator.
 *
 * This is not a high quality source of entropy and is intended for
 * use in tests or other places where determinism is important
 * (including across platforms and C library implementations).
 *
 * See: https://prng.di.unimi.it/xoroshiro128starstar.c
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

struct random_state_S {
  uint64_t a;
  uint64_t b;
};

typedef struct random_state_S random_state_t;

extern random_state_t random_state_for_test(void);
extern uint64_t random_next(random_state_t* state);

#endif /* _RANDOM_H_ */

/**
 * Return a consistent initial random state for tests.
 */
random_state_t random_state_for_test(void) {
  return (random_state_t){.a = 0x1E1D43C2CA44B1F5, .b = 0x4FDD267452CEDBAC};
}

static inline uint64_t rotl(uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

uint64_t random_next(random_state_t* state) {
  uint64_t s0 = state->a;
  uint64_t s1 = state->b;
  uint64_t result = rotl(s0 * 5, 7) * 9;
  s1 ^= s0;
  state->a = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
  state->b = rotl(s1, 37);                   // c

  return result;
}
#line 2 "string-alist.c"
/**
 * @file string-alist.c
 *
 * A simple map between a string with a pointer to some other
 * data-structure.
 */

#ifndef _STRING_ALIST_H_
#define _STRING_ALIST_H_

struct string_alist_S {
  struct string_alist_S* next;
  char* key;
  value_t value;
};

typedef struct string_alist_S string_alist_t;

extern value_result_t alist_find(string_alist_t* list, char* key);

__attribute__((warn_unused_result)) extern string_alist_t*
    alist_insert(string_alist_t* list, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_alist_t*
    alist_delete(string_alist_t* list, char* key);

#define string_alist_foreach(alist, key_var, value_var, statements)            \
  do {                                                                         \
    string_alist_t* head = alist;                                              \
    while (head) {                                                             \
      char* key_var = head->key;                                               \
      value_t value_var = head->value;                                         \
      statements;                                                              \
      head = head->next;                                                       \
    }                                                                          \
  } while (0)

#endif /* _STRING_ALIST_H_ */

string_alist_t* alist_insert(string_alist_t* list, char* key, value_t value) {
  string_alist_t* result = (malloc_struct(string_alist_t));
  result->next = alist_delete(list, key);
  result->key = key;
  result->value = value;
  return result;
}

string_alist_t* alist_delete(string_alist_t* list, char* key) {
  // This appears to be logically correct but could easily blow out
  // the stack with a long list.
  if (list == NULL) {
    return list;
  }
  if (strcmp(key, list->key) == 0) {
    string_alist_t* result = list->next;
    free(list);
    return result;
  }
  list->next = alist_delete(list->next, key);
  return list;
}

value_result_t alist_find(string_alist_t* list, char* key) {
  while (list) {
    if (strcmp(key, list->key) == 0) {
      return (value_result_t){.val = list->value, .found = true};
    }
    list = list->next;
  }
  return (value_result_t){};
}
#line 2 "string-hashtable.c"
/**
 * @file string-hashtable.c
 *
 * A hash map of string to a value_t.
 */

#ifndef _STRING_HASHTABLE_H_
#define _STRING_HASHTABLE_H_

struct string_hashtable_S {
  uint64_t n_buckets;
  // TODO(jawilson): keep track of the number of entries in the
  // hashtable so we can automtically grow the hashtable.
  string_alist_t* buckets[0];
};

typedef struct string_hashtable_S string_hashtable_t;

extern string_hashtable_t* make_string_hashtable(uint64_t n_buckets);

extern string_hashtable_t* string_ht_insert(string_hashtable_t* ht, char* key,
                                            value_t value);

extern string_hashtable_t* string_ht_delete(string_hashtable_t* ht, char* key);

extern value_result_t string_ht_find(string_hashtable_t* ht, char* key);

#define string_ht_foreach(ht, key_var, value_var, statements)                  \
  do {                                                                         \
    for (int ht_index = 0; ht_index < ht->n_buckets; ht_index++) {             \
      string_alist_t* alist = ht->buckets[ht_index];                           \
      if (alist != NULL) {                                                     \
        string_alist_foreach(alist, key_var, value_var, statements);           \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif /* _STRING_HASHTABLE_H_ */

/**
 * Create a hashtable with the given initial capacity.
 */
string_hashtable_t* make_string_hashtable(uint64_t n_buckets) {
  if (n_buckets == 0) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }
  string_hashtable_t* result = (string_hashtable_t*) (malloc_bytes(
      sizeof(string_hashtable_t) + sizeof(string_alist_t*) * n_buckets));
  result->n_buckets = n_buckets;
  return result;
}

/**
 * Insert an association into the hashtable.
 */
string_hashtable_t* string_ht_insert(string_hashtable_t* ht, char* key,
                                     value_t value) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  ht->buckets[bucket] = alist_insert(list, key, value);
  return ht;
}

/**
 * Delete an association from the hashtable.
 */
string_hashtable_t* string_ht_delete(string_hashtable_t* ht, char* key) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  ht->buckets[bucket] = alist_delete(list, key);
  return ht;
}

/**
 * Find an association in the hashtable.
 */
value_result_t string_ht_find(string_hashtable_t* ht, char* key) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  return alist_find(list, key);
}
#line 2 "string-tree.c"

/**
 * This is a balanced binary tree to associate a string and a value.
 *
 * Generally a string_alist is prefered for small "maps", and
 * string_hashtable is prefered for large maps, but string_tree is the
 * easiest way to get sorted results (which is most important for
 * reproducibility).
 *
 * Currently we are using "AA" trees (see
 * https://en.wikipedia.org/wiki/AA_tree) since it has simpler code
 * than many other balanced trees like RB trees and the Wikipedia
 * article and paper spell out most of the non-trivial details.
 */

#ifndef _STRING_TREE_H_
#define _STRING_TREE_H_

struct string_tree_S {
  char* key;
  value_t value;
  uint32_t level;
  struct string_tree_S* left;
  struct string_tree_S* right;
};

typedef struct string_tree_S string_tree_t;

extern value_result_t string_tree_find(string_tree_t* t, char* key);

__attribute__((warn_unused_result)) extern string_tree_t*
    string_tree_insert(string_tree_t* t, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_tree_t*
    string_tree_delete(string_tree_t* t, char* key);

// TODO(jawilson): in order traversal, aka string_tree_foreach

#endif /* _STRING_TREE_H_ */

/**
 * Find the value associate with the key in the tree.
 */
value_result_t string_tree_find(string_tree_t* t, char* key) {
  if (t == NULL) {
    return (value_result_t){.found = 0};
  }
  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    return string_tree_find(t->left, key);
  } else if (cmp_result > 0) {
    return string_tree_find(t->right, key);
  } else {
    return (value_result_t){.val = t->value, .found = true};
  }
}

string_tree_t* string_tree_skew(string_tree_t* t) {
  if (t == NULL) {
    return NULL;
  }
  if (t->left == NULL) {
    return t;
  }
  if (t->left->level == t->level) {
    string_tree_t* L = t->left;
    t->left = L->right;
    L->right = t;
    return L;
  }
  return t;
}

string_tree_t* string_tree_split(string_tree_t* t) {
  if (t == NULL) {
    return NULL;
  }
  if (t->right == NULL || t->right->right == NULL) {
    return t;
  }
  if (t->level == t->right->right->level) {
    // We have two horizontal right links.  Take the middle node,
    // elevate it, and return it.
    string_tree_t* R = t->right;
    t->right = R->left;
    R->left = t;
    R->level = R->level + 1;
    return R;
  }
  return t;
}

string_tree_t* make_string_tree_leaf(char* key, value_t value) {
  string_tree_t* result = malloc_struct(string_tree_t);
  result->level = 1;
  result->key = key;
  result->value = value;
  return result;
}

/**
 * Insert an association of key and a value.
 */
string_tree_t* string_tree_insert(string_tree_t* t, char* key, value_t value) {
  if (t == NULL) {
    // Create a new leaf node
    return make_string_tree_leaf(key, value);
  }
  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    t->left = string_tree_insert(t->left, key, value);
  } else if (cmp_result > 0) {
    t->right = string_tree_insert(t->right, key, value);
  } else {
    // might need to free either key or t->key...
    t->value = value;
    return t;
  }
  t = string_tree_skew(t);
  t = string_tree_split(t);
  return t;
}

static inline boolean_t min_level(uint32_t a, uint32_t b) {
  return a < b ? a : b;
}

string_tree_t* string_tree_decrease_level(string_tree_t* t) {
  if (t->left && t->right) {
    uint32_t should_be = min_level(t->left->level, t->right->level) + 1;
    if (should_be < t->level) {
      t->level = should_be;
      if (should_be < t->right->level) {
        t->right->level = should_be;
      }
    }
  }
  return t;
}

string_tree_t* string_tree_predecessor(string_tree_t* t) {
  t = t->left;
  while (t->right != NULL) {
    t = t->right;
  }
  return t;
}

string_tree_t* string_tree_successor(string_tree_t* t) {
  t = t->right;
  while (t->left != NULL) {
    t = t->left;
  }
  return t;
}

static inline boolean_t string_tree_is_leaf(string_tree_t* t) {
  return t->left == NULL && t->right == NULL;
}

/**
 * Delete the association of key (if it exists in the tree).
 */
string_tree_t* string_tree_delete(string_tree_t* t, char* key) {

  if (t == NULL) {
    return t;
  }

  int cmp_result = strcmp(key, t->key);
  if (cmp_result > 0) {
    t->right = string_tree_delete(t->right, key);
  } else if (cmp_result < 0) {
    t->left = string_tree_delete(t->left, key);
  } else {
    // If we're a leaf, easy, otherwise reduce to leaf case.
    if (string_tree_is_leaf(t)) {
      return t->right;
    } else if (t->left == NULL) {
      string_tree_t* L = string_tree_successor(t);
      t->right = string_tree_delete(t->right, L->key);
      t->key = L->key;
      t->value = L->value;
    } else {
      string_tree_t* L = string_tree_predecessor(t);
      t->left = string_tree_delete(t->left, L->key);
      t->key = L->key;
      t->value = L->value;
    }
  }

  // Rebalance the tree. Decrease the level of all nodes in this level
  // if necessary, and then skew and split all nodes in the new level.

  t = string_tree_decrease_level(t);
  t = string_tree_skew(t);
  t->right = string_tree_skew(t->right);
  if (t->right != NULL) {
    t->right->right = string_tree_skew(t->right->right);
  }
  t = string_tree_split(t);
  t->right = string_tree_split(t->right);
  return t;
}
#line 2 "string-util.c"
/**
 * @file string-util.c
 *
 * This contains additional string function to operate on "strings"
 * since the C libary has only basic routines.
 */

// ======================================================================
// This is block is extraced to allocate.h
// ======================================================================

#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <stdint.h>

extern int string_is_null_or_empty(const char* str1);
extern int string_equal(const char* str1, const char* str2);
extern int string_starts_with(const char* str1, const char* str2);
extern int string_ends_with(const char* str1, const char* str2);
extern boolean_t string_contains_char(const char* str, char ch);
extern int string_index_of_char(const char* a, char ch);
extern uint64_t string_to_uint64(const char* str);
extern char* uint64_to_string(uint64_t number);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern uint64_t string_parse_uint64(const char* string);
extern char* string_duplicate(const char* src);
extern char* string_append(const char* a, const char* b);

#endif /* _STRING_UTIL_H_ */

// ======================================================================

#include <stdlib.h>
#include <string.h>

uint64_t fasthash64(const void* buf, size_t len, uint64_t seed);

int string_is_null_or_empty(const char* str) {
  return (str == NULL) || (strlen(str) == 0);
}

int string_equal(const char* str1, const char* str2) {
  if (string_is_null_or_empty(str1)) {
    return string_is_null_or_empty(str2);
  }
  return strcmp(str1, str2) == 0;
}

int string_starts_with(const char* str1, const char* str2) {
  return strncmp(str1, str2, strlen(str2)) == 0;
}

int string_ends_with(const char* str1, const char* str2) {
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);

  if (len2 > len1) {
    return 0;
  }

  return strcmp(str1 + (len1 - len2), str2) == 0;
}

boolean_t string_contains_char(const char* str, char ch) {
  return string_index_of_char(str, ch) >= 0;
}

int string_index_of_char(const char* str, char ch) {
  if (string_is_null_or_empty(str)) {
    return -1;
  }
  int str_length = strlen(str);
  for (int i = 0; i < str_length; i++) {
    if (str[i] == ch) {
      return i;
    }
  }
  return -1;
}


uint64_t string_hash(const char* str) {
  return fasthash64(str, strlen(str), 0);
}

char* string_substring(const char* str, int start, int end) {
  // TODO(jawilson): check length of str...
  int result_size = end - start + 1;
  char* result = (char*) (malloc_bytes(result_size));
  for (int i = start; (i < end); i++) {
    result[i - start] = str[i];
  }
  result[result_size] = '\0';
  return result;
}

uint64_t string_parse_uint64_dec(const char* string) {
  uint64_t integer = 0;
  uint64_t digit;

  while (*string != '\0') {
    digit = *string - '0';
    integer = integer * 10 + digit;
    string++;
  }

  return integer;
}

uint64_t string_parse_uint64_hex(const char* string) {
  uint64_t integer = 0;
  uint64_t digit;

  int i = 0;
  if (string[i] == '0' && (string[i + 1] == 'x' || string[i + 1] == 'X')) {
    i += 2;
  }

  while (string[i] != '\0') {
    digit = string[i] - '0';
    if (digit > 9) {
      digit = string[i] - 'a' + 10;
    }
    integer = integer * 16 + digit;
    i++;
  }

  return integer;
}

uint64_t string_parse_uint64_bin(const char* string) {
  uint64_t integer = 0;
  uint64_t digit;

  int i = 0;
  if (string[i] == '0' && (string[i + 1] == 'b' || string[i + 1] == 'B')) {
    i += 2;
  }

  while (string[i] != '\0') {
    digit = string[i] - '0';
    integer = integer * 2 + digit;
    i++;
  }

  return integer;
}

uint64_t string_parse_uint64(const char* string) {
  if (string_starts_with(string, "0x")) {
    return string_parse_uint64_hex(string);
  } else if (string_starts_with(string, "0b")) {
    return string_parse_uint64_bin(string);
  } else {
    return string_parse_uint64_dec(string);
  }
}

/**
 * Just like strdup except NULL is a valid source argument and we use
 * malloc_bytes which checks the return result from malloc.
 */
char* string_duplicate(const char* src) {
  if (src == NULL) {
    return NULL;
  }
  int len = strlen(src) + 1;
  char* result = (char*) malloc_bytes(len);
  memcpy(result, src, len);

  return result;
}

/**
 * Return a freshly allocated string that is the concatentation of the
 * two input strings (neither of which should be NULL);
 */
char* string_append(const char* a, const char* b) {
  if (a == NULL || b == NULL) {
    fatal_error(ERROR_ILLEGAL_NULL_ARGUMENT);
  }
  int total_length = strlen(a) + strlen(b) + 1;
  char* result = (char*) (malloc_bytes(total_length));
  strcat(result, a);
  strcat(result, b);
  return result;
}

/**
 * Convert a uint64_t number to a string.
 */
char* uint64_to_string(uint64_t number) {
  char buffer[32];
  sprintf(buffer, "%lu", number);
  return string_duplicate(buffer);
}

/* ================================================================ */

/* The MIT License

   Copyright (C) 2012 Zilong Tan (eric.zltan@gmail.com)

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

// #include "fasthash.h"

// Compression function for Merkle-Damgard construction.
// This function is generated using the framework provided.
static inline uint64_t mix(uint64_t h) {
  h ^= h >> 23;
  h *= 0x2127599bf4325c37ULL;
  h ^= h >> 47;
  return h;
}

// security: if the system allows empty keys (len=3) the seed is exposed, the
// reverse of mix. objsize: 0-1fd: 509
uint64_t fasthash64(const void* buf, size_t len, uint64_t seed) {
  const uint64_t m = 0x880355f21e6d1965ULL;
  const uint64_t* pos = (const uint64_t*) buf;
  const uint64_t* end = pos + (len / 8);
  const unsigned char* pos2;
  uint64_t h = seed ^ (len * m);
  uint64_t v;

  while (pos != end) {
    v = *pos++;
    h ^= mix(v);
    h *= m;
  }

  pos2 = (const unsigned char*) pos;
  v = 0;

  switch (len & 7) {
  case 7:
    v ^= (uint64_t) pos2[6] << 48;
  case 6:
    v ^= (uint64_t) pos2[5] << 40;
  case 5:
    v ^= (uint64_t) pos2[4] << 32;
  case 4:
    v ^= (uint64_t) pos2[3] << 24;
  case 3:
    v ^= (uint64_t) pos2[2] << 16;
  case 2:
    v ^= (uint64_t) pos2[1] << 8;
  case 1:
    v ^= (uint64_t) pos2[0];
    h ^= mix(v);
    h *= m;
  }

  return mix(h);
}
#line 2 "test.c"
/**
 * @file test.c
 *
 * A set of macros and inline functions to help write tests for the
 * armyknife library (or your own tests).
 */

// ======================================================================
// This section is extraced to test.h
// ======================================================================

#ifndef _TEST_H_
#define _TEST_H_

#define ARMYKNIFE_TEST_FAIL(msg)                                               \
  do {                                                                         \
    fprintf(stderr, "%s:%d: -- FAIL (fn=%s, msg='%s')\n", __FILE__, __LINE__,  \
            __func__, msg);                                                    \
    armyknife_test_fail_exit();                                                \
  } while (0)

#endif /* _TEST_H_ */

// Provide a convenient place to set a breakpoint
void armyknife_test_fail_exit() { exit(1); }
#line 2 "tokenizer.c"
/**
 * @file tokenizer.c
 *
 * This contains a routine to split a string into a series of tokens
 * seperated via delimiters.
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

extern value_array_t* tokenize(const char* str, const char* delimiters);

#endif /* _TOKENIZER_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_duplicate(value_array_t* token_array, const char* data);

/**
 * Tokenize a string.
 *
 * Delimiters terminate the current token and are thrown away.
 */

value_array_t* tokenize(const char* str, const char* delimiters) {
  value_array_t* result = make_value_array(1);
  char token_data[1024];
  int cpos = 0;
  for (int i = 0; (i < strlen(str)); i++) {
    char ch = str[i];
    if (string_contains_char(delimiters, ch)) {
      token_data[cpos++] = '\0';
      if (strlen(token_data) > 0) {
        add_duplicate(result, token_data);
      }
      cpos = 0;
    } else {
      token_data[cpos++] = ch;
    }
  }
  token_data[cpos++] = '\0';
  if (strlen(token_data) > 0) {
    add_duplicate(result, token_data);
  }

  return result;
}

/**
 * Add a *copy* of the string named data to the token list.
 */
void add_duplicate(value_array_t* token_array, const char* data) {
  value_array_add(token_array, str_to_value(string_duplicate(data)));
}
#line 2 "trace.c"
#ifndef _TRACE_H_
#define _TRACE_H_

#include <stdio.h>

#define TRACE()                                                                \
  do {                                                                         \
    fprintf(stderr, "TRACE file=%s line=%d\n", __FILE__, __LINE__);            \
  } while (0)

#define WARN(msg)                                                              \
  do {                                                                         \
    fprintf(stderr, "WARN file=%s line=%d '%s'\n", __FILE__, __LINE__, msg);   \
  } while (0)

#endif /* _TRACE_H_ */
#line 2 "value.c"

#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>

typedef union {
  uint64_t u64;
  uint64_t i64;
  char* str;
  void* ptr;
  void* dbl;
} value_t;

typedef struct {
  union {
    uint64_t u64;
    uint64_t i64;
    char* str;
    void* ptr;
    void* dbl;
    value_t val;
  };
  boolean_t found;
} value_result_t;

#define u64_to_value(x) ((value_t){.u64 = x})
#define i64_to_value(x) ((value_t){.i64 = x})
#define str_to_value(x) ((value_t){.str = x})
#define ptr_to_value(x) ((value_t){.ptr = x})
#define dbl_to_value(x) ((value_t){.dbl = x})

#endif /* _VALUE_H_ */
#line 2 "value-array.c"

/**
 * @file ptr-array.c
 *
 * This file contains a growable array of pointers which is
 * significantly easier to use than array.c though it is not
 * contiguous.
 */

#ifndef _VALUE_ARRAY_H_
#define _VALUE_ARRAY_H_

struct value_array_S {
  uint32_t length;
  uint32_t capacity;
  value_t* elements;
};

typedef struct value_array_S value_array_t;

extern value_array_t* make_value_array(uint32_t initial_capacity);
extern value_t value_array_get(value_array_t* array, uint32_t index);
extern void value_array_add(value_array_t* array, value_t element);

#endif /* _VALUE_ARRAY_H_ */

value_array_t* make_value_array(uint32_t initial_capacity) {
  if (initial_capacity == 0) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }

  value_array_t* result = malloc_struct(value_array_t);
  result->capacity = initial_capacity;
  result->elements
      = (value_t*) malloc_bytes(sizeof(value_t) * initial_capacity);

  return result;
}

value_t value_array_get(value_array_t* array, uint32_t index) {
  if (index < array->length) {
    return array->elements[index];
  }
  fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
}

void value_array_add(value_array_t* array, value_t element) {
  if (array->length == array->capacity) {
    uint32_t new_capacity = array->capacity * 2;
    value_t* new_elements
        = (value_t*) (malloc_bytes(sizeof(value_t) * new_capacity));
    for (int i = 0; i < array->capacity; i++) {
      new_elements[i] = array->elements[i];
    }
    array->capacity = new_capacity;
    free(array->elements);
    array->elements = new_elements;
    value_array_add(array, element);
    return;
  }
  array->elements[(array->length)++] = element;
}
#endif /* C_ARMYKNIFE_LIB_IMPL */
