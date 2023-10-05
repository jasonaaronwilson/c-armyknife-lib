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
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);

#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */
// SSCF generated file from: ct-assert.c

#line 8 "ct-assert.c"
#ifndef _CT_ASSERT_H_
#define _CT_ASSERT_H_

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

#endif /* _CT_ASSERT_H_ */
// SSCF generated file from: boolean.c

#line 13 "boolean.c"
#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

typedef int boolean_t;

#define true ((boolean_t) 1)
#define false ((boolean_t) 0)

#endif /* _BOOLEAN_H_ */
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
// SSCF generated file from: ct-assert.c

#line 8 "ct-assert.c"
#ifndef _CT_ASSERT_H_
#define _CT_ASSERT_H_

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

#endif /* _CT_ASSERT_H_ */
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
extern int string_contains_char(const char* str, const char ch);
extern uint64_t string_to_uint64(const char* str);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern uint64_t string_parse_uint64(const char* string);
extern char* string_duplicate(const char* src);
extern char* string_append(const char* a, const char* b);

#endif /* _STRING_UTIL_H_ */
// SSCF generated file from: byte-array.c

#line 10 "byte-array.c"
#ifndef _BYTE_ARRAY_H_
#define _BYTE_ARRAY_H_

#include <stdint.h>
#include <string.h>

struct byte_array_S {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
};

typedef struct byte_array_S byte_array_t;

extern byte_array_t* make_byte_array(uint32_t initial_capacity);

extern uint64_t byte_array_length(byte_array_t* byte_array);

extern uint8_t byte_array_get(byte_array_t* byte_array, uint64_t position);

extern char* byte_array_c_substring(byte_array_t* byte_array, uint64_t start,
                                    uint64_t end);

extern char* byte_array_to_c_strring(byte_array_t* byte_array);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_byte(byte_array_t* byte_array, uint8_t byte);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_bytes(byte_array_t* byte_array, uint8_t* bytes,
                            uint64_t n_bytes);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_string(byte_array_t* byte_array, const char* str);

#endif /* _BYTE_ARRAY_H_ */
// SSCF generated file from: type.c

#line 10 "type.c"
#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_TYPE_PARAMETERS 8

struct reference_S;
struct type_S;

typedef int (*compare_references_fn_t)(struct reference_S a,
                                       struct reference_S b);

typedef uint64_t (*hash_reference_fn_t)(struct reference_S object);

typedef byte_array_t* (*append_text_representation_fn_t)(
    byte_array_t* byte_array, struct reference_S object);

struct type_S {
  char* name;
  int size;
  int alignment;
  uint64_t number_of_parameters;
  struct type_S* parameters[MAX_TYPE_PARAMETERS];
  compare_references_fn_t compare_fn;
  hash_reference_fn_t hash_fn;
  append_text_representation_fn_t append_fn;
};
typedef struct type_S type_t;

extern type_t* intern_type(type_t type);

extern type_t char_ptr_type_constant;
extern type_t nil_type_constant;
extern type_t self_ptr_type_constant;

extern type_t uint64_type_constant;
extern type_t uint32_type_constant;
extern type_t uint16_type_constant;
extern type_t uint8_type_constant;

extern type_t int64_type_constant;
extern type_t int32_type_constant;
extern type_t int16_type_constant;
extern type_t int8_type_constant;

static inline type_t* nil_type() { return &nil_type_constant; }
static inline type_t* char_ptr_type() { return &char_ptr_type_constant; }
static inline type_t* self_ptr_type() { return &self_ptr_type_constant; }

static inline type_t* uint64_type() { return &uint64_type_constant; }
static inline type_t* uint32_type() { return &uint32_type_constant; }
static inline type_t* uint16_type() { return &uint16_type_constant; }
static inline type_t* uint8_type() { return &uint8_type_constant; }

static inline type_t* int64_type() { return &int64_type_constant; }
static inline type_t* int32_type() { return &int32_type_constant; }
static inline type_t* int16_type() { return &int16_type_constant; }
static inline type_t* int8_type() { return &int8_type_constant; }

// TODO: global constants for standard types like uint64_t and void*

type_t* intern_type(type_t type) {
  WARN("intern_type is not actually doing interning");
  type_t* result = (type_t*) malloc_copy_of((uint8_t*) &type, sizeof(type));
  return result;
}

#define pointer_t(t) t*

#endif /* _TYPE_H_ */
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

static inline int compare_references(reference_t ref_a, reference_t ref_b) {
  if (ref_a.underlying_type != ref_b.underlying_type) {
    // An aribrary ordering based on the "random" layout of references
    // in memory.
    return (int) (((uint64_t) ref_a.underlying_type)
                  - ((uint64_t) ref_b.underlying_type));
  }
  return ref_a.underlying_type->compare_fn(ref_a, ref_b);
}

static inline reference_t nil() { return reference_of(nil_type(), 0); }

static inline reference_t reference_of_char_ptr(char** pointer) {
  reference_t result;
  result.underlying_type = char_ptr_type();
  result.pointer = pointer;
  return result;
}

static inline char* dereference_char_ptr(reference_t reference) {
  if (reference.underlying_type != char_ptr_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((char**) reference.pointer);
}

// Unsigned Numbers

// ----- uint64_t -----

static inline reference_t reference_of_uint64(uint64_t* pointer) {
  reference_t result;
  result.underlying_type = uint64_type();
  result.pointer = pointer;
  return result;
}

static inline uint64_t dereference_uint64(reference_t reference) {
  if (reference.underlying_type != uint64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint64_t*) reference.pointer);
}

static inline void write_to_uint64_reference(reference_t reference,
                                             uint64_t value) {
  if (reference.underlying_type != uint64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint64_t*) reference.pointer) = value;
}

// ----- uint32 -----

static inline reference_t reference_of_uint32(uint32_t* pointer) {
  reference_t result;
  result.underlying_type = uint32_type();
  result.pointer = pointer;
  return result;
}

static inline uint32_t dereference_uint32(reference_t reference) {
  if (reference.underlying_type != uint32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint32_t*) reference.pointer);
}

static inline void write_to_uint32_reference(reference_t reference,
                                             uint32_t value) {
  if (reference.underlying_type != uint32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint32_t*) reference.pointer) = value;
}

// ----- uint16 -----

static inline reference_t reference_of_uint16(uint16_t* pointer) {
  reference_t result;
  result.underlying_type = uint16_type();
  result.pointer = pointer;
  return result;
}

static inline uint16_t dereference_uint16(reference_t reference) {
  if (reference.underlying_type != uint16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint16_t*) reference.pointer);
}

static inline void write_to_uint16_reference(reference_t reference,
                                             uint16_t value) {
  if (reference.underlying_type != uint16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint16_t*) reference.pointer) = value;
}

// ----- uint8 -----

static inline reference_t reference_of_uint8(uint8_t* pointer) {
  reference_t result;
  result.underlying_type = uint8_type();
  result.pointer = pointer;
  return result;
}

static inline uint8_t dereference_uint8(reference_t reference) {
  if (reference.underlying_type != uint8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint8_t*) reference.pointer);
}

static inline void write_to_uint8_reference(reference_t reference,
                                             uint8_t value) {
  if (reference.underlying_type != uint8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint8_t*) reference.pointer) = value;
}

// Signed Integers

// ----- int64_t -----

static inline reference_t reference_of_int64(int64_t* pointer) {
  reference_t result;
  result.underlying_type = int64_type();
  result.pointer = pointer;
  return result;
}

static inline int64_t dereference_int64(reference_t reference) {
  if (reference.underlying_type != int64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((int64_t*) reference.pointer);
}

static inline void write_to_int64_reference(reference_t reference,
                                            int64_t value) {
  if (reference.underlying_type != uint64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((int64_t*) reference.pointer) = value;
}

// ----- int32_t -----

static inline reference_t reference_of_int32(int32_t* pointer) {
  reference_t result;
  result.underlying_type = int32_type();
  result.pointer = pointer;
  return result;
}

static inline int32_t dereference_int32(reference_t reference) {
  if (reference.underlying_type != int32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((int32_t*) reference.pointer);
}

static inline void write_to_int32_reference(reference_t reference,
                                            int32_t value) {
  if (reference.underlying_type != int32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((int32_t*) reference.pointer) = value;
}

// ----- int16_t -----

static inline reference_t reference_of_int16(int16_t* pointer) {
  reference_t result;
  result.underlying_type = int16_type();
  result.pointer = pointer;
  return result;
}

static inline int16_t dereference_int16(reference_t reference) {
  if (reference.underlying_type != int16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((int16_t*) reference.pointer);
}

static inline void write_to_int16_reference(reference_t reference,
                                            int16_t value) {
  if (reference.underlying_type != int16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((int16_t*) reference.pointer) = value;
}

// ----- int8_t -----

static inline reference_t reference_of_int8(int8_t* pointer) {
  reference_t result;
  result.underlying_type = int8_type();
  result.pointer = pointer;
  return result;
}

static inline uint64_t dereference_int8(reference_t reference) {
  if (reference.underlying_type != int8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((int8_t*) reference.pointer);
}

static inline void write_to_int8_reference(reference_t reference,
                                           int8_t value) {
  if (reference.underlying_type != int8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((int8_t*) reference.pointer) = value;
}

// -----

static inline byte_array_t*
    byte_array_append_reference(byte_array_t* byte_array,
                                reference_t reference) {
  return reference.underlying_type->append_fn(byte_array, reference);
}

#endif /* _REFERENCE_H_ */
// SSCF generated file from: tuple.c

#line 16 "tuple.c"
#ifndef _TUPLE_H_
#define _TUPLE_H_

#include <stdint.h>

// Technically there is no struct that can really represent a tuple
// and the alignment can be as little as one byte. I think this will
// eventually prove to be the wrong thing.
typedef struct {
  __attribute__((aligned(8))) uint8_t data[0];
} tuple_t;

extern type_t* intern_tuple_type(int number_of_parameters, ...);

// Rename all of these since they are in fact painfully long...

extern reference_t tuple_reference_of_element(reference_t tuple,
                                              uint64_t position);

extern reference_t tuple_reference_of_element_from_pointer(
    type_t* type, tuple_t* tuple_pointer, uint64_t position);

extern void tuple_write_element(reference_t tuple_ref, uint64_t position,
                                reference_t value);

extern struct byte_array_S* tuple_append_text(struct byte_array_S* byte_array,
                                              reference_t tuple_ref);

#endif /* _TUPLE_H_ */
// SSCF generated file from: array.c

#line 13 "array.c"
#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdint.h>

struct array_S {
  type_t* element_type;
  uint32_t length;
  uint32_t capacity;
  __attribute__((aligned(8))) uint8_t data[0];
};

#define array_t(t) struct array_S

extern array_t(|?|)* make_array(type_t* element_type, uint32_t initial_capacity);
extern uint64_t array_length(array_t(|?|)* arr);
extern reference_t array_get_reference(array_t(|?|)* arr, uint64_t position);
__attribute__((warn_unused_result)) extern array_t(|?|)*
    array_add(array_t(|?|)* arr, reference_t element);

#endif /* _ARRAY_H_ */
// SSCF generated file from: hashtree.c

#line 29 "hashtree.c"
#ifndef _HASHTREE_H_
#define _HASHTREE_H_

#include <stdint.h>

// Each node in the tree is simply a tuple of hashcode, left (pointer), right
// (pointer), K, and V.
#define hashtree_t(K, V) tuple_t

extern hashtree_t(K, V)
    * make_hashtree_node(type_t* key_type, type_t* value_type);

extern reference_t hashtree_get_reference_to_value(hashtree_t(K, V) * htree,
                                                   type_t* key_type,
                                                   type_t* value_type,
                                                   uint64_t hashcode,
                                                   reference_t key_reference);

extern boolean_t hashtree_insert(pointer_t(hashtree_t(K, V)) htree,
                                 type_t* key_type, type_t* value_type,
                                 uint64_t hashcode, reference_t key_reference,
                                 reference_t value_reference);

extern void hashtree_delete(pointer_t(hashtree_t(K, V)) htree, type_t* key_type,
                            type_t* value_type, reference_t key_reference);

#endif /* _HASHTREE_H_ */
// SSCF generated file from: hashtable.c

#line 8 "hashtable.c"
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdint.h>

struct hashtable_S {
  type_t* type;
  array_t(tuple_of_type(uint64_type(), K, V)) * storage;
  uint64_t number_of_keys;
};

#define hashtable_t(K, V) struct hashtable_S

extern hashtable_t(K, V)
    * make_hashtable(type_t* key_type, type_t* value_type,
                     uint32_t initial_capacity);
extern uint64_t hashtable_number_of_keys(hashtable_t(K, V) * hashtable);
extern reference_t hashtable_get_reference_to_value(hashtable_t(K, V)
                                                        * hashtable,
                                                    reference_t key_reference);
extern void hashtable_set_value(hashtable_t(K, V) * ht,
                                reference_t key_reference,
                                reference_t value_reference);
extern int hashtable_compare(hashtable_t(K, V) * a, hashtable_t(K, V) * b);

#endif /* _HASHTABLE_H_ */
// SSCF generated file from: io.c

#line 13 "io.c"
#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_file_contents(byte_array_t* bytes, char* file_name);
extern void byte_array_write_file(byte_array_t* bytes, char* file_name);

#endif /* _IO_H_ */
// SSCF generated file from: tokenizer.c

#line 9 "tokenizer.c"
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "array.h"

extern array_t(char*) * tokenize(const char* str, const char* delimiters);

#endif /* _TOKENIZER_H_ */
// SSCF generated file from: test.c

#line 13 "test.c"
#ifndef _TEST_H_
#define _TEST_H_

// Provide a convenient place to set a breakpoint
void armyknife_test_fail_exit() { exit(1); }

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

#include "allocate.h"
#include "boolean.h"
#include "fatal-error.h"

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
#line 2 "array.c"
/**
 * @file array.c
 *
 * This file contains a growable array of values. Any function that
 * changes the length of an array may return a new pointer.
 */

// ======================================================================
// This is block is extraced to array.h
// ======================================================================

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdint.h>

struct array_S {
  type_t* element_type;
  uint32_t length;
  uint32_t capacity;
  __attribute__((aligned(8))) uint8_t data[0];
};

#define array_t(t) struct array_S

extern array_t(|?|)* make_array(type_t* element_type, uint32_t initial_capacity);
extern uint64_t array_length(array_t(|?|)* arr);
extern reference_t array_get_reference(array_t(|?|)* arr, uint64_t position);
__attribute__((warn_unused_result)) extern array_t(|?|)*
    array_add(array_t(|?|)* arr, reference_t element);

#endif /* _ARRAY_H_ */

// ======================================================================

#include <stdlib.h>

#include "allocate.h"
#include "array.h"
#include "fatal-error.h"

static inline void* array_address_of_element(array_t(|?|)* array,
                                             uint64_t position) {
  void* result = &(array->data[0]) + position * array->element_type->size;
  return result;
}

/**
 * Make an array with the given initial_capacity.
 */
array_t(|?|)* make_array(type_t* type, uint32_t initial_capacity) {
  if (initial_capacity == 0) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }

  int element_size = type->size;
  if (element_size <= 0) {
    fatal_error(ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER);
  }
  array_t(|?|)* result = (array_t(|?|)*) (malloc_bytes(
      sizeof(array_t(|?|)) + (element_size * initial_capacity)));
  result->element_type = type;
  result->length = 0;
  result->capacity = initial_capacity;
  return result;
}

/**
 * Return the number of actual entries in an array.
 */
uint64_t array_length(array_t(|?|)* arr) { return arr->length; }

/**
 * Get the nth element from an array.
 */
reference_t array_get_reference(array_t(|?|)* array, uint64_t position) {
  if (position < array->length) {
    return reference_of(array->element_type,
                        array_address_of_element(array, position));
  } else {
    fprintf(stderr, "%s:%d: position is %lu but array length is %d\n", __FILE__,
            __LINE__, position, array->length);
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Add an element to the end of an array.
 */
__attribute__((warn_unused_result)) array_t(|?|)* array_add(array_t(|?|)* array,
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
    array_t(|?|)* result = make_array(array->element_type, array->capacity * 2);
    memcpy(array_address_of_element(result, 0),
           array_address_of_element(array, 0), size * array->length);
    result->length = array->length;
    free_bytes(array);
    array = NULL;
    return array_add(result, reference);
  }
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

typedef int boolean_t;

#define true ((boolean_t) 1)
#define false ((boolean_t) 0)

#endif /* _BOOLEAN_H_ */

// ======================================================================
// Currently no implementation
// ======================================================================
#line 2 "byte-array.c"
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

struct byte_array_S {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
};

typedef struct byte_array_S byte_array_t;

extern byte_array_t* make_byte_array(uint32_t initial_capacity);

extern uint64_t byte_array_length(byte_array_t* byte_array);

extern uint8_t byte_array_get(byte_array_t* byte_array, uint64_t position);

extern char* byte_array_c_substring(byte_array_t* byte_array, uint64_t start,
                                    uint64_t end);

extern char* byte_array_to_c_strring(byte_array_t* byte_array);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_byte(byte_array_t* byte_array, uint8_t byte);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_bytes(byte_array_t* byte_array, uint8_t* bytes,
                            uint64_t n_bytes);

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_string(byte_array_t* byte_array, const char* str);

#endif /* _BYTE_ARRAY_H_ */

// ======================================================================

#include <stdlib.h>

#include "allocate.h"
#include "byte-array.h"
#include "ct-assert.h"
#include "fatal-error.h"

/**
 * Make an empty byte array with the given initial capacity.
 */
byte_array_t* make_byte_array(uint32_t initial_capacity) {

  // We make the assumption that casting (char*) to (uint8_t*) and
  // vice-versa is completely reasonable which it is on all modern
  // architecures.
  ct_assert(sizeof(char) == 1);

  if (initial_capacity < 1) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }

  byte_array_t* result
      = (byte_array_t*) (malloc_bytes(initial_capacity + sizeof(byte_array_t)));
  result->capacity = initial_capacity;
  return result;
}

/**
 * Return the number of bytes that have been added to this byte array.
 */
uint64_t byte_array_length(byte_array_t* array) { return array->length; }

/**
 * Get a single byte from a byte array.
 */
uint8_t byte_array_get(byte_array_t* byte_array, uint64_t position) {
  if (position < byte_array->length) {
    return byte_array->elements[position];
  } else {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
}

/**
 * Extract a newly allocated string that contain the bytes from start
 * to end (appending a zero byte to make sure it's a legal C string).
 */
char* byte_array_c_substring(byte_array_t* byte_array, uint64_t start,
                             uint64_t end) {
  // Add one extra byte for a NUL string terminator byte
  char* result = (char*) (malloc_bytes(end - start + 1));
  for (int i = start; i < end; i++) {
    result[i - start] = byte_array->elements[i];
  }
  return result;
}

/**
 * Extract a newly allocated string that contain all of the bytes in the byte
 * buffer as a NU * terminated C string.
 */
char* byte_array_to_c_string(byte_array_t* byte_array) {
  return byte_array_c_substring(byte_array, 0, byte_array->length);
}

/**
 * Append a single byte to the byte array.
 */
__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_byte(byte_array_t* byte_array, uint8_t element) {
  if (byte_array->length < byte_array->capacity) {
    byte_array->elements[byte_array->length] = element;
    byte_array->length++;
    return byte_array;
  } else {
    byte_array_t* result = make_byte_array(byte_array->capacity * 2);
    for (int i = 0; i < byte_array->length; i++) {
      result = byte_array_append_byte(result, byte_array_get(byte_array, i));
    }
    free_bytes(byte_array);
    return byte_array_append_byte(result, element);
  }
}

/**
 * Append multiple bytes to the byte array.
 */
__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_bytes(byte_array_t* byte_array, uint8_t* bytes,
                            uint64_t n_bytes) {
  // Obviously this can be optimized...
  for (int i = 0; i < n_bytes; i++) {
    byte_array = byte_array_append_byte(byte_array, bytes[i]);
  }
  return byte_array;
}

/**
 * Append all of the bytes from a C string (except the ending NUL
 * char).
 */
__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_string(byte_array_t* byte_array, const char* str) {
  return byte_array_append_bytes(byte_array, (uint8_t*) str, strlen(str));
}
#line 2 "ct-assert.c"
/**
 * @file ct-assert.c
 *
 * Provide a basic compile time assert facility.
 */

#ifndef _CT_ASSERT_H_
#define _CT_ASSERT_H_

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

#endif /* _CT_ASSERT_H_ */
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
#line 2 "hashtable.c"
/**
 * @file hashtable.c
 *
 * A simple hashtable from keys to values.
 */

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdint.h>

struct hashtable_S {
  type_t* type;
  array_t(tuple_of_type(uint64_type(), K, V)) * storage;
  uint64_t number_of_keys;
};

#define hashtable_t(K, V) struct hashtable_S

extern hashtable_t(K, V)
    * make_hashtable(type_t* key_type, type_t* value_type,
                     uint32_t initial_capacity);
extern uint64_t hashtable_number_of_keys(hashtable_t(K, V) * hashtable);
extern reference_t hashtable_get_reference_to_value(hashtable_t(K, V)
                                                        * hashtable,
                                                    reference_t key_reference);
extern void hashtable_set_value(hashtable_t(K, V) * ht,
                                reference_t key_reference,
                                reference_t value_reference);
extern int hashtable_compare(hashtable_t(K, V) * a, hashtable_t(K, V) * b);

#endif /* _HASHTABLE_H_ */

// ======================================================================

#include <stdlib.h>

#define HT_ENTRY_HASHCODE_POSITION 0
#define HT_ENTRY_KEY_POSITION 1
#define HT_ENTRY_VALUE_POSITION 2

type_t* intern_hashtable_type(type_t* key_type, type_t* value_type);

/**
 * Make an array with the given initial_capacity.
 */
hashtable_t(K, V)
    * make_hashtable(type_t* key_type, type_t* value_type,
                     uint32_t initial_capacity) {
  if (initial_capacity == 0) {
    fatal_error(ERROR_ILLEGAL_INITIAL_CAPACITY);
  }
  type_t* hashtable_type = intern_hashtable_type(key_type, value_type);
  type_t* storage_type
      = intern_tuple_type(3, uint64_type(), key_type, value_type);
  hashtable_t(K, V)* result = malloc_struct(hashtable_t(K, V));
  result->type = hashtable_type;
  result->storage = make_array(storage_type, initial_capacity);
  result->storage->length = result->storage->capacity;
  return result;
}

/**
 * Return the number of actual entries in an array.
 */
uint64_t hashtable_size(hashtable_t(K, V) * ht) { return ht->number_of_keys; }

uint64_t hashtable_hash_key(hashtable_t(K, V) * ht, reference_t key_reference) {
  uint64_t hash = ht->type->parameters[0]->hash_fn(key_reference);
  // Reserve 0 so we can tell which buckets contain something. Any
  // value except 0 would be fine here but I choose a random looking
  // number that might be a prime.
  if (hash == 0) {
    hash = 113649;
  }
  return hash;
}

reference_t hashtable_get_reference_to_bucket(hashtable_t(K, V) * ht,
                                              uint64_t hashcode,
                                              uint64_t probe_number) {
  uint64_t position = (hashcode + probe_number) % ht->storage->length;
  return array_get_reference(ht->storage, position);
}

/**
 * Lookup a key in a hashtable.
 */
reference_t hashtable_get_reference_to_value(hashtable_t(K, V) * ht,
                                             reference_t key_reference) {
  uint64_t hashcode = hashtable_hash_key(ht, key_reference);
  for (int i = 0; true; i++) {
    reference_t bucket_reference
        = hashtable_get_reference_to_bucket(ht, hashcode, i);
    uint64_t stored_hashcode = dereference_uint64(tuple_reference_of_element(
        bucket_reference, HT_ENTRY_HASHCODE_POSITION));
    if (stored_hashcode == hashcode) {
      reference_t stored_key_reference
          = tuple_reference_of_element(bucket_reference, HT_ENTRY_KEY_POSITION);
      if (compare_references(key_reference, stored_key_reference) == 0) {
        return tuple_reference_of_element(bucket_reference,
                                          HT_ENTRY_VALUE_POSITION);
      }
    } else if (stored_hashcode == 0) {
      break;
    }
  }

  return nil();
}

void hashtable_set_value(hashtable_t(K, V) * ht, reference_t key_reference,
                         reference_t value_reference) {
  uint64_t hashcode = hashtable_hash_key(ht, key_reference);
  for (int i = 0; true; i++) {
    reference_t bucket_reference
        = hashtable_get_reference_to_bucket(ht, hashcode, i);
    uint64_t stored_hashcode = dereference_uint64(tuple_reference_of_element(
        bucket_reference, HT_ENTRY_HASHCODE_POSITION));
    if ((stored_hashcode == 0)
        || ((stored_hashcode == hashcode)
            && ((compare_references(key_reference, tuple_reference_of_element(
                                                       bucket_reference,
                                                       HT_ENTRY_KEY_POSITION)))
                == 0))) {
      tuple_write_element(bucket_reference, HT_ENTRY_HASHCODE_POSITION,
                          reference_of_uint64(&hashcode));
      tuple_write_element(bucket_reference, HT_ENTRY_KEY_POSITION,
                          key_reference);
      tuple_write_element(bucket_reference, HT_ENTRY_VALUE_POSITION,
                          value_reference);
      // TODO(jawilson): keep track of number of entries and possibly
      // grow if we exceed a certain load factor.
      return;
    }
  }
}

// ----------------------------------------------------------------------
// Some construction helper functions
// ----------------------------------------------------------------------

char* construct_hashtable_type_name(type_t* key_type, type_t* value_type) {
  byte_array_t* name = make_byte_array(32);
  name = byte_array_append_string(name, "hashtable_t(");
  name = byte_array_append_string(name, key_type->name);
  name = byte_array_append_string(name, ",");
  name = byte_array_append_string(name, value_type->name);
  name = byte_array_append_string(name, ")");
  char* result = byte_array_c_substring(name, 0, byte_array_length(name));
  free(name);
  return result;
}

type_t* intern_hashtable_type(type_t* key_type, type_t* value_type) {
  type_t hashtable_type;
  hashtable_type.name = construct_hashtable_type_name(key_type, value_type);
  hashtable_type.size = sizeof(struct hashtable_S);
  hashtable_type.alignment = alignof(struct hashtable_S);
  hashtable_type.number_of_parameters = 2;
  hashtable_type.parameters[0] = key_type;
  hashtable_type.parameters[1] = value_type;
  // TODO(jawilson): compare_fn, append_fn, hash_fn which are only
  // needed to print or use the hashtable itself as the key to another
  // hashtable...
  return intern_type(hashtable_type);
}
#line 2 "hashtree.c"
/**
 * @file hashtree.c
 *
 * A hashtree is a binary tree where the order of keys is based on the
 * hash of the key rather than the key itself (hence it appears
 * unsorted to an external observer).
 *
 * Since hashcodes are simply 64bit numbers, they can be very
 * efficiently compared versus complex keys (which obviously can then
 * take more time to compare) providing a constant factor speedup
 * versus a tree sorted by key. Additionally, hashtrees do not require
 * balancing operations like normal binary trees to provide O(lg(n))
 * operations under normal conditions which simplify their
 * implementation.
 *
 * Hashtrees have better space efficieny versus a hashtable but
 * obviously have slower lookup, insert, and delete (and will incur
 * more cache misses). The real reason we implement hashtrees is to
 * allow hashtables a simple but efficient mechanism to handle bucket
 * collisions and thus permit higher load in the hashtable itself
 * while still providing good performance.
 *
 * Like tuples, hashtrees do not carry their key and value types with
 * them so the types must be provided to all operations that operate
 * on them, one reason they be less desireable to use than hashtables.
 */

#ifndef _HASHTREE_H_
#define _HASHTREE_H_

#include <stdint.h>

// Each node in the tree is simply a tuple of hashcode, left (pointer), right
// (pointer), K, and V.
#define hashtree_t(K, V) tuple_t

extern hashtree_t(K, V)
    * make_hashtree_node(type_t* key_type, type_t* value_type);

extern reference_t hashtree_get_reference_to_value(hashtree_t(K, V) * htree,
                                                   type_t* key_type,
                                                   type_t* value_type,
                                                   uint64_t hashcode,
                                                   reference_t key_reference);

extern boolean_t hashtree_insert(pointer_t(hashtree_t(K, V)) htree,
                                 type_t* key_type, type_t* value_type,
                                 uint64_t hashcode, reference_t key_reference,
                                 reference_t value_reference);

extern void hashtree_delete(pointer_t(hashtree_t(K, V)) htree, type_t* key_type,
                            type_t* value_type, reference_t key_reference);

#endif /* _HASHTREE_H_ */

#define HTREE_HASHCODE_POSITION 0
#define HTREE_LEFT_POSITION 1
#define HTREE_RIGHT_POSITION 2
#define HTREE_KEY_POSITION 3
#define HTREE_VALUE_POSITION 4

#define tuple_write tuple_write_element
#define tuple_read tuple_reference_of_element

type_t* intern_hashtree_type(type_t* key_type, type_t* value_type) {
  return intern_tuple_type(5, uint64_type(), self_ptr_type(), self_ptr_type(),
                           key_type, value_type);
}

pointer_t(hashtree_t(K, V))
    make_empty_hashtree_node(type_t* key_type, type_t* value_type) {
  type_t* node_type = intern_hashtree_type(key_type, value_type);
  return (pointer_t(hashtree_t(K, V)))(malloc_bytes(node_type->size));
}

reference_t hashtree_get_reference_to_value(pointer_t(hashtree_t(K, V)) htree,
                                            type_t* key_type,
                                            type_t* value_type,
                                            uint64_t hashcode,
                                            reference_t key_reference) {
  type_t* node_type = intern_hashtree_type(key_type, value_type);
  reference_t node_ref = reference_of(node_type, htree);

  uint64_t node_hashcode = dereference_uint64(
      tuple_reference_of_element(node_ref, HTREE_HASHCODE_POSITION));

  if (node_hashcode == 0) {
    return nil();
  }

  if (hashcode == node_hashcode) {
    // TODO(jawilson): check key!!!
    return tuple_reference_of_element(node_ref, HTREE_VALUE_POSITION);
  } else if (hashcode < node_hashcode) {
    pointer_t(hashtree_t(K, V)) left = *(
        (hashtree_t(K, V)**) tuple_read(node_ref, HTREE_LEFT_POSITION).pointer);
    if (left == NULL) {
      return nil();
    } else {
      return hashtree_get_reference_to_value(left, key_type, value_type,
                                             hashcode, key_reference);
    }

  } else {
    // GO RIGHT
    return nil();
  }
}


/**
 * Insert (or replace) a mapping from K to V in the tree. If the value
 * is not already in the hashtable than this will always add a new
 * node as a leaf node.
 *
 * When the htree tuple has a current hashcode of zero, we assume it
 * is an empty root node and those overwrite an empty node. For this
 * reason, the input hashcode for the key should never itself be zero
 * (simply consistently map zero to any other value like hashtable
 * already does).
 */
boolean_t hashtree_insert(pointer_t(hashtree_t(K, V)) htree, type_t* key_type,
                          type_t* value_type, uint64_t hashcode,
                          reference_t key_reference,
                          reference_t value_reference) {

  if (hashcode == 0) {
    fatal_error(ERROR_ILLEGAL_ZERO_HASHCODE_VALUE);
  }

  type_t* node_type = intern_hashtree_type(key_type, value_type);
  reference_t node_ref = reference_of(node_type, htree);

  // TODO(jawilson): make sure types match
  uint64_t node_hashcode = dereference_uint64(
      tuple_reference_of_element(node_ref, HTREE_HASHCODE_POSITION));
  if (node_hashcode == 0) {
    tuple_write(node_ref, HTREE_HASHCODE_POSITION,
                reference_of_uint64(&hashcode));
    tuple_write(node_ref, HTREE_KEY_POSITION, key_reference);
    tuple_write(node_ref, HTREE_VALUE_POSITION, value_reference);
    return true;
  } else if (hashcode == node_hashcode) {
    // TODO(jawilson): make sure the keys are actually the same!
    tuple_write(node_ref, HTREE_VALUE_POSITION, value_reference);
    return false;
  } else if (hashcode < node_hashcode) {
    hashtree_t(K, V)* left = *(
        (hashtree_t(K, V)**) tuple_read(node_ref, HTREE_LEFT_POSITION).pointer);
    if (left == NULL) {
      hashtree_t(K, V)* new_node
          = make_empty_hashtree_node(key_type, value_type);
      reference_t new_node_ref = reference_of(node_type, new_node);
      reference_t new_node_pointer_ref
          = reference_of(node_type->parameters[HTREE_LEFT_POSITION], &new_node);
      tuple_write(new_node_ref, HTREE_HASHCODE_POSITION,
                  reference_of_uint64(&hashcode));
      tuple_write(new_node_ref, HTREE_KEY_POSITION, key_reference);
      tuple_write(new_node_ref, HTREE_VALUE_POSITION, value_reference);
      tuple_write(node_ref, HTREE_LEFT_POSITION, new_node_pointer_ref);
      return true;
    } else {
      return hashtree_insert(left, key_type, value_type, hashcode,
                             key_reference, value_reference);
    }
  } else {
    hashtree_t(K, V)* right
        = *((hashtree_t(K, V)**) tuple_read(node_ref, HTREE_RIGHT_POSITION)
                .pointer);
    if (right == NULL) {
      hashtree_t(K, V)* new_node
          = make_empty_hashtree_node(key_type, value_type);
      reference_t new_node_ref = reference_of(node_type, new_node);
      reference_t new_node_pointer_ref = reference_of(
          node_type->parameters[HTREE_RIGHT_POSITION], &new_node);
      tuple_write(new_node_ref, HTREE_HASHCODE_POSITION,
                  reference_of_uint64(&hashcode));
      tuple_write(new_node_ref, HTREE_KEY_POSITION, key_reference);
      tuple_write(new_node_ref, HTREE_VALUE_POSITION, value_reference);
      tuple_write_element(node_ref, HTREE_RIGHT_POSITION, new_node_pointer_ref);
      return true;
    } else {
      return hashtree_insert(right, key_type, value_type, hashcode,
                             key_reference, value_reference);
    }
  }
}

void hashtree_delete(pointer_t(hashtree_t(K, V)) htree, type_t* key_type,
                     type_t* value_type, reference_t key_reference) {
  fatal_error(ERROR_UNIMPLEMENTED);
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

__attribute__((warn_unused_result)) extern byte_array_t*
    byte_array_append_file_contents(byte_array_t* bytes, char* file_name);
extern void byte_array_write_file(byte_array_t* bytes, char* file_name);

#endif /* _IO_H_ */

// ======================================================================

#include <stdio.h>

#include "byte-array.h"
#include "io.h"

__attribute__((warn_unused_result)) byte_array_t*
    byte_array_append_file_contents(byte_array_t* bytes, char* file_name) {
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

void byte_array_write_file(byte_array_t* bytes, char* file_name) {
  FILE* file = fopen(file_name, "r");
  fwrite(&bytes->elements, 1, bytes->length, file);
  fclose(file);
}
#line 2 "reference.c"
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

static inline int compare_references(reference_t ref_a, reference_t ref_b) {
  if (ref_a.underlying_type != ref_b.underlying_type) {
    // An aribrary ordering based on the "random" layout of references
    // in memory.
    return (int) (((uint64_t) ref_a.underlying_type)
                  - ((uint64_t) ref_b.underlying_type));
  }
  return ref_a.underlying_type->compare_fn(ref_a, ref_b);
}

static inline reference_t nil() { return reference_of(nil_type(), 0); }

static inline reference_t reference_of_char_ptr(char** pointer) {
  reference_t result;
  result.underlying_type = char_ptr_type();
  result.pointer = pointer;
  return result;
}

static inline char* dereference_char_ptr(reference_t reference) {
  if (reference.underlying_type != char_ptr_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((char**) reference.pointer);
}

// Unsigned Numbers

// ----- uint64_t -----

static inline reference_t reference_of_uint64(uint64_t* pointer) {
  reference_t result;
  result.underlying_type = uint64_type();
  result.pointer = pointer;
  return result;
}

static inline uint64_t dereference_uint64(reference_t reference) {
  if (reference.underlying_type != uint64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint64_t*) reference.pointer);
}

static inline void write_to_uint64_reference(reference_t reference,
                                             uint64_t value) {
  if (reference.underlying_type != uint64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint64_t*) reference.pointer) = value;
}

// ----- uint32 -----

static inline reference_t reference_of_uint32(uint32_t* pointer) {
  reference_t result;
  result.underlying_type = uint32_type();
  result.pointer = pointer;
  return result;
}

static inline uint32_t dereference_uint32(reference_t reference) {
  if (reference.underlying_type != uint32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint32_t*) reference.pointer);
}

static inline void write_to_uint32_reference(reference_t reference,
                                             uint32_t value) {
  if (reference.underlying_type != uint32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint32_t*) reference.pointer) = value;
}

// ----- uint16 -----

static inline reference_t reference_of_uint16(uint16_t* pointer) {
  reference_t result;
  result.underlying_type = uint16_type();
  result.pointer = pointer;
  return result;
}

static inline uint16_t dereference_uint16(reference_t reference) {
  if (reference.underlying_type != uint16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint16_t*) reference.pointer);
}

static inline void write_to_uint16_reference(reference_t reference,
                                             uint16_t value) {
  if (reference.underlying_type != uint16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint16_t*) reference.pointer) = value;
}

// ----- uint8 -----

static inline reference_t reference_of_uint8(uint8_t* pointer) {
  reference_t result;
  result.underlying_type = uint8_type();
  result.pointer = pointer;
  return result;
}

static inline uint8_t dereference_uint8(reference_t reference) {
  if (reference.underlying_type != uint8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((uint8_t*) reference.pointer);
}

static inline void write_to_uint8_reference(reference_t reference,
                                             uint8_t value) {
  if (reference.underlying_type != uint8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((uint8_t*) reference.pointer) = value;
}

// Signed Integers

// ----- int64_t -----

static inline reference_t reference_of_int64(int64_t* pointer) {
  reference_t result;
  result.underlying_type = int64_type();
  result.pointer = pointer;
  return result;
}

static inline int64_t dereference_int64(reference_t reference) {
  if (reference.underlying_type != int64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((int64_t*) reference.pointer);
}

static inline void write_to_int64_reference(reference_t reference,
                                            int64_t value) {
  if (reference.underlying_type != uint64_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((int64_t*) reference.pointer) = value;
}

// ----- int32_t -----

static inline reference_t reference_of_int32(int32_t* pointer) {
  reference_t result;
  result.underlying_type = int32_type();
  result.pointer = pointer;
  return result;
}

static inline int32_t dereference_int32(reference_t reference) {
  if (reference.underlying_type != int32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((int32_t*) reference.pointer);
}

static inline void write_to_int32_reference(reference_t reference,
                                            int32_t value) {
  if (reference.underlying_type != int32_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((int32_t*) reference.pointer) = value;
}

// ----- int16_t -----

static inline reference_t reference_of_int16(int16_t* pointer) {
  reference_t result;
  result.underlying_type = int16_type();
  result.pointer = pointer;
  return result;
}

static inline int16_t dereference_int16(reference_t reference) {
  if (reference.underlying_type != int16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((int16_t*) reference.pointer);
}

static inline void write_to_int16_reference(reference_t reference,
                                            int16_t value) {
  if (reference.underlying_type != int16_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((int16_t*) reference.pointer) = value;
}

// ----- int8_t -----

static inline reference_t reference_of_int8(int8_t* pointer) {
  reference_t result;
  result.underlying_type = int8_type();
  result.pointer = pointer;
  return result;
}

static inline uint64_t dereference_int8(reference_t reference) {
  if (reference.underlying_type != int8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  return *((int8_t*) reference.pointer);
}

static inline void write_to_int8_reference(reference_t reference,
                                           int8_t value) {
  if (reference.underlying_type != int8_type()) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  *((int8_t*) reference.pointer) = value;
}

// -----

static inline byte_array_t*
    byte_array_append_reference(byte_array_t* byte_array,
                                reference_t reference) {
  return reference.underlying_type->append_fn(byte_array, reference);
}

#endif /* _REFERENCE_H_ */
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
extern int string_contains_char(const char* str, const char ch);
extern uint64_t string_to_uint64(const char* str);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern uint64_t string_parse_uint64(const char* string);
extern char* string_duplicate(const char* src);
extern char* string_append(const char* a, const char* b);

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

// Provide a convenient place to set a breakpoint
void armyknife_test_fail_exit() { exit(1); }

#define ARMYKNIFE_TEST_FAIL(msg)                                               \
  do {                                                                         \
    fprintf(stderr, "%s:%d: -- FAIL (fn=%s, msg='%s')\n", __FILE__, __LINE__,  \
            __func__, msg);                                                    \
    armyknife_test_fail_exit();                                                \
  } while (0)

#endif /* _TEST_H_ */

// ======================================================================
// In order to not increase the runtime footprint, we don't actually
// have any non-macros (or inlined functions).
// ======================================================================
#line 2 "tokenizer.c"
/**
 * @file tokenizer.c
 *
 * This contains a routine to split a string into a series of tokens
 * seperated via delimiters.
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "array.h"

extern array_t(char*) * tokenize(const char* str, const char* delimiters);

#endif /* _TOKENIZER_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocate.h"
#include "array.h"
#include "fatal-error.h"
#include "string-util.h"
#include "tokenizer.h"

array_t(char*) * add_duplicate(array_t(char*) * token_array, const char* data);

/**
 * Tokenize a string.
 *
 * Delimiters terminate the current token and are thrown away.
 */

array_t(char*) * tokenize(const char* str, const char* delimiters) {
  array_t(char*)* result = make_array(char_ptr_type(), 4);
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
array_t(char*) * add_duplicate(array_t(char*) * token_array, const char* data) {
  char* duplicate = string_duplicate(data);
  return array_add(token_array, reference_of_char_ptr(&duplicate));
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
#line 2 "tuple.c"
/**
 * @file tuple.c
 *
 * Tuples are a low-level construct which are most useful for
 * implementing containers. The abstraction is like a C structure
 * except that "members" are referenced by position rather than name
 * and that the layout of tuples are determined by the tuple type not
 * the static declaration of the structure.
 */

// ======================================================================
// This section is extraced to tuple.h
// ======================================================================

#ifndef _TUPLE_H_
#define _TUPLE_H_

#include <stdint.h>

// Technically there is no struct that can really represent a tuple
// and the alignment can be as little as one byte. I think this will
// eventually prove to be the wrong thing.
typedef struct {
  __attribute__((aligned(8))) uint8_t data[0];
} tuple_t;

extern type_t* intern_tuple_type(int number_of_parameters, ...);

// Rename all of these since they are in fact painfully long...

extern reference_t tuple_reference_of_element(reference_t tuple,
                                              uint64_t position);

extern reference_t tuple_reference_of_element_from_pointer(
    type_t* type, tuple_t* tuple_pointer, uint64_t position);

extern void tuple_write_element(reference_t tuple_ref, uint64_t position,
                                reference_t value);

extern struct byte_array_S* tuple_append_text(struct byte_array_S* byte_array,
                                              reference_t tuple_ref);

#endif /* _TUPLE_H_ */

// ======================================================================

#include <stdarg.h>
#include <stdlib.h>

#define TUPLE_ALIGN_OFFSET(offset, alignment)                                  \
  ((offset + (alignment - 1)) & ~(alignment - 1))

/**
 * Make a tuple type.
 */
type_t* intern_tuple_type(int number_of_parameters, ...) {
  type_t* result = (malloc_struct(type_t));

  byte_array_t* name = make_byte_array(32);
  name = byte_array_append_string(name, "tuple(");

  int offset = 0;
  int alignment = 1;

  va_list args;
  va_start(args, number_of_parameters);
  for (int i = 0; (i < number_of_parameters); i++) {

    type_t* element_type = va_arg(args, type_t*);
    result->parameters[result->number_of_parameters++] = element_type;
    offset = TUPLE_ALIGN_OFFSET(offset, element_type->alignment);
    if (element_type->size <= 0) {
      fatal_error(ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER);
    }
    if (element_type->alignment > alignment) {
      alignment = element_type->alignment;
    }
    if (i > 0) {
      name = byte_array_append_string(name, ",");
    }
    name = byte_array_append_string(name, element_type->name);
    offset += element_type->size;
  }
  va_end(args);

  // TODO(jawilson): make sure alignment is a power of two.

  result->size = offset;
  result->alignment = alignment;
  name = byte_array_append_string(name, ")");
  result->name = byte_array_c_substring(name, 0, byte_array_length(name));
  free(name);

  result->append_fn = &tuple_append_text;

  // TODO(jawilson): compare_fn, append_fn, hash_fn
  // TODO(jawilson): actually intern the type!

  return intern_type(*result);
}

/**
 * Get a reference to a "member" element (from a reference to a tuple).
 */
reference_t tuple_reference_of_element(reference_t tuple_ref,
                                       uint64_t position) {
  // Make sure the reference is to a tuple?
  type_t* type = tuple_ref.underlying_type;
  tuple_t* tuple_pointer = tuple_ref.pointer;

  uint64_t offset = 0;
  for (int i = 0; (i < type->number_of_parameters); i++) {
    type_t* element_type = type->parameters[i];
    offset = TUPLE_ALIGN_OFFSET(offset, element_type->alignment);
    if (i == position) {
      return reference_of(element_type, &tuple_pointer->data[offset]);
    }
    offset += element_type->size;
  }
  fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
}

void tuple_write_element(reference_t tuple_ref, uint64_t position,
                         reference_t value) {
  reference_t element_reference
      = tuple_reference_of_element(tuple_ref, position);
  if (element_reference.underlying_type != value.underlying_type) {
    fatal_error(ERROR_REFERENCE_NOT_EXPECTED_TYPE);
  }
  /*
  TRACE();
  fprintf(stderr, "Writing size=%d bytes to address %ul\n",
          value.underlying_type->size,
          element_reference.pointer);
  */
  memcpy(element_reference.pointer, value.pointer, value.underlying_type->size);
}

struct byte_array_S* tuple_append_text(struct byte_array_S* byte_array,
                                       reference_t tuple_ref) {
  // Make sure the reference is to a tuple?
  type_t* type = tuple_ref.underlying_type;
  tuple_t* tuple_pointer = tuple_ref.pointer;

  byte_array = byte_array_append_string(byte_array, "tuple(");
  for (int i = 0; (i < tuple_ref.underlying_type->number_of_parameters); i++) {
    if (i > 0) {
      byte_array = byte_array_append_string(byte_array, ", ");
    }
    type_t* element_type = type->parameters[i];
    reference_t element_ref = tuple_reference_of_element(tuple_ref, i);
    byte_array = element_type->append_fn(byte_array, element_ref);
  }
  byte_array = byte_array_append_string(byte_array, ")");
  return byte_array;
}
#line 2 "type.c"
/**
 * @file type.c
 *
 * This is a runtime type so that containers can do dynamic type
 * checking since the C compiler isn't capable of type checking
 * automatically.
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_TYPE_PARAMETERS 8

struct reference_S;
struct type_S;

typedef int (*compare_references_fn_t)(struct reference_S a,
                                       struct reference_S b);

typedef uint64_t (*hash_reference_fn_t)(struct reference_S object);

typedef byte_array_t* (*append_text_representation_fn_t)(
    byte_array_t* byte_array, struct reference_S object);

struct type_S {
  char* name;
  int size;
  int alignment;
  uint64_t number_of_parameters;
  struct type_S* parameters[MAX_TYPE_PARAMETERS];
  compare_references_fn_t compare_fn;
  hash_reference_fn_t hash_fn;
  append_text_representation_fn_t append_fn;
};
typedef struct type_S type_t;

extern type_t* intern_type(type_t type);

extern type_t char_ptr_type_constant;
extern type_t nil_type_constant;
extern type_t self_ptr_type_constant;

extern type_t uint64_type_constant;
extern type_t uint32_type_constant;
extern type_t uint16_type_constant;
extern type_t uint8_type_constant;

extern type_t int64_type_constant;
extern type_t int32_type_constant;
extern type_t int16_type_constant;
extern type_t int8_type_constant;

static inline type_t* nil_type() { return &nil_type_constant; }
static inline type_t* char_ptr_type() { return &char_ptr_type_constant; }
static inline type_t* self_ptr_type() { return &self_ptr_type_constant; }

static inline type_t* uint64_type() { return &uint64_type_constant; }
static inline type_t* uint32_type() { return &uint32_type_constant; }
static inline type_t* uint16_type() { return &uint16_type_constant; }
static inline type_t* uint8_type() { return &uint8_type_constant; }

static inline type_t* int64_type() { return &int64_type_constant; }
static inline type_t* int32_type() { return &int32_type_constant; }
static inline type_t* int16_type() { return &int16_type_constant; }
static inline type_t* int8_type() { return &int8_type_constant; }

// TODO: global constants for standard types like uint64_t and void*

type_t* intern_type(type_t type) {
  WARN("intern_type is not actually doing interning");
  type_t* result = (type_t*) malloc_copy_of((uint8_t*) &type, sizeof(type));
  return result;
}

#define pointer_t(t) t*

#endif /* _TYPE_H_ */

#include <stdalign.h>

int compare_string_references(reference_t ref_a, reference_t ref_b) {
  return strcmp(dereference_char_ptr(ref_a), dereference_char_ptr(ref_b));
}

int compare_uint64_t(struct reference_S a, struct reference_S b) {
  return dereference_uint64(a) - dereference_uint64(b);
}

// etc.

uint64_t hash_reference_bytes(reference_t reference) {
  // Actually call fasthash64!
  return 12;
}

uint64_t hash_string_reference(reference_t reference) {
  // Hash the underlying string (we won't know it's size like above).
  return 12;
}

byte_array_t* append_string_text(byte_array_t* byte_array,
                                 struct reference_S object) {
  char* str = dereference_char_ptr(object);
  byte_array = byte_array_append_byte(byte_array, '"');
  // TODO(jawilson): quote "
  byte_array = byte_array_append_string(byte_array, str);
  byte_array = byte_array_append_byte(byte_array, '"');
  return byte_array;
}

struct byte_array_S* append_uint64_text(struct byte_array_S* byte_array,
                                        struct reference_S object) {
  char buffer[64];
  uint64_t number = dereference_uint64(object);
  sprintf(buffer, "%lu", number);
  return byte_array_append_string(byte_array, buffer);
}

struct byte_array_S* append_uint32_text(struct byte_array_S* byte_array,
                                        struct reference_S object) {
  char buffer[64];
  uint64_t number = dereference_uint32(object);
  sprintf(buffer, "%lu", number);
  return byte_array_append_string(byte_array, buffer);
}

struct byte_array_S* append_uint16_text(struct byte_array_S* byte_array,
                                        struct reference_S object) {
  char buffer[64];
  uint64_t number = dereference_uint16(object);
  sprintf(buffer, "%lu", number);
  return byte_array_append_string(byte_array, buffer);
}

struct byte_array_S* append_uint8_text(struct byte_array_S* byte_array,
                                       struct reference_S object) {
  char buffer[64];
  uint64_t number = dereference_uint8(object);
  sprintf(buffer, "%lu", number);
  return byte_array_append_string(byte_array, buffer);
}

struct byte_array_S* append_int64_text(struct byte_array_S* byte_array,
                                       struct reference_S object) {
  char buffer[64];
  int64_t number = dereference_int64(object);
  sprintf(buffer, "%ld", number);
  return byte_array_append_string(byte_array, buffer);
}

struct byte_array_S* append_int32_text(struct byte_array_S* byte_array,
                                       struct reference_S object) {
  char buffer[64];
  int32_t number = dereference_int32(object);
  sprintf(buffer, "%d", number);
  return byte_array_append_string(byte_array, buffer);
}

struct byte_array_S* append_int16_text(struct byte_array_S* byte_array,
                                       struct reference_S object) {
  char buffer[64];
  int16_t number = dereference_int16(object);
  sprintf(buffer, "%d", number);
  return byte_array_append_string(byte_array, buffer);
}

struct byte_array_S* append_int8_text(struct byte_array_S* byte_array,
                                      struct reference_S object) {
  char buffer[64];
  int8_t number = dereference_int8(object);
  sprintf(buffer, "%d", number);
  return byte_array_append_string(byte_array, buffer);
}

type_t uint64_type_constant = {
    .name = "uint64_t",
    .size = sizeof(uint64_t),
    .alignment = alignof(uint64_t),
    .hash_fn = &hash_reference_bytes,
    .append_fn = &append_uint64_text,
};

type_t uint32_type_constant = {
    .name = "uint32_t",
    .size = sizeof(uint32_t),
    .alignment = alignof(uint32_t),
    .hash_fn = &hash_reference_bytes,
    .append_fn = &append_uint32_text,
};

type_t uint16_type_constant = {
    .name = "uint16_t",
    .size = sizeof(uint16_t),
    .alignment = alignof(uint16_t),
    .hash_fn = &hash_reference_bytes,
    .append_fn = &append_uint16_text,
};

type_t uint8_type_constant = {
    .name = "uint8_t",
    .size = sizeof(uint8_t),
    .alignment = alignof(uint8_t),
    .hash_fn = &hash_reference_bytes,
    .append_fn = &append_uint8_text,
};

type_t int64_type_constant = {
    .name = "int64_t",
    .size = sizeof(int64_t),
    .alignment = alignof(int64_t),
    .hash_fn = &hash_reference_bytes,
    .append_fn = &append_int64_text,
};

type_t int32_type_constant = {
    .name = "int32_t",
    .size = sizeof(int32_t),
    .alignment = alignof(int32_t),
    .hash_fn = &hash_reference_bytes,
    .append_fn = &append_int32_text,
};

type_t int16_type_constant = {
    .name = "int16_t",
    .size = sizeof(int16_t),
    .alignment = alignof(int16_t),
    .hash_fn = &hash_reference_bytes,
    .append_fn = &append_int16_text,
};

type_t int8_type_constant = {
    .name = "int8_t",
    .size = sizeof(int8_t),
    .alignment = alignof(int8_t),
    .hash_fn = &hash_reference_bytes,
    .append_fn = &append_int8_text,
};

type_t char_type_constant = {
    .name = "char",
    .size = sizeof(char),
    .alignment = alignof(char),
    .hash_fn = &hash_reference_bytes,
};

type_t double_type_constant = {
    .name = "double",
    .size = sizeof(double),
    .alignment = alignof(double),
    .hash_fn = &hash_reference_bytes,
};

type_t float_type_constant = {
    .name = "float",
    .size = sizeof(float),
    .alignment = alignof(float),
    .hash_fn = &hash_reference_bytes,
};

type_t char_ptr_type_constant = {
    .name = "char*",
    .size = sizeof(char*),
    .alignment = alignof(char*),
    .hash_fn = &hash_string_reference,
    .compare_fn = &compare_string_references,
    .append_fn = &append_string_text,
};

type_t nil_type_constant = {
    .name = "nil",
    .size = 0,
    .alignment = 0,
    .hash_fn = &hash_reference_bytes,
};

type_t self_ptr_type_constant = {
    .name = "self*",
    .size = sizeof(uint64_t*),
    .alignment = alignof(uint64_t*),
};

// TODO(jawilson): more pointer types for the built in C types.
#endif /* C_ARMYKNIFE_LIB_IMPL */
