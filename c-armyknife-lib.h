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

#line 1 "boolean.h"
#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

typedef int boolean_t;

#define true ((boolean_t) 1)
#define false ((boolean_t) 0)

#endif /* _BOOLEAN_H_ */
#ifndef _CT_ASSERT_
#define _CT_ASSERT_

// Do a compile time assertion. Using shorter name so that if someone
// google's this they are more likely to find out where I got
// this. Note that this only works where a statement is allowed (not
// top-level in a file).

// When this fails, you'll see something like:
//
// main.c:18:3: error: array size is negative
//
// We can change to use vendor specific version in the future.

#define ct_assert(e) ((void) sizeof(char[1 - 2 * !(e)]))

#endif /* _CT_ASSERT_ */
// SSCF generated file from: type.c

#line 10 "type.c"
#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdint.h>
#include <stdlib.h>

struct byte_buffer_S;
struct reference_S;
struct type_S;

typedef int (*compare_references_fn_t)(struct reference_S a,
                                       struct reference_S b);
typedef void (*append_text_representation_fn_t)(struct byte_buffer_S* buffer,
                                                struct reference_S object);
typedef uint64_t (*hash_reference_fn_t)(struct reference_S object);

struct type_S {
  char* name;
  int size;
  compare_references_fn_t compare_fn;
  append_text_representation_fn_t append_fn;
  hash_reference_fn_t hash_fn;
};
typedef struct type_S type_t;

extern type_t* intern_type(type_t type);

extern type_t uint8_type_constant;
extern type_t uint16_type_constant;
extern type_t uint32_type_constant;
extern type_t uint64_type_constant;
extern type_t char_ptr_type_constant;

static inline type_t* uint64_type() { return &uint64_type_constant; }

static inline type_t* char_ptr_type() { return &char_ptr_type_constant; }

// TODO: global constants for standard types like uint64_t and void*

type_t* intern_type(type_t type) {
  // HERE: malloc storage space and copy type to the heap.
  return NULL;
}

#endif /* _TYPE_H_ */
// SSCF generated file from: fatal-error.c

#line 19 "fatal-error.c"
#ifndef _FATAL_ERROR_H_
#define _FATAL_ERROR_H_

typedef enum {
  ERROR_UKNOWN,
  ERROR_MEMORY_ALLOCATION,
  ERROR_MEMORY_FREE_NULL,
  ERROR_REFERENCE_NOT_EXPECTED_TYPE,
  ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER,
  ERROR_ACCESS_OUT_OF_BOUNDS,
  ERROR_NOT_REACHED,
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);

#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */
// SSCF generated file from: reference.c

#line 17 "reference.c"
#ifndef _REFERENCE_H_
#define _REFERENCE_H_

#include <stdint.h>

struct reference_S {
  type_t* underlying_type;
  void* pointer;
};
typedef struct reference_S reference_t;

static inline reference_t reference_of(type_t* type, void* pointer) {
  reference_t result;
  result.underlying_type = type;
  result.pointer = pointer;
  return result;
}

static inline uint64_t reference_to_uint64(reference_t reference) {
  if (reference.underlying_type != &uint64_type_constant) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint64_t*) reference.pointer);
}

#endif /* _REFERENCE_H_ */
// SSCF generated file from: allocate.c

#line 13 "allocate.c"
#ifndef _ALLOCATE_H_
#define _ALLOCATE_H_

#include <stdint.h>

extern uint8_t* checked_malloc(char* file, int line, uint64_t amount);
extern void checked_free(char* file, int line, void* pointer);

#define malloc_bytes(amount) (checked_malloc(__FILE__, __LINE__, amount))
#define free_bytes(ptr) (checked_free(__FILE__, __LINE__, ptr))

#define malloc_struct(struct_name)                                             \
  ((struct_name*) (checked_malloc(__FILE__, __LINE__, sizeof(struct_name))))

#endif /* _ALLOCATE_H_ */
// SSCF generated file from: array.c

#line 17 "array.c"
#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdint.h>

typedef struct {
  type_t* element_type;
  uint32_t length;
  uint32_t capacity;
  uint8_t data[0];
} array_t;

extern array_t* make_array(type_t* element_type, uint32_t initial_capacity);
extern uint64_t array_length(array_t* arr);
extern reference_t array_get_reference(array_t* arr, uint64_t position);
__attribute__((warn_unused_result)) extern array_t*
    array_add(array_t* arr, reference_t element);

#endif /* _ARRAY_H_ */
// SSCF generated file from: byte-array.c

#line 10 "byte-array.c"
#ifndef _BYTE_ARRAY_H_
#define _BYTE_ARRAY_H_

#include <stdint.h>
#include <string.h>

typedef struct {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
} byte_array_t;

extern byte_array_t* make_byte_array(uint32_t initial_capacity);
extern uint64_t byte_array_length(byte_array_t* arr);
extern uint8_t byte_array_get(byte_array_t* arr, uint64_t position);
extern char* byte_array_c_substring(byte_array_t* arr, uint64_t start,
                                    uint64_t end);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_byte(byte_array_t* arr, uint8_t byte);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_bytes(byte_array_t* arr, uint8_t* bytes,
                            uint64_t n_bytes);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_string(byte_array_t* arr, const char* str);

#endif /* _BYTE_ARRAY_H_ */
// SSCF generated file from: io.c

#line 13 "io.c"
#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_file_contents(byte_array_t* bytes, char* file_name);
extern void byte_array_write_file(byte_array_t* bytes, char* file_name);

#endif /* _IO_H_ */
// SSCF generated file from: string-util.c

#line 13 "string-util.c"
#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <stdint.h>

extern int string_is_null_or_empty(const char* str1);
extern int string_equal(const char* str1, const char* str2);
extern int string_starts_with(const char* str1, const char* str2);
extern int string_ends_with(const char* str1, const char* str2);
extern int string_contains_char(const char* str, const char ch);
extern uint64_t string_to_uint64(const char* str);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern uint64_t string_parse_uint64(const char* string);
extern char* string_duplicate(const char* src);

#endif /* _STRING_UTIL_H_ */
// SSCF generated file from: tokenizer.c

#line 9 "tokenizer.c"
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "array.h"

extern array_t* tokenize(const char* str, const char* delimiters);

#endif /* _TOKENIZER_H_ */
#ifdef C_ARMYKNIFE_LIB_IMPL
#line 1 "allocate.c"
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
extern void checked_free(char* file, int line, void* pointer);

#define malloc_bytes(amount) (checked_malloc(__FILE__, __LINE__, amount))
#define free_bytes(ptr) (checked_free(__FILE__, __LINE__, ptr))

#define malloc_struct(struct_name)                                             \
  ((struct_name*) (checked_malloc(__FILE__, __LINE__, sizeof(struct_name))))

#endif /* _ALLOCATE_H_ */

// ======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "boolean.h"
#include "fatal-error.h"

boolean_t is_initialized = false;
boolean_t should_log_value = false;

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
  memset(result, 0, amount);
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
  free(pointer);
}
#line 1 "array.c"
/**
 * @file array.c
 *
 * This file contains a growable array of 64 bit values. Any function
 * that changes the length of an array may return a new pointer.
 *
 * (For scheme vectors, we actually will have a different
 * implemetation that doesn't have a capacity and knows it is holding
 * tagged_reference_t which are like 128 bits in size, not 64 bits.
 */

// ======================================================================
// This is block is extraced to array.h
// ======================================================================

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdint.h>

typedef struct {
  type_t* element_type;
  uint32_t length;
  uint32_t capacity;
  uint8_t data[0];
} array_t;

extern array_t* make_array(type_t* element_type, uint32_t initial_capacity);
extern uint64_t array_length(array_t* arr);
extern reference_t array_get_reference(array_t* arr, uint64_t position);
__attribute__((warn_unused_result)) extern array_t*
    array_add(array_t* arr, reference_t element);

#endif /* _ARRAY_H_ */

// ======================================================================

#include <stdlib.h>

#include "allocate.h"
#include "array.h"
#include "fatal-error.h"

static inline void* array_address_of_element(array_t* array,
                                             uint64_t position) {
  return &(array->data[position * array->element_type->size]);
}

/**
 * Make an array with the given initial_capacity.
 */
array_t* make_array(type_t* type, uint32_t initial_capacity) {
  int element_size = type->size;
  if (element_size < 0) {
    fatal_error(ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER);
  }
  array_t* result = (array_t*) (malloc_bytes(
      sizeof(array_t) + (element_size * initial_capacity)));
  result->element_type = type;
  result->capacity = initial_capacity;
  return result;
}

/**
 * Return the number of actual entries in an array.
 */
uint64_t array_length(array_t* arr) { return arr->length; }

/**
 * Get the nth element from an array.
 */
reference_t array_get(array_t* array, uint64_t position) {
  if (position < array->length) {
    return reference_of(array->element_type,
                        array_address_of_element(array, position));
  } else {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Add an element to the end of an array.
 */
__attribute__((warn_unused_result)) array_t* array_add(array_t* array,
                                                       reference_t reference) {
  if (reference.underlying_type != array->element_type) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  int size = array->element_type->size;
  if (array->length < array->capacity) {
    memcpy(array_address_of_element(array, array->length), reference.pointer,
           size);
    array->length++;
    return array;
  } else {
    array_t* result = make_array(array->element_type, array->capacity * 2);
    memcpy(array_address_of_element(result, 0),
           array_address_of_element(array, 0), size * array->length);
    free_bytes(array);
    return result;
  }
}
#line 1 "byte-array.c"
/**
 * @file byte-array.c
 */

// ======================================================================
// This is block is extraced to byte-array.h
// ======================================================================

#ifndef _BYTE_ARRAY_H_
#define _BYTE_ARRAY_H_

#include <stdint.h>
#include <string.h>

typedef struct {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
} byte_array_t;

extern byte_array_t* make_byte_array(uint32_t initial_capacity);
extern uint64_t byte_array_length(byte_array_t* arr);
extern uint8_t byte_array_get(byte_array_t* arr, uint64_t position);
extern char* byte_array_c_substring(byte_array_t* arr, uint64_t start,
                                    uint64_t end);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_byte(byte_array_t* arr, uint8_t byte);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_bytes(byte_array_t* arr, uint8_t* bytes,
                            uint64_t n_bytes);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_string(byte_array_t* arr, const char* str);

#endif /* _BYTE_ARRAY_H_ */

// ======================================================================

#include <stdlib.h>

#include "allocate.h"
#include "byte-array.h"
#include "ct-assert.h"
#include "fatal-error.h"

byte_array_t* make_byte_array(uint32_t initial_capacity) {

  // We make the assumption that casting (char*) to (uint8_t*) and
  // vice-versa is completely reasonable which it is on all modern
  // architecures.
  ct_assert(sizeof(char) == 1);

  byte_array_t* result
      = (byte_array_t*) (malloc_bytes(initial_capacity + sizeof(byte_array_t)));
  result->capacity = initial_capacity;
  return result;
}

uint64_t byte_array_length(byte_array_t* array) { return array->length; }

uint8_t byte_array_get(byte_array_t* arr, uint64_t position) {
  if (position < arr->length) {
    return arr->elements[position];
  } else {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Extract a newly allocated string contain the bytes from start to
 * end (appending a zero byte to make sure it's a legal C string).
 */
char* byte_array_c_substring(byte_array_t* arr, uint64_t start, uint64_t end) {
  // Add one extra byte for a NUL string terminator byte
  char* result = (char*) (malloc_bytes(end - start + 1));
  for (int i = start; i < end; i++) {
    result[i - start] = arr->elements[i];
  }
  return result;
}

__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_byte(byte_array_t* arr, uint8_t element) {
  if (arr->length < arr->capacity) {
    arr->elements[arr->length] = element;
    arr->length++;
    return arr;
  } else {
    byte_array_t* result = make_byte_array(arr->capacity * 2);
    for (int i = 0; i < arr->length; i++) {
      result = byte_array_append_byte(result, byte_array_get(arr, i));
    }
    free_bytes(arr);

    return result;
  }
}

__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_bytes(byte_array_t* arr, uint8_t* bytes,
                            uint64_t n_bytes) {
  for (int i = 0; i < n_bytes; i++) {
    arr = byte_array_append_byte(arr, bytes[i]);
  }
  return arr;
}

__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_string(byte_array_t* arr, const char* str) {
  return byte_array_append_bytes(arr, (uint8_t*) str, strlen(str));
}
#line 1 "fatal-error.c"
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
  ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER,
  ERROR_ACCESS_OUT_OF_BOUNDS,
  ERROR_NOT_REACHED,
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);

#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */

// ======================================================================

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include "fatal-error.h"

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
  case ERROR_NOT_REACHED:
    return "ERROR_NOT_REACHED";
  default:
    return "error";
  }
}

void print_error_code_name(int error_code) {
#ifndef NO_READABLE_ERROR_CODES
  fprintf(stderr, " ");
  do {
    fprintf(stderr, "*** ");
    fprintf(stderr, "%d -- %s", error_code,
            fatal_error_code_to_string(error_code));
    fprintf(stderr, " ***\n");
  } while (0);
#else
  fprintf(stderr, "%d %s\n", error_code, "ERROR_UKNOWN");
#endif /* NO_READABLE_ERROR_CODES */
}
#line 1 "io.c"
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

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_file_contents(byte_array_t* bytes, char* file_name);
extern void byte_array_write_file(byte_array_t* bytes, char* file_name);

#endif /* _IO_H_ */

// ======================================================================

#include <stdio.h>

#include "byte-array.h"
#include "io.h"

byte_array_t* byte_array_append_file_contents(byte_array_t* bytes,
                                              char* file_name) {
  FILE* file = fopen(file_name, "r");
  uint8_t buffer[1024];

  while (1) {
    uint64_t n_read = fread(buffer, 1, sizeof(buffer), file);
    if (n_read == 0) {
      break;
    }
    bytes = byte_array_append_bytes(bytes, buffer, n_read);
  }

  fclose(file);

  return bytes;
}

// TODO(jawilson): implement
void byte_array_write_file(byte_array_t* bytes, char* file_name) {}
#line 1 "reference.c"
/**
 * C does not have parameterized types which makes generic containers
 * more difficult to work with.
 *
 * Therefore we we resort to dynamic (aka runtime) type checking of
 * the generic parameters.  types are interned so we can check a type
 * with a single comparison of a type_t pointer. Additionally, we pass
 * these structs by value which the C compiler can probably return in
 * registers.
 *
 * Containers which store elements of the same type only need to store
 * the type once so this doesn't impact the overall space occupied by
 * the collection.
 */

#ifndef _REFERENCE_H_
#define _REFERENCE_H_

#include <stdint.h>

struct reference_S {
  type_t* underlying_type;
  void* pointer;
};
typedef struct reference_S reference_t;

static inline reference_t reference_of(type_t* type, void* pointer) {
  reference_t result;
  result.underlying_type = type;
  result.pointer = pointer;
  return result;
}

static inline uint64_t reference_to_uint64(reference_t reference) {
  if (reference.underlying_type != &uint64_type_constant) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint64_t*) reference.pointer);
}

#endif /* _REFERENCE_H_ */
#line 1 "string-util.c"
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
extern int string_contains_char(const char* str, const char ch);
extern uint64_t string_to_uint64(const char* str);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern uint64_t string_parse_uint64(const char* string);
extern char* string_duplicate(const char* src);

#endif /* _STRING_UTIL_H_ */

// ======================================================================

#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "string-util.h"

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

int string_contains_char(const char* str, char ch) {
  if (string_is_null_or_empty(str)) {
    return 0;
  }
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == ch) {
      return 1;
    }
  }
  return 0;
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
#line 1 "tokenizer.c"
/**
 * @file tokenizer.c
 *
 * This contains a routine to split a string into a series of tokens
 * seperated via delimiters.
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "array.h"

extern array_t* tokenize(const char* str, const char* delimiters);

#endif /* _TOKENIZER_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "fatal-error.h"
#include "string-util.h"
#include "tokenizer.h"

array_t* add_duplicate(array_t* token_array, const char* data);

/**
 * Tokenize a string.
 *
 * Delimiters terminate the current token and are thrown away.
 */
array_t* tokenize(const char* str, const char* delimiters) {
  array_t* result = make_array(char_ptr_type(), 4);
  char token_data[1024];
  int cpos = 0;
  for (int i = 0; (i < strlen(str)); i++) {
    char ch = str[i];
    if (string_contains_char(delimiters, ch)) {
      token_data[cpos++] = '\0';
      if (strlen(token_data) > 0) {
        result = add_duplicate(result, token_data);
      }
      cpos = 0;
    } else {
      token_data[cpos++] = ch;
    }
  }
  token_data[cpos++] = '\0';
  if (strlen(token_data) > 0) {
    result = add_duplicate(result, token_data);
  }

  return result;
}

/**
 * Add a *copy* of the string named data to the token list.
 */
array_t* add_duplicate(array_t* token_array, const char* data) {
  return array_add(token_array, reference_of(token_array->element_type,
                                             string_duplicate(data)));
}
#line 1 "type.c"
/**
 * @file type.c
 *
 * This is a runtime type so that containers can do dynamic type
 * checking since the C compiler isn't capable of type checking
 * automatically.
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdint.h>
#include <stdlib.h>

struct byte_buffer_S;
struct reference_S;
struct type_S;

typedef int (*compare_references_fn_t)(struct reference_S a,
                                       struct reference_S b);
typedef void (*append_text_representation_fn_t)(struct byte_buffer_S* buffer,
                                                struct reference_S object);
typedef uint64_t (*hash_reference_fn_t)(struct reference_S object);

struct type_S {
  char* name;
  int size;
  compare_references_fn_t compare_fn;
  append_text_representation_fn_t append_fn;
  hash_reference_fn_t hash_fn;
};
typedef struct type_S type_t;

extern type_t* intern_type(type_t type);

extern type_t uint8_type_constant;
extern type_t uint16_type_constant;
extern type_t uint32_type_constant;
extern type_t uint64_type_constant;
extern type_t char_ptr_type_constant;

static inline type_t* uint64_type() { return &uint64_type_constant; }

static inline type_t* char_ptr_type() { return &char_ptr_type_constant; }

// TODO: global constants for standard types like uint64_t and void*

type_t* intern_type(type_t type) {
  // HERE: malloc storage space and copy type to the heap.
  return NULL;
}

#endif /* _TYPE_H_ */

type_t uint8_type_constant = {
    .name = "uint8_t",
    .size = sizeof(uint8_t),
};

type_t uint16_type_constant = {
    .name = "uint16_t",
    .size = sizeof(uint16_t),
};

type_t uint32_type_constant = {
    .name = "uint32_t",
    .size = sizeof(uint32_t),
};

type_t uint64_type_constant = {
    .name = "uint64_t",
    .size = sizeof(uint64_t),
};

type_t char_type_constant = {
    .name = "char",
    .size = sizeof(char),
};

type_t double_type_constant = {
    .name = "double",
    .size = sizeof(double),
};

type_t float_type_constant = {
    .name = "float",
    .size = sizeof(float),
};

type_t char_ptr_type_constant = {
    .name = "char*",
    .size = sizeof(char*),
};

// TODO(jawilson): more pointer types for the built in C types.
#endif /* C_ARMYKNIFE_LIB_IMPL */
