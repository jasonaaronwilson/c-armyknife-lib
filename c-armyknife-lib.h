/*
 * Copyright (c) 2023, Jason A. Wilson
 *
 * The project documentation, source code, tests, and the full open
 * source license is available at:
 *
 * https://github.com/jasonaaronwilson/c-armyknife-lib/
 * 
 * This file itself is automatically generated from about 18 or more
 * source files and is "packaged" into this particular format
 * sometimes known as a "header only" libray (see
 * https://en.wikipedia.org/wiki/Header-only) so we don't recommend
 * editing this file by hand. Other ways of using or obtaining this
 * library will be documented in the c-armyknife-lib repository
 * including instructions for turning off some included functionality
 * to save space.
 *
 * To actually use this library, one ".c" file in your project must
 * include this file, (typically called
 * c-armyknife-lib-<<version-string>>.h) after *first* defining
 * C_ARMYKNIFE_LIB_IMPL otherwise it only provides the prototypes,
 * inlined functions, macros, and type definitions, like you would
 * normally see in a C header file and your program will not link
 * correctly.
 */

// SSCF generated file from: boolean.c

#line 14 "boolean.c"
#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

#include <stdbool.h>

/**
 * @type boolean_t
 *
 * This is a simple typedef for "bool" (or _Bool from C99) which is
 * available from "stdbool.h" as bool and true and false constants are
 * also defined. We use it for more consistency in primitive types
 * (where only char* is commonly used in this library despite not
 * following the typically naming convention).
 */
typedef bool boolean_t;

// #define true ((boolean_t) 1)
// #define false ((boolean_t) 0)

#endif /* _BOOLEAN_H_ */
// SSCF generated file from: fatal-error.c

#line 23 "fatal-error.c"
#ifndef _FATAL_ERROR_H_
#define _FATAL_ERROR_H_

struct fatal_error_config_S {
  boolean_t catch_sigsegv;
};

typedef struct fatal_error_config_S fatal_error_config_t;

/**
 * @constants error_code_t
 */
typedef enum {
  ERROR_UKNOWN,
  ERROR_SIGSEGV,
  ERROR_ACCESS_OUT_OF_BOUNDS,
  ERROR_BAD_COMMAND_LINE,
  ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER,
  ERROR_ILLEGAL_ENUM_VALUE,
  ERROR_ILLEGAL_INITIAL_CAPACITY,
  ERROR_ILLEGAL_NULL_ARGUMENT,
  ERROR_ILLEGAL_ZERO_HASHCODE_VALUE,
  ERROR_ILLEGAL_RANGE,
  ERROR_MEMORY_ALLOCATION,
  ERROR_MEMORY_FREE_NULL,
  ERROR_NOT_REACHED,
  ERROR_REFERENCE_NOT_EXPECTED_TYPE,
  ERROR_UNIMPLEMENTED,
  ERROR_OPEN_LOG_FILE,
  ERROR_TEST,
  ERROR_INTERNAL_ASSERTION_FAILURE,
  ERROR_BAD_ALLOCATION_SIZE,
  ERROR_ILLEGAL_ARGUMENT,
  ERROR_MEMORY_START_PADDING_ERROR,
  ERROR_MEMORY_END_PADDING_ERROR,
  ERROR_FATAL,
  ERROR_ILLEGAL_STATE,
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);
extern void configure_fatal_errors(fatal_error_config_t config);

/**
 * @macro fatal_error
 *
 * Terminates the program with a fatal error.
 */
#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */
// SSCF generated file from: value.c

#line 3 "value.c"
#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>

/**
 * @file value.c
 *
 * A major part of the armyknife library is giving basic "collection"
 * capabilities to C.
 *
 * In an ideal world, C would allow structures to be parameterized so
 * that a field holding a "key" or a "value" could change size (based
 * on its static parameters).
 *
 * Since that capability doesn't currently exist, instead we take an
 * approach where keys and values are always the same size (64
 * bits). While this doesn't allow storing *anything* as a key or
 * value, it does allow storing a *pointer to anything* as a key or
 * value. This is actually very similar to how Java collections work
 * except we can store primitive values like integers and doubles
 * without "boxing".
 *
 * When "searching" a collection for a key, we want to be able to
 * return "not found" (and potentially other "non-fatal" error
 * conditions). For this reason we also have the value_result_t
 * structure, which is a pair of a value_t and a
 * non_fatal_error_code_t. (It's actually slightly more complicated
 * because `tcc` treats anonymous unions a little differently than
 * `gcc` and `clang` so we work around that by repeating the fields of
 * a value to make value_result_t a bit more convenient to work with).
 *
 * Sometimes value_result_t is also used by non collection based
 * functions, such as parsing an integer, so that other non-fatal
 * errors can be communicated back to the caller.
 *
 * The contract with returning a non-fatal errors is that the state of
 * the system is in a good state to continue processing (or they get
 * to decide if the error should be fatal). Users will thus expect
 * that when a non-fatal error is returned that no global state
 * modification has ocurred.
 *
 * value_t's are typically only passed into functions while
 * optional_value_result_t's are typically returned from functions.
 *
 * When a value_result_t is returned you must always check for an
 * error code before using the value component of the result. `is_ok`
 * and `is_not_ok` make this slightly easier and easier to read.
 *
 * value_t's and value_result_t's carry no type information that can
 * be queried at runtime and by their nature C compilers are going to
 * do a very incomplete job of statically type checking these. For
 * example you can easily put a double into a collection and
 * successfully get back this value and use it as a very suspicious
 * pointer and the compiler will not warn you about this. So
 * collections aren't as safe as in other languages at either compile
 * or run-time. (Java's collections (when generic types are *not*
 * used), are not "safe" at compile time but are still dynamically
 * safe.)
 *
 * On the positive side, the lack of dynamic typing means you haven't
 * paid a price to maintain these and in theory your code can run
 * faster.
 *
 * If C had a richer type-system, namely generic types, we could catch
 * all all potential type errors at compile time and potentially even
 * allow storing "values" larger than 64bits "inline" with a little
 * more magic.
 *
 * The most common things to use as keys are strings, integers, and
 * pointers (while common association values are strings, integers,
 * pointers and booleans).
 *
 * Our primary goal is to make collections convenient. Using typedef
 * and inline functions you can also make these safer at compile time.
 */

/**
 * @typedef value_t
 *
 * An un-typed union of integers, doubles, and pointers.
 */
typedef union {
  uint64_t u64;
  uint64_t i64;
  char* str;
  void* ptr;
  void* dbl;
} value_t;

/**
 * @enum non_fatal_error_code_t
 *
 * These are user recoverable errors and when a non-recoverable error
 * is returned, the state of the system should be left in a
 * recoverable state.
 */
typedef enum {
  NF_OK,
  NF_ERROR_NOT_FOUND,
  NF_ERROR_NOT_PARSED_AS_NUMBER,
  NF_ERROR_NOT_PARSED_AS_EXPECTED_ENUM,
} non_fatal_error_code_t;

/**
 * @typedef value_result_t
 *
 * A pair of a value_t and a non-fatal error code. When the error code
 * is set, the value_t should never be looked at (most likely will be
 * "zero" or a "nullptr" but you shouldn't trust that).
 */
typedef struct {
  union {
    uint64_t u64;
    uint64_t i64;
    char* str;
    void* ptr;
    void* dbl;
    value_t val;
  };
  // TODO(jawilson): change the name of the field after checking if
  // the compiler helps at all here!
  non_fatal_error_code_t nf_error;
} value_result_t;

#define boolean_to_value(x) ((value_t){.u64 = x})
#define u64_to_value(x) ((value_t){.u64 = x})
#define i64_to_value(x) ((value_t){.i64 = x})
#define str_to_value(x) ((value_t){.str = x})
#define ptr_to_value(x) ((value_t){.ptr = x})
#define dbl_to_value(x) ((value_t){.dbl = x})

/**
 * @function is_ok
 *
 * Return true if the given value_result_t contains a legal value
 * instead of an error condition.
 */
static inline boolean_t is_ok(value_result_t value) {
  return value.nf_error == NF_OK;
}

/**
 * @function is_not_ok
 *
 * Return true if the given value_result_t contains an error, such as
 * NF_ERROR_NOT_FOUND.
 */
static inline boolean_t is_not_ok(value_result_t value) {
  return value.nf_error != NF_OK;
}

#define cast(type, expr) ((type) (expr))

#endif /* _VALUE_H_ */
// SSCF generated file from: allocate.c

#line 29 "allocate.c"
#ifndef _ALLOCATE_H_
#define _ALLOCATE_H_

#include <stdint.h>

extern uint8_t* checked_malloc(char* file, int line, uint64_t amount);
extern uint8_t* checked_malloc_copy_of(char* file, int line, uint8_t* source,
                                       uint64_t amount);
extern void checked_free(char* file, int line, void* pointer);

extern void check_memory_hashtable_padding();

/**
 * @macro malloc_bytes
 *
 * This is essentially the same as malloc but the memory is always
 * zeroed before return it to the user. We use a macro here to call
 * checked_malloc so that the file and line number can be passed.
 */
#define malloc_bytes(amount) (checked_malloc(__FILE__, __LINE__, amount))

/**
 * @macro free_bytes
 *
 * This is essentially the same as free.
 */
#define free_bytes(ptr) (checked_free(__FILE__, __LINE__, ptr))

/**
 * @macro malloc_struct
 *
 * This provides a convenient way to allocate a zero-filled space big
 * enough to hold the given structure with sizeof automatically used
 * and the result automatically casted to a pointer to the given type.
 */
#define malloc_struct(struct_name)                                             \
  ((struct_name*) (checked_malloc(__FILE__, __LINE__, sizeof(struct_name))))

/**
 * @macro malloc_copy_of
 *
 * This provides a convenient way to allocate a copy of a given
 * "source". Generally you would only use it with a pointer to a
 * structure though in theory it could be used on other things.
 *
 * See also: string_duplicate which automatically calls strlen, etc.
 */
#define malloc_copy_of(source, number_of_bytes)                                \
  (checked_malloc_copy_of(__FILE__, __LINE__, source, number_of_bytes))

// TODO(jawilson): malloc_copy_of_struct

#endif /* _ALLOCATE_H_ */
// SSCF generated file from: uint64.c

#line 9 "uint64.c"
#ifndef _UINT64_H_
#define _UINT64_H_

#include <stdint.h>

extern int uint64_highest_bit_set(uint64_t n);

#endif /* _UINT64_H_ */
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
extern char* uint64_to_string(uint64_t number);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern value_result_t string_parse_uint64(const char* string);
extern value_result_t string_parse_uint64_dec(const char* string);
extern value_result_t string_parse_uint64_hex(const char* string);
extern value_result_t string_parse_uint64_bin(const char* string);
extern char* string_duplicate(const char* src);
extern char* string_append(const char* a, const char* b);
extern char* string_left_pad(const char* a, int count, char ch);
__attribute__((format(printf, 1, 2))) extern char* string_printf(char* format,
                                                                 ...);

#endif /* _STRING_UTIL_H_ */
// SSCF generated file from: logger.c

#line 65 "logger.c"
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdarg.h>
#include <stdio.h>

#define LOGGER_OFF 0
#define LOGGER_TRACE 1
#define LOGGER_DEBUG 2
#define LOGGER_INFO 3
#define LOGGER_WARN 4
#define LOGGER_FATAL 5

struct logger_state_S {
  boolean_t initialized;
  int level;
  char* logger_output_filename;
  FILE* output;
};

typedef struct logger_state_S logger_state_t;

#ifndef LOGGER_DEFAULT_LEVEL
#define LOGGER_DEFAULT_LEVEL LOGGER_WARN
#endif /* LOGGER_DEFAULT_LEVEL */

logger_state_t global_logger_state
    = (logger_state_t){.level = LOGGER_DEFAULT_LEVEL};

extern void logger_init(void);

__attribute__((format(printf, 4, 5))) extern void
    logger_impl(char* file, int line_number, int level, char* format, ...);

/**
 * @macro log_none
 *
 * This will never ever log and should have essentially zero impact on
 * compilation (including detecting errors). In other words it should
 * behave like an empty statment ";".
 *
 * On the other hand, errors in these statements will not be caught
 * and therefore it can't be depended on to keep working as you
 * refactor code and decide later that you want to turn it on.
 */
#define log_none(format, ...)                                                  \
  do {                                                                         \
  } while (0);

/**
 * @macro log_off
 *
 * This will never log however the compiler *should* still check to
 * make sure the code is legal and compiles. Any sufficiently smart
 * compiler with some level of optimization turned on should not
 * change it's code generation strategy at all if you leave one of these
 * statements in your source code and you should easily be able to
 * upgrade them to a real level later.
 */
#define log_off(format, ...)                                                   \
  do {                                                                         \
    if (0) {                                                                   \
      logger_impl(__FILE__, __LINE__, LOGGER_TRACE, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

/**
 * @macro log_trace
 *
 * Log at the TRACE level using printf style formatting.
 */
#define log_trace(format, ...)                                                 \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_TRACE) {                           \
      logger_impl(__FILE__, __LINE__, LOGGER_TRACE, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

/**
 * @macro log_debug
 *
 * Log at the DEBUG level using printf style formatting.
 */
#define log_debug(format, ...)                                                 \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_DEBUG) {                           \
      logger_impl(__FILE__, __LINE__, LOGGER_DEBUG, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

/**
 * @macro log_info
 *
 * Log at the INFO level using printf style formatting.
 */
#define log_info(format, ...)                                                  \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_INFO) {                            \
      logger_impl(__FILE__, __LINE__, LOGGER_INFO, format, ##__VA_ARGS__);     \
    }                                                                          \
  } while (0)

/**
 * Determine if logging at the INFO level is enabled.
 */
static inline boolean_t should_log_info() {
  return global_logger_state.level <= LOGGER_INFO;
}

/**
 * @macro log_warn
 *
 * Log at the WARN level using printf style formatting.
 */
#define log_warn(format, ...)                                                  \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_WARN) {                            \
      logger_impl(__FILE__, __LINE__, LOGGER_WARN, format, ##__VA_ARGS__);     \
    }                                                                          \
  } while (0)

/**
 * @macro log_fatal
 *
 * Log at the FATAL level using printf style formatting.
 *
 * Typically this is only done before invoking fatal_error though I
 * don't have a convenient way to enforce this.
 */
#define log_fatal(format, ...)                                                 \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_FATAL) {                           \
      logger_impl(__FILE__, __LINE__, LOGGER_FATAL, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

#endif /* _LOGGER_H_ */
// SSCF generated file from: buffer.c

#line 2 "buffer.c"
#ifndef _BUFFER_H_
#define _BUFFER_H_

/**
 * @file buffer.c
 *
 * A growable array of bytes.
 *
 * Buffers are useful for constructing UTF-8 strings (for example by
 * using buffer_printf) though they can hold any binary data including
 * interior NUL bytes. Buffers automatically grow as data is appended
 * (or inserted into) them reducing large classes of errors.
 */

#include <stdint.h>
#include <string.h>

// struct buffer_range_S {
//
// };

struct buffer_S {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
};

/**
 * @struct buffer_t
 *
 * Buffers are non-thread-safe memory regions that generally "grow"
 * over time. Whenever a buffer grows beyound it's capacity, it is
 * moved to accomodates its new capacity (even if there may be other
 * pointers to it's insides, so don't do that unless you have decided
 * the buffer will never grow again).
 *
 * While buffers may seem scary, used properly from a single thread,
 * they are actually quite predictable.
 */
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

__attribute__((warn_unused_result))
__attribute__((format(printf, 2, 3))) extern buffer_t*
    buffer_printf(buffer_t* buffer, char* format, ...);

#endif /* _BUFFER_H_ */
// SSCF generated file from: value-array.c

#line 15 "value-array.c"
#ifndef _VALUE_ARRAY_H_
#define _VALUE_ARRAY_H_

struct value_array_S {
  uint32_t length;
  uint32_t capacity;
  value_t* elements;
};

/**
 * @typedef value_array_t
 *
 * A growable array of 64bit "values" (so integers, doubles, and
 * pointers).
 */
typedef struct value_array_S value_array_t;

extern value_array_t* make_value_array(uint32_t initial_capacity);
extern value_t value_array_get(value_array_t* array, uint32_t index);
extern void value_array_replace(value_array_t* array, uint32_t index,
                                value_t element);
extern void value_array_add(value_array_t* array, value_t element);
extern void value_array_push(value_array_t* array, value_t element);
extern value_t value_array_pop(value_array_t* array);
extern void value_array_insert_at(value_array_t* array, uint32_t position,
                                  value_t element);
extern value_t value_array_delete_at(value_array_t* array, uint32_t position);

#endif /* _VALUE_ARRAY_H_ */
// SSCF generated file from: string-alist.c

#line 8 "string-alist.c"
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

/**
 * @macro string_alist_foreach
 *
 * Allows iteration over the keys and values in a string association
 * list.
 */
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

#line 14 "string-hashtable.c"
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

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_ht_insert(string_hashtable_t* ht, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_ht_delete(string_hashtable_t* ht, char* key);

extern value_result_t string_ht_find(string_hashtable_t* ht, char* key);

/**
 * @macro string_ht_foreach
 *
 * Allows traversing all elements of a hashtable in an unspecified
 * order.
 */
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

#line 20 "string-tree.c"
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

/**
 * @macro string_tree_foreach
 *
 * Perform an inorder traversal of a string-tree.
 *
 * key_var is created in a new block scope with type char*.
 *
 * value_var is created in a new block scope with type value_t and you
 * will probably want to use something like ".ptr" or ".u64" on the
 * value to obtain the actual value.
 *
 * statements should be a normal C block, aka, something like:
 * ```
 * {
 *   statement1();
 *   statement2();
 * }
 * ```
 *
 * Unforunately it is not possible to use "break" or "continue" with
 * this style of loop (and worse, there will be no compilation error
 * or warning if you accidentally do that...)
 */
#define string_tree_foreach(tree, key_var, value_var, statements)              \
  do {                                                                         \
    int stack_n_elements = 0;                                                  \
    string_tree_t* stack[64];                                                  \
    string_tree_t* current = tree;                                             \
    while (current != NULL || stack_n_elements > 0) {                          \
      while (current != NULL) {                                                \
        stack[stack_n_elements++] = current;                                   \
        current = current->left;                                               \
      }                                                                        \
      current = stack[--stack_n_elements];                                     \
      char* key_var = current->key;                                            \
      value_t value_var = current->value;                                      \
      statements;                                                              \
      current = current->right;                                                \
    }                                                                          \
  } while (0)

#endif /* _STRING_TREE_H_ */
// SSCF generated file from: flag.c

#line 94 "flag.c"
#ifndef _FLAG_H_
#define _FLAG_H_

/**
 * @enum flag_type_t
 */
typedef enum {
  flag_type_none,
  flag_type_boolean,
  // TODO(jawilson): flag_type_switch,
  flag_type_string,
  flag_type_uint64,
  flag_type_int64,
  flag_type_double,
  flag_type_enum,
  flag_type_custom,
} flag_type_t;

struct program_descriptor_S {
  char* name;
  char* description;
  string_tree_t* flags;
  string_tree_t* commands;
  value_array_t** write_back_file_args_ptr;
};
typedef struct program_descriptor_S program_descriptor_t;

struct command_descriptor_S {
  program_descriptor_t* program;
  char* name;
  char* description;
  char* write_back_ptr;
  value_array_t** write_back_file_args_ptr;
  string_tree_t* flags;
};
typedef struct command_descriptor_S command_descriptor_t;

struct flag_descriptor_S {
  char* name;
  char* description;
  flag_type_t flag_type;
  char* help_string;
  void* write_back_ptr;
  int enum_size;
  string_tree_t* enum_values;
  // TODO(jawilson): add custom parser call back (and call back data).
};
typedef struct flag_descriptor_S flag_descriptor_t;

extern void flag_program_name(char* name);
extern void flag_description(char* description);
extern void flag_file_args(value_array_t** write_back_ptr);

extern void flag_boolean(char* name, boolean_t* write_back_ptr);
extern void flag_string(char* name, char** write_back_ptr);
extern void flag_uint64(char* name, uint64_t* write_back_ptr);
extern void flag_int64(char* name, int64_t* write_back_ptr);
extern void flag_double(char* name, double* write_back_ptr);
extern void flag_enum(char* name, int* write_back_ptr);
extern void flag_enum_64(char* name, uint64_t* write_back_ptr);
extern void flag_enum_value(char* name, uint64_t value);
extern void flag_alias(char* alias);

// TODO(jawilson): flag_custom

extern char* flag_parse_command_line(int argc, char** argv);

extern void flag_print_help(FILE* out, char* error);

#endif /* _FLAG_H_ */
// SSCF generated file from: command-line-parser.c

#line 16 "command-line-parser.c"
#ifndef _COMMAND_LINE_PARSER_H_
#define _COMMAND_LINE_PARSER_H_

struct command_line_command_descriptor_S {
  char* name;
  char* help_string;
};

typedef struct command_line_command_descriptor_S
    command_line_command_descriptor_t;

/**
 * @constants command_line_flag_type_t
 */
typedef enum {
  command_line_flag_type_string,
  command_line_flag_type_boolean,
  command_line_flag_type_unsigned,
  command_line_flag_type_signed,
  command_line_flag_type_double,
} command_line_flag_type_t;

struct command_line_flag_descriptor_S {
  char* long_name;
  command_line_flag_type_t arg_type;
  char* help_string;
};

typedef struct command_line_flag_descriptor_S command_line_flag_descriptor_t;

struct command_line_parser_configuation_S {
  char* program_name;
  char* program_description;
  value_array_t* command_descriptors;
  value_array_t* flag_descriptors;
};

typedef struct command_line_parser_configuation_S
    command_line_parser_configuation_t;

struct command_line_parse_result_S {
  char* program;
  char* command;
  string_hashtable_t* flags;
  value_array_t* files;
};

typedef struct command_line_parse_result_S command_line_parse_result_t;

extern command_line_command_descriptor_t*
    make_command_line_command_descriptor(char* name, char* help_string);

extern command_line_flag_descriptor_t* make_command_line_flag_descriptor(
    char* long_name, command_line_flag_type_t arg_type, char* help_string);

extern command_line_parse_result_t
    parse_command_line(int argc, char** argv,
                       command_line_parser_configuation_t* config);

#endif /* _COMMAND_LINE_PARSER_H_ */
// SSCF generated file from: io.c

#line 13 "io.c"
#ifndef _IO_H_
#define _IO_H_

#include <limits.h>
#include <stdint.h>

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name);

extern void buffer_write_file(buffer_t* bytes, char* file_name);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_read_until(buffer_t* buffer, FILE* input, char end_of_line);

int file_peek_byte(FILE* input);

boolean_t file_eof(FILE* input);

void file_copy_stream(FILE* input, FILE* output, boolean_t until_eof,
                      uint64_t size);

#endif /* _IO_H_ */
// SSCF generated file from: tokenizer.c

#line 7 "tokenizer.c"
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

extern value_array_t* string_tokenize(const char* str, const char* delimiters);
extern value_array_t* buffer_tokenize(buffer_t* buffer, const char* delimiters);
extern value_array_t* tokenize_memory_range(uint8_t* start, uint64_t length,
                                            const char* delimiters);

// TODO(jawilson):

#endif /* _TOKENIZER_H_ */
// SSCF generated file from: random.c

#line 16 "random.c"
#ifndef _RANDOM_H_
#define _RANDOM_H_

struct random_state_S {
  uint64_t a;
  uint64_t b;
};

typedef struct random_state_S random_state_t;

extern random_state_t random_state_for_test(void);
extern uint64_t random_next_uint64(random_state_t* state);
extern uint64_t random_next_uint64_below(random_state_t* state,
                                         uint64_t maximum);

#endif /* _RANDOM_H_ */
// SSCF generated file from: test.c

#line 13 "test.c"
#ifndef _TEST_H_
#define _TEST_H_

/**
 * @macro test_fail
 *
 * Immediately fail a test after printing a message (supplied the same
 * way as printf, log_fatal, etc.)
 *
 * This is a macro instead of a function because it automatically
 * passes the current file and line number to test_fail_and_exit which
 * enables uniform and convenient error messagages according to GNU
 * guidelines (so they will look like gcc compiler errors if your IDE
 * understands those).
 *
 * It is *advised* (at least for now) to not pass complicated
 * expressions to `test_fail` if those are likely to fail. Instead run
 * the test in a debugger and set a break-point on
 * `test_fail_and_exit`.
 */
#define test_fail(format, ...)                                                 \
  do {                                                                         \
    test_fail_and_exit(__FILE__, __LINE__, format, ##__VA_ARGS__);             \
  } while (0)


/**
 * @macro test_assert
 *
 * Evaluate the condition and fail the test if the result is false.
 */
#define test_assert(condition)                                                 \
  do {                                                                         \
    if (!(condition))                                                          \
      test_fail("A test assertion failed. Condition expression was: %s",       \
                #condition);                                                   \
  } while (0)

#endif /* _TEST_H_ */
#ifdef C_ARMYKNIFE_LIB_IMPL
#line 2 "allocate.c"

/**
 * @file allocate.c
 *
 * Hook into malloc and free to make them more convenient and possibly
 * safer. For example, allocated memory is always zero'd and macros
 * like malloc_struct are more readable. We also have a novel LRU
 * memory-bounds checker that found a bug that valgrind didn't.
 *
 * For missing calls to free, we are fully compatbile with valgrind
 * (since we just call malloc/free). (Valgrind also has a memcheck
 * mode though it actually masked a bug instead of finding it.)
 *
 * We also have our own memcheck mode. The basic idea is to maintain a
 * LRU style lossy hashtable and to repeatedly scan this set of
 * allocations for overwrites. While this will not detect all
 * instances of an overwrite, when it does, it will be pretty
 * convenient to use in conjuction with a debugger to track the bug
 * down.
 *
 * There should be no run-time penalty when our additional debugging
 * options are turned off (though I still like how valgrind doesn't
 * even require recompilation so maybe if our different techniques
 * pays off, perhaps we can port it into valgrind so please send
 * feedback if you find armyknife-lib's memcheck mode helpful or not.
 */

#ifndef _ALLOCATE_H_
#define _ALLOCATE_H_

#include <stdint.h>

extern uint8_t* checked_malloc(char* file, int line, uint64_t amount);
extern uint8_t* checked_malloc_copy_of(char* file, int line, uint8_t* source,
                                       uint64_t amount);
extern void checked_free(char* file, int line, void* pointer);

extern void check_memory_hashtable_padding();

/**
 * @macro malloc_bytes
 *
 * This is essentially the same as malloc but the memory is always
 * zeroed before return it to the user. We use a macro here to call
 * checked_malloc so that the file and line number can be passed.
 */
#define malloc_bytes(amount) (checked_malloc(__FILE__, __LINE__, amount))

/**
 * @macro free_bytes
 *
 * This is essentially the same as free.
 */
#define free_bytes(ptr) (checked_free(__FILE__, __LINE__, ptr))

/**
 * @macro malloc_struct
 *
 * This provides a convenient way to allocate a zero-filled space big
 * enough to hold the given structure with sizeof automatically used
 * and the result automatically casted to a pointer to the given type.
 */
#define malloc_struct(struct_name)                                             \
  ((struct_name*) (checked_malloc(__FILE__, __LINE__, sizeof(struct_name))))

/**
 * @macro malloc_copy_of
 *
 * This provides a convenient way to allocate a copy of a given
 * "source". Generally you would only use it with a pointer to a
 * structure though in theory it could be used on other things.
 *
 * See also: string_duplicate which automatically calls strlen, etc.
 */
#define malloc_copy_of(source, number_of_bytes)                                \
  (checked_malloc_copy_of(__FILE__, __LINE__, source, number_of_bytes))

// TODO(jawilson): malloc_copy_of_struct

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

static inline boolean_t should_log_memory_allocation() {
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
 * @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING
 *
 * The amount of padding to place in front of each allocation. We have
 * several ways to then check that no one overwrites this padding
 * though we won't catch every case.
 *
 * This should be a multiple of 8 or else the expected alignment
 * (which malloc doesn't make that explicit...) will be broken.
 */
#ifndef ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING
#define ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING 8
#endif

/**
 * @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING
 *
 * The amount of padding to place at the back of each allocation. We
 * have several ways to then check that no one overwrites this padding
 * though we won't catch every case.
 *
 * Unlike start padding, this does not effect alignment and so values
 * as small as 1 make perfect sense though I still recommend 4 or 8
 * bytes especially on 64bit big-endian architectures.
 */
#ifndef ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING
#define ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING 8
#endif

/**
 * @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE
 *
 * This determine how big the lossy hashtable is. On every allocation
 * or deallocation the *entire* lossy hashtable is scanned to see if
 * the padding bytes have been perturbed which makes it possible to
 * find some memory overwrite errors earlier than waiting for the free
 * call (or potentially even if the memory isn't ever freed).
 *
 * It makes no sense to set this unless either
 * ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING or
 * ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING is non-zero.
 */
#ifndef ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE
#define ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE 16
#endif

/**
 * @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT
 *
 * This is just a fail-safe to catch extremely dumb allocation amounts
 * (at least as of 2023). If you know you will have a fixed size
 * amount of memory, you could set this lower and potentially get a
 * slightly nicer error message.
 */
#ifndef ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT
#define ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT (1L << 48)
#endif

#define START_PADDING_BYTE (170 & 0xff)
#define END_PADDING_BYTE ((~START_PADDING_BYTE) & 0xff)

struct memory_hashtable_bucket_S {
  // malloc will never allocated at address 0 so if this field is
  // zero, then this spot in the hashtable is occupied.
  uint64_t malloc_address;
  uint64_t malloc_size;
  char* allocation_filename;
  uint64_t allocation_line_number;
};

typedef struct memory_hashtable_bucket_S memory_hashtable_bucket_t;

memory_hashtable_bucket_t memory_ht[ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE];

void check_start_padding(uint8_t* address) {
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING; i++) {
    if (address[i] != START_PADDING_BYTE) {
      fatal_error(ERROR_MEMORY_START_PADDING_ERROR);
    }
  }
}

void check_end_padding(uint8_t* address, char* filename, uint64_t line) {
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING; i++) {
    if (address[i] != END_PADDING_BYTE) {
      fprintf(stderr,
              "FATAL: someone clobbered past an allocation %lu. (allocated "
              "here: %s:%lu)\n",
              ((uint64_t) address), filename, line);
      fatal_error(ERROR_MEMORY_END_PADDING_ERROR);
    }
  }
}

void check_memory_hashtable_padding() {
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE; i++) {
    if (memory_ht[i].malloc_address != 0) {
      uint64_t malloc_start_address = memory_ht[i].malloc_address;
      uint64_t malloc_size = memory_ht[i].malloc_size;
      check_start_padding((uint8_t*) malloc_start_address);
      check_end_padding((uint8_t*) (malloc_start_address
                                    + ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING
                                    + malloc_size),
                        memory_ht[i].allocation_filename,
                        memory_ht[i].allocation_line_number);
    }
  }
}

// I got this from a blog post by Daniel Lemire (who was actually
// pushing a different scheme...) A terrible hash function will sink
// our scheme but anything that isn't terrible just gets us closer to
// some ideal.
uint64_t mumurhash64_mix(uint64_t h) {
  h *= h >> 33;
  h *= 0xff51afd7ed558ccdL;
  h *= h >> 33;
  h *= 0xc4ceb9fe1a85ec53L;
  h *= h >> 33;
  return h;
}

// Start tracking padding for a given allocated address. This includes
// setting the padding to particular values and of course putting the
// address into the tracking table.
void track_padding(char* file, int line, uint8_t* address, uint64_t amount) {
  // First set the padding to predicatable values
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING; i++) {
    address[i] = START_PADDING_BYTE;
  }
  uint8_t* end_padding_address
      = address + amount + ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING;
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING; i++) {
    end_padding_address[i] = END_PADDING_BYTE;
  }

  if (ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE > 0) {
    // Now replace whatever entry we might already have there. This is
    // why we have more LRU semantics. We could use another signal to
    // probalistically delay updating the hashtable when the bucket is
    // already occupied but I think LRU might work well most of the
    // time. (Mostly a hunch I will admit.).
    int bucket = mumurhash64_mix((uint64_t) address)
                 % ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE;
    memory_ht[bucket].malloc_address = (uint64_t) address;
    memory_ht[bucket].malloc_size = amount;
    memory_ht[bucket].allocation_filename = file;
    memory_ht[bucket].allocation_line_number = line;
  }
}

void untrack_padding(uint8_t* malloc_address) {
  check_start_padding(malloc_address);
  // Unfortunately, since we don't know the size of the allocation, we
  // can't actually check the end padding! When there is enough start
  // padding (say at least 64bits), then we could potentially store
  // say 48bits worth of an allocation amount in it.
  //
  // On the other hand, we do check the end padding if it is still
  // tracked in the lossy memory hashtable.

  if (ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE > 0) {
    // Now finally zero-out the memory hashtable.
    int bucket = mumurhash64_mix((uint64_t) malloc_address)
                 % ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE;
    memory_ht[bucket].malloc_address = 0;
    memory_ht[bucket].malloc_size = 0;
    memory_ht[bucket].allocation_filename = 0;
    memory_ht[bucket].allocation_line_number = 0;
  }
}

/**
 * @function checked_malloc
 *
 * Allocate the given amount bytes or cause a fatal error. The memory
 * is also zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
uint8_t* checked_malloc(char* file, int line, uint64_t amount) {

  if (amount == 0 || amount > ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT) {
    fatal_error(ERROR_BAD_ALLOCATION_SIZE);
  }

  if (should_log_memory_allocation()) {
    fprintf(stderr,
            "ALLOCATE %s:%d -- n_bytes=%lu already_allocated=%lu n_calls=%lu\n",
            file, line, amount, number_of_bytes_allocated,
            number_of_malloc_calls);
  }

  check_memory_hashtable_padding();

  uint64_t amount_with_padding = ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING
                                 + amount
                                 + ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING;
  uint8_t* result = malloc(amount_with_padding);
  if (result == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_ALLOCATION);
  }

  if (should_log_memory_allocation()) {
    fprintf(stderr, "ALLOCATE %s:%d -- %lu -- ptr=%lu\n", file, line, amount,
            (unsigned long) result);
  }

  memset(result, 0, amount_with_padding);
  track_padding(file, line, result, amount);

  number_of_bytes_allocated += amount;
  number_of_malloc_calls++;

  return result + ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING;
}

/**
 * @function checked_malloc_copy_of
 *
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
 * @function checked_free
 *
 * Allocate amount bytes or cause a fatal error. The memory is also
 * zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
void checked_free(char* file, int line, void* pointer) {
  if (should_log_memory_allocation()) {
    fprintf(stderr, "DEALLOCATE %s:%d -- %lu\n", file, line,
            (uint64_t) pointer);
  }
  if (pointer == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_FREE_NULL);
  }

  // Check all of the padding we know about
  check_memory_hashtable_padding();

  uint8_t* malloc_pointer
      = ((uint8_t*) pointer) - ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING;

  // Check this entries padding (in case it got lossed from the global
  // hashtable), and also remove it from the hashtable if it was
  // found.
  untrack_padding(malloc_pointer);
  number_of_free_calls++;
  free(malloc_pointer);
}
#line 2 "boolean.c"
/**
 * @file boolean.c
 *
 * Make sure that at least true/false from <stdbool.h> are available
 * and and a new typedef named boolean_t because bool seems ugly. (Use
 * what you prefer!)
 */

// ======================================================================
// This section is extraced to boolean.h
// ======================================================================

#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

#include <stdbool.h>

/**
 * @type boolean_t
 *
 * This is a simple typedef for "bool" (or _Bool from C99) which is
 * available from "stdbool.h" as bool and true and false constants are
 * also defined. We use it for more consistency in primitive types
 * (where only char* is commonly used in this library despite not
 * following the typically naming convention).
 */
typedef bool boolean_t;

// #define true ((boolean_t) 1)
// #define false ((boolean_t) 0)

#endif /* _BOOLEAN_H_ */
#line 2 "buffer.c"
#ifndef _BUFFER_H_
#define _BUFFER_H_

/**
 * @file buffer.c
 *
 * A growable array of bytes.
 *
 * Buffers are useful for constructing UTF-8 strings (for example by
 * using buffer_printf) though they can hold any binary data including
 * interior NUL bytes. Buffers automatically grow as data is appended
 * (or inserted into) them reducing large classes of errors.
 */

#include <stdint.h>
#include <string.h>

// struct buffer_range_S {
//
// };

struct buffer_S {
  uint32_t length;
  uint32_t capacity;
  uint8_t elements[0];
};

/**
 * @struct buffer_t
 *
 * Buffers are non-thread-safe memory regions that generally "grow"
 * over time. Whenever a buffer grows beyound it's capacity, it is
 * moved to accomodates its new capacity (even if there may be other
 * pointers to it's insides, so don't do that unless you have decided
 * the buffer will never grow again).
 *
 * While buffers may seem scary, used properly from a single thread,
 * they are actually quite predictable.
 */
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

__attribute__((warn_unused_result))
__attribute__((format(printf, 2, 3))) extern buffer_t*
    buffer_printf(buffer_t* buffer, char* format, ...);

#endif /* _BUFFER_H_ */

// ======================================================================

#include <stdlib.h>

/**
 * @function make_buffer
 *
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
 * @function buffer_length
 *
 * Return the number of bytes that have been added to this byte array.
 */
uint64_t buffer_length(buffer_t* array) { return array->length; }

/**
 * @function buffer_get
 *
 * Get a single byte from a byte array.
 */
uint8_t buffer_get(buffer_t* buffer, uint64_t position) {
  if (position < buffer->length) {
    return buffer->elements[position];
  } else {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
#ifdef __TINYC__
    /* gcc and clang know fatal_error is _Noreturn but tcc doesn't */
    return 0;
#endif
  }
}

/**
 * @function buffer_c_substring
 *
 * Extract a newly allocated string that contains the bytes from start
 * to end (appending a zero byte to make sure it's a legal C string).
 */
char* buffer_c_substring(buffer_t* buffer, uint64_t start, uint64_t end) {
  if (buffer == NULL) {
    fatal_error(ERROR_ILLEGAL_NULL_ARGUMENT);
  }

  if (start > end) {
    fatal_error(ERROR_ILLEGAL_RANGE);
  }

  uint64_t copy_length = (end - start);
  char* result = (char*) (malloc_bytes(copy_length + 1));
  if (copy_length > 0) {
    memcpy(result, &buffer->elements[start], copy_length);
  }
  result[copy_length] = '\0';
  return result;
}

/**
 * @function buffer_to_c_string
 *
 * Extract a newly allocated string that contain all of the bytes in the byte
 * buffer as a NUL (zero byte) terminated C string.
 */
char* buffer_to_c_string(buffer_t* buffer) {
  return buffer_c_substring(buffer, 0, buffer->length);
}

/**
 * @function buffer_append_byte
 *
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
 * @function buffer_append_bytes
 *
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
 * @function buffer_append_string
 *
 * Append all of the bytes from a C string (except the ending NUL
 * char).
 */
__attribute__((warn_unused_result)) buffer_t*
    buffer_append_string(buffer_t* buffer, const char* str) {
  return buffer_append_bytes(buffer, (uint8_t*) str, strlen(str));
}

/**
 * @function buffer_increase_capacity
 *
 * As an optimization, the capacity of a buffer can be increased.
 */
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

#ifndef BUFFER_PRINTF_INITIAL_BUFFER_SIZE
#define BUFFER_PRINTF_INITIAL_BUFFER_SIZE 1024
#endif

/**
 * @function buffer_printf
 *
 * Format like printf but append the result to the passed in buffer
 * (returning a new buffer in case the buffer exceeded it's capacity).
 */
__attribute__((warn_unused_result)) __attribute__((format(printf, 2, 3)))
buffer_t*
    buffer_printf(buffer_t* buffer, char* format, ...) {
  char cbuffer[BUFFER_PRINTF_INITIAL_BUFFER_SIZE];
  int n_bytes = 0;
  do {
    va_list args;
    va_start(args, format);
    n_bytes
        = vsnprintf(cbuffer, BUFFER_PRINTF_INITIAL_BUFFER_SIZE, format, args);
    va_end(args);
  } while (0);

  if (n_bytes < BUFFER_PRINTF_INITIAL_BUFFER_SIZE) {
    return buffer_append_string(buffer, cbuffer);
  } else {
    // Be lazy for now and just copy the code from string_printf for
    // this case but we should be able to do ensure capacity and just
    // put the bytes directly at the end of the buffer...
    char* result = (char*) malloc_bytes(n_bytes + 1);
    va_list args;
    va_start(args, format);
    int n_bytes_second = vsnprintf(result, n_bytes + 1, format, args);
    va_end(args);
    if (n_bytes_second != n_bytes) {
      fatal_error(ERROR_INTERNAL_ASSERTION_FAILURE);
    }
    buffer = buffer_append_string(buffer, result);
    free_bytes(result);
    return buffer;
  }
}
#line 2 "command-line-parser.c"
/**
 * @file command-line-parser.c
 *
 * WARNING: this is deperecated. Use flag_* functions instead which
 * provide a more declarative interface to the same functionality
 * which is more pleasant and extensible.
 *
 * A simple command line parser for GNU long style flags and file
 * arguments.
 *
 * Takes a command line and returns an array of "file" arguments and a
 * string_hashtable of command line flags.
 */

#ifndef _COMMAND_LINE_PARSER_H_
#define _COMMAND_LINE_PARSER_H_

struct command_line_command_descriptor_S {
  char* name;
  char* help_string;
};

typedef struct command_line_command_descriptor_S
    command_line_command_descriptor_t;

/**
 * @constants command_line_flag_type_t
 */
typedef enum {
  command_line_flag_type_string,
  command_line_flag_type_boolean,
  command_line_flag_type_unsigned,
  command_line_flag_type_signed,
  command_line_flag_type_double,
} command_line_flag_type_t;

struct command_line_flag_descriptor_S {
  char* long_name;
  command_line_flag_type_t arg_type;
  char* help_string;
};

typedef struct command_line_flag_descriptor_S command_line_flag_descriptor_t;

struct command_line_parser_configuation_S {
  char* program_name;
  char* program_description;
  value_array_t* command_descriptors;
  value_array_t* flag_descriptors;
};

typedef struct command_line_parser_configuation_S
    command_line_parser_configuation_t;

struct command_line_parse_result_S {
  char* program;
  char* command;
  string_hashtable_t* flags;
  value_array_t* files;
};

typedef struct command_line_parse_result_S command_line_parse_result_t;

extern command_line_command_descriptor_t*
    make_command_line_command_descriptor(char* name, char* help_string);

extern command_line_flag_descriptor_t* make_command_line_flag_descriptor(
    char* long_name, command_line_flag_type_t arg_type, char* help_string);

extern command_line_parse_result_t
    parse_command_line(int argc, char** argv,
                       command_line_parser_configuation_t* config);

#endif /* _COMMAND_LINE_PARSER_H_ */

/**
 * @function make_command_line_command_descriptor
 *
 * Allocate a command_line_command_descriptor_t and fill in it's most
 * common fields.
 */
command_line_command_descriptor_t*
    make_command_line_command_descriptor(char* name, char* help_string) {
  command_line_command_descriptor_t* result
      = malloc_struct(command_line_command_descriptor_t);
  result->name = name;
  result->help_string = help_string;
  return result;
}

/**
 * @function make_command_line_flag_descriptor
 *
 * Allocate a command_line_flag_descriptor_t and fill in it's most
 * common fields.
 */
command_line_flag_descriptor_t* make_command_line_flag_descriptor(
    char* long_name, command_line_flag_type_t arg_type, char* help_string) {
  command_line_flag_descriptor_t* result
      = malloc_struct(command_line_flag_descriptor_t);
  result->long_name = long_name;
  result->arg_type = arg_type;
  result->help_string = help_string;
  return result;
}

_Noreturn void show_usage(command_line_parser_configuation_t* config,
                          char* message) {
  if (message) {
    fprintf(stdout, "%s\n\n", message);
  }
  if (config->command_descriptors) {
    fprintf(stdout, "Usage: %s <command> <flags*> <files*>\n",
            config->program_name);
  } else {
    fprintf(stdout, "Usage: %s <flags*> <files*>\n", config->program_name);
  }
  if (config->program_description) {
    fprintf(stdout, "\n%s\n", config->program_description);
  }
  if (config->command_descriptors) {
    fprintf(stdout, "\nCommands:\n");
    for (int i = 0; i < config->command_descriptors->length; i++) {
      command_line_command_descriptor_t* command_descriptor
          = value_array_get(config->command_descriptors, i).ptr;
      fprintf(stdout, "    %s [[%s]]\n", command_descriptor->name,
              command_descriptor->help_string);
    }
  }
  if (config->flag_descriptors) {
    fprintf(stdout, "\nFlags:\n");
    for (int i = 0; i < config->flag_descriptors->length; i++) {
      command_line_flag_descriptor_t* flag
          = value_array_get(config->flag_descriptors, i).ptr;
      fprintf(stdout, "    --%s=<flag-value> (%s)\n", flag->long_name,
              flag->help_string);
    }
  }
  fatal_error(ERROR_BAD_COMMAND_LINE);
}

/**
 * @function parse_command_line
 *
 * Given a command line such as:
 *
 * --count=10 --type=bar --no-arg file1.c file2.c
 *
 * Returns a map containing 3 keys and an array containing two values
 * (file1.c and file2.c)
 *
 * The map: "count" -> "10", "type" -> "bar", "no-arg" -> ""
 * The array: "file1.c" "file2.c"
 */
command_line_parse_result_t
    parse_command_line(int argc, char** argv,
                       command_line_parser_configuation_t* config) {

  value_array_t* command_descriptors = config->command_descriptors;
  value_array_t* flag_descriptors = config->flag_descriptors;


  value_array_t* files = make_value_array(argc);
  string_hashtable_t* flags = make_string_hashtable(32);

  boolean_t parse_flags = true;
  for (int i = (command_descriptors != NULL ? 2 : 1); i < argc; i++) {
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

        if (flag_descriptors != NULL) {
          boolean_t found = false;
          for (int j = 0; j < flag_descriptors->length; j++) {
            command_line_flag_descriptor_t* cl_flag
                = value_array_get(flag_descriptors, j).ptr;
            if (string_equal(cl_flag->long_name, key)) {
              found = true;
              break;
            }
          }
          if (!found) {
            // show_usage(string_printf("Unrecognized flag: %s\n", arg));
            show_usage(config, "Unrecognized flag");
          }
        }

        flags = string_ht_insert(flags, key, str_to_value(value));

        continue;
      }
    }

    value_array_add(files, str_to_value(arg));
  }

  char* command = NULL;
  if (command_descriptors != NULL) {
    if (argc < 2) {
      show_usage(config, "Must supply a command");
    }

    command = argv[1];

    if (command && string_starts_with(command, "-")) {
      show_usage(config, "Flags should not appear before a command.");
      fatal_error(ERROR_BAD_COMMAND_LINE);
    }

    boolean_t found_command = false;
    for (int i = 0; i < command_descriptors->length; i++) {
      command_line_command_descriptor_t* command_descriptor
          = value_array_get(command_descriptors, i).ptr;
      if (string_equal(command, command_descriptor->name)) {
        found_command = true;
        break;
      }
    }
    if (!found_command) {
      show_usage(config, "Unrecognized command");
    }
  }

  return (command_line_parse_result_t){
      .program = argv[0],
      .command = command,
      .flags = flags,
      .files = files,
  };
}
#line 2 "flag.c"
/**
 * @file flag.c
 *
 * A command line parser for flags (and the uninterpreted "file"
 * arguments, aka "left-over" arguments). We use a pragmatic
 * declarative configuration and unless you need to localize the
 * result, we also automatically generate the "usage" / "help"
 * documentation (via flag_print_help).
 *
 * Here is maybe the smallest example you might possibly use:
 *
 * ```
 *   // For simple tools, just use "global" variaables but you can place
 *   // stuff wherever you choose, for example in a struct you can pass
 *   // around.
 *
 *   boolean_t FLAG_verbose = true; // default value is true instead of
 *                                  // false in this case
 *   array_t* FLAG_file_args = NULL;
 *
 *   flag_program_name("program-name");
 *   flag_boolean("--verbose", &FLAG_verbose);
 *   flag_file_args(&FLAG_files);
 *
 *   char* error = flag_parse_command_line(argc, argv);
 *   if (error) {
 *     flag_print_help(stderr, error);
 *     exit(1);
 *   }
 * ```
 *
 * To make the auto generated help be the most useful, a human
 * readable description can be added to the last mentioned "thing" via
 * flag_description().
 *
 * To make your program "easier" to use, you can also define aliases
 * for flags and ("sub-commands" explained below).
 *
 * Another capability is using "sub-commands" so that your tool can be
 * a little more like "git", "apt", "yum" or "docker") versus more
 * traditional command line tools. We are a little more specific about
 * where the sub-command should be located in the command (it *must*
 * currently be in the first position). You can easily work around my
 * opinionated stance in several ways for example by "bubbling up"
 * anything that looks like one of your commands to the beginning of
 * the argument array passed to flag_parse_command_line. The
 * sub-command functionality is automatically enabled once you define
 * *any* sub-command. It's possible to create flags that only apply
 * when the correct sub-command is used.
 *
 * Obviously flag_command() (just like flag_<type> can be called)
 * multiple times to define multiple "sub commands" using different
 * command names.
 *
 * Aliases for flags and commands are also supported.
 *
 * Besides the obvious basic types like integers and strings, it's
 * possible to add flags for enums (really just named integers with a
 * passed in sizeof) and eventually custom value parsers so that lists
 * and maps can be command line arguments. Want to parse a date? Even
 * though I'm not providing it, you can do it and catch errors before
 * something unexpected happens.
 *
 * The interface presented here is *not* thread safe but generally a
 * program will define and parse command line arguments in the initial
 * thread before spawning other threads so this isn't a terrible
 * restriction.
 *
 * Currently, flags and "left-over" arguments are not allowed to be
 * freely mixed. Once something that doesn't start with a "-" is seen,
 * all remaining arguments are treated as "left-over" arguments. The
 * special flag "--" can be used by the user to seperate things that
 * might otherwise be misinterpreted.
 *
 * Note that when an error is returned, some of the "write backs" may
 * have been partially applied changing the default value they may
 * already contain. Unless you are running a test, the best option
 * will be to show help and exit to prevent continuing execution from
 * an undefined state.
 *
 * I think adding a "switch" type may make sense since we currently
 * don't support flags like "-abCdefG" where each each letter is a
 * different flag alias nor do we support "--no-" long format which is
 * sometimes used to "negate" a switch.
 *
 * TODO(jawilson): strict mode and custom parsers.
 * TODO(jawilson): allow stdin, stdout, and stderr deescriptions.
 * TODO(jawilson): allow limits on numeric arguments.
 * TODO(jawilson): allow "switches" which are like boolean flags but
 * don't require an argument. --bubble-gum, --no-bubble-gum.
 */

#ifndef _FLAG_H_
#define _FLAG_H_

/**
 * @enum flag_type_t
 */
typedef enum {
  flag_type_none,
  flag_type_boolean,
  // TODO(jawilson): flag_type_switch,
  flag_type_string,
  flag_type_uint64,
  flag_type_int64,
  flag_type_double,
  flag_type_enum,
  flag_type_custom,
} flag_type_t;

struct program_descriptor_S {
  char* name;
  char* description;
  string_tree_t* flags;
  string_tree_t* commands;
  value_array_t** write_back_file_args_ptr;
};
typedef struct program_descriptor_S program_descriptor_t;

struct command_descriptor_S {
  program_descriptor_t* program;
  char* name;
  char* description;
  char* write_back_ptr;
  value_array_t** write_back_file_args_ptr;
  string_tree_t* flags;
};
typedef struct command_descriptor_S command_descriptor_t;

struct flag_descriptor_S {
  char* name;
  char* description;
  flag_type_t flag_type;
  char* help_string;
  void* write_back_ptr;
  int enum_size;
  string_tree_t* enum_values;
  // TODO(jawilson): add custom parser call back (and call back data).
};
typedef struct flag_descriptor_S flag_descriptor_t;

extern void flag_program_name(char* name);
extern void flag_description(char* description);
extern void flag_file_args(value_array_t** write_back_ptr);

extern void flag_boolean(char* name, boolean_t* write_back_ptr);
extern void flag_string(char* name, char** write_back_ptr);
extern void flag_uint64(char* name, uint64_t* write_back_ptr);
extern void flag_int64(char* name, int64_t* write_back_ptr);
extern void flag_double(char* name, double* write_back_ptr);
extern void flag_enum(char* name, int* write_back_ptr);
extern void flag_enum_64(char* name, uint64_t* write_back_ptr);
extern void flag_enum_value(char* name, uint64_t value);
extern void flag_alias(char* alias);

// TODO(jawilson): flag_custom

extern char* flag_parse_command_line(int argc, char** argv);

extern void flag_print_help(FILE* out, char* error);

#endif /* _FLAG_H_ */

// Non exported data structures

struct flag_key_value_S {
  char* key;
  char* value;
};
typedef struct flag_key_value_S flag_key_value_t;

// Non exported function prototypes

command_descriptor_t* flag_find_command_descriptor(char* name);

flag_descriptor_t* flag_find_flag_descriptor(command_descriptor_t* command,
                                             char* name);

flag_key_value_t flag_split_argument(char* arg);

char* parse_and_write_value(flag_descriptor_t* flag,
                            flag_key_value_t key_value);

char* parse_and_write_boolean(flag_descriptor_t* flag,
                              flag_key_value_t key_value);

char* parse_and_write_uint64(flag_descriptor_t* flag,
                             flag_key_value_t key_value);

char* parse_and_write_enum(flag_descriptor_t* flag, flag_key_value_t key_value);

// Global Variables

program_descriptor_t* current_program;
command_descriptor_t* current_command;
flag_descriptor_t* current_flag;

/**
 * @function flag_program_name
 *
 * Starts the command line configuration builder process and sets the
 * program name to be used in the automatically generated help string.
 */
void flag_program_name(char* name) {
  current_program = malloc_struct(program_descriptor_t);
  current_program->name = name;
  current_command = NULL;
  current_flag = NULL;
}

/**
 * @function flag_description
 *
 * Sets a description for the last "thing" "started".
 */
void flag_description(char* description) {
  if (current_flag != NULL) {
    current_flag->description = description;
  } else if (current_command != NULL) {
    current_command->description = description;
  } else if (current_program != NULL) {
    current_program->description = description;
  } else {
    log_fatal("A current flag, program or command must be executed first");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
}

/**
 * @function flag_file_args
 *
 * Set where to write "left-over" arguments (which are usually file
 * paths, wild-cards, etc.) should be written. Any pointer to the
 * write back array is completely replaced with a new array so you
 * will typically initialize it to just NULL and everything just
 * works.
 *
 * Unless you have a very good reason call this sometime after calling
 * flag_program_name but not before you start processing "children" of
 * the program node.
 */
void flag_file_args(value_array_t** write_back_file_args_ptr) {
  if (current_command != NULL) {
    current_command->write_back_file_args_ptr = write_back_file_args_ptr;
  } else if (current_program != NULL) {
    current_program->write_back_file_args_ptr = write_back_file_args_ptr;
  } else {
    log_fatal("A current program or command must be executed first");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
}

// Place a flag in either the current_command or current_program. The
// name is passed in explicitly to allow aliases.
void add_flag(char* name, void* write_back_ptr, flag_type_t flag_type) {
  current_flag = malloc_struct(flag_descriptor_t);
  current_flag->flag_type = flag_type;
  current_flag->name = name;
  current_flag->write_back_ptr = write_back_ptr;

  // TODO(jawilson): check for a flag with the same name?
  if (current_command != NULL) {
    current_command->flags = string_tree_insert(current_command->flags, name,
                                                ptr_to_value(current_flag));
  } else if (current_program != NULL) {
    current_program->flags = string_tree_insert(current_program->flags, name,
                                                ptr_to_value(current_flag));
  } else {
    log_fatal("A current program or command must be executed first");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
}

void flag_boolean(char* name, boolean_t* write_back_ptr) {
  add_flag(name, write_back_ptr, flag_type_boolean);
}

void flag_string(char* name, char** write_back_ptr) {
  add_flag(name, write_back_ptr, flag_type_string);
}

void flag_uint64(char* name, uint64_t* write_back_ptr) {
  add_flag(name, write_back_ptr, flag_type_uint64);
}

void flag_int64(char* name, int64_t* write_back_ptr) {
  add_flag(name, write_back_ptr, flag_type_int64);
}

void flag_double(char* name, double* write_back_ptr) {
  add_flag(name, write_back_ptr, flag_type_double);
}

/**
 * @function flag_enum
 *
 * This should handle normal enums (which are represented in C as an
 * "int" when you use the enum to declare the type of the variable)
 * though an LLM is telling me that sometimes C will try to stuff
 * enum's into smaller types when they fit and I don't know enough to
 * say if this will be a problem.
 *
 * You will need to cast the write_back_ptr to int* to call this
 * function without getting a warning with Clang and other
 * compilers. Since you will probably declare the flag only once but
 * use it multiple times, this extra cast may not matter too much. You
 * could consider using an explicitly size type like uint64_t or
 * int64_t instead to hold the enum value and essentially only use
 * "enum" as a convenient way to "define" constants.
 */
void flag_enum(char* name, int* write_back_ptr) {
  add_flag(name, write_back_ptr, flag_type_enum);
  current_flag->enum_size = sizeof(int) * 8;
}

void flag_enum_64(char* name, uint64_t* write_back_ptr) {
  add_flag(name, write_back_ptr, flag_type_enum);
  current_flag->enum_size = 64;
}

void flag_enum_value(char* name, uint64_t value) {
  if (!current_flag || current_flag->flag_type != flag_type_enum) {
    log_fatal("The current flag is not an enum type");
    fatal_error(ERROR_ILLEGAL_STATE);
  }

  current_flag->enum_values = string_tree_insert(current_flag->enum_values,
                                                 name, u64_to_value(value));
}

// TODO(jawilson): flag_type_switch,
// flag_type_custom,

/**
 * @function flag_parse_command_line
 *
 * Parses a command line writing back the actual parsed values so they
 * can be used after command line parsing.
 *
 * When an error occurs, return a string with an error message OR NULL
 * if parsing was successfull.
 */
char* flag_parse_command_line(int argc, char** argv) {
  if (current_program == NULL) {
    log_fatal(
        "flag_parse_command_line can't be called unless flag_program_name() is "
        "first called.");
    fatal_error(ERROR_ILLEGAL_STATE);
  }

  int start = 1;
  command_descriptor_t* command = NULL;
  if (current_program->commands) {
    char* name = argv[1];
    command = flag_find_command_descriptor(name);
    if (command == NULL) {
      return string_printf(
          "The first command line argument is not a known command: %s", name);
    }
    start = 2;
  }

  value_array_t* files = make_value_array(argc);
  boolean_t parse_flags = true;

  for (int i = start; i < argc; i++) {
    char* arg = argv[i];
    if (parse_flags) {
      if (string_equal(arg, "--")) {
        parse_flags = false;
        continue;
      }

      if (string_starts_with(arg, "-")) {
        flag_key_value_t key_value = flag_split_argument(arg);
        if (key_value.key == NULL) {
          return string_printf("This argument is not a well formed flag: %s",
                               arg);
        }
        flag_descriptor_t* flag
            = flag_find_flag_descriptor(command, key_value.key);
        if (flag == NULL) {
          return string_printf(
              "The argument looks like a flag but was not found: '%s'\n"
              "You may want to use ' -- ' to seperate flags from non flag "
              "arguments.",
              arg);
        }
        // If the value is NULL, that means there was no "=" sign
        // which means we should grab the next argument directly. When
        // the argument ends with a trailing "=", we get back an empty
        // string which is legal for purely string arguments but other
        // argument types will generaly error out during parsing.
        if (key_value.value == NULL) {
          // TODO(jawilson): bounds checking!
          i++;
          key_value.value = argv[i];
        }
        char* error = parse_and_write_value(flag, key_value);
        if (error) {
          return error;
        }
        continue;
      }
    }
    // Either it doesn't look like a flag or else we are no longer
    // parsing flags because we saw "--".
    value_array_add(files, str_to_value(arg));
  }

  // Write back the left-over arguments
  *(current_program->write_back_file_args_ptr) = files;
  return NULL;
}

// Search the implicit "current_program" to see if anyone define
// "name" as a commmand.,
command_descriptor_t* flag_find_command_descriptor(char* name) {
  if (current_program->commands == NULL) {
    log_fatal(
        "flag_get_command() shouldn't not be called when we don't have any "
        "defined commands.");
    fatal_error(ERROR_ILLEGAL_STATE);
  }
  value_result_t command_value
      = string_tree_find(current_program->commands, name);
  if (is_ok(command_value)) {
    return ((command_descriptor_t*) (command_value.ptr));
  } else {
    return NULL;
  }
}

// Search the command for a matching flag and return that first but if
// the command doesn't have a definition for flag, then the the
// "program" might have a definition for the flag so we also search
// for it there.
flag_descriptor_t* flag_find_flag_descriptor(/*nullable*/
                                             command_descriptor_t* command,
                                             char* name) {
  if (command != NULL) {
    value_result_t command_flag_value = string_tree_find(command->flags, name);
    if (is_ok(command_flag_value)) {
      return cast(flag_descriptor_t*, command_flag_value.ptr);
    }
  }

  value_result_t program_flag_value
      = string_tree_find(current_program->flags, name);
  if (is_ok(program_flag_value)) {
    return cast(flag_descriptor_t*, program_flag_value.ptr);
  }

  return NULL;
}

// The returned key will start with one or more "-" characters.
//
// BTW, we are way down in the call stack and are not prepared to
// properly deal with say "---", and the caller currently actually
// benefits from seeing either "-" or "--" prepended to the key so we
// don't even bother to detect if there are more than two dashes. If
// this is illegal, the key (aka flag) is not found and better error
// reporting will happen in the caller.
//
// This actually looks like it could be a useful addition to the
// library if it can be given a descriptive generic name. Split on
// first?
//
// TODO(jawilson): Nothing says that error handler couldn't do as part
// of fuzzy matching notice this and be more helpful as a special
// case. Long command lines get broken at weird places so that might
// be useful to look out for.
flag_key_value_t flag_split_argument(char* arg) {
  int equal_sign_index = string_index_of_char(arg, '=');
  if (equal_sign_index >= 0) {
    char* key = string_substring(arg, 0, equal_sign_index);
    // We know there is an "=". If nothing comes after it, we want to
    // set value to "" instead of NULL so that we don't try to process
    // the next argument. So --foo and --foo=, will *not* be treeated
    // the same way.
    char* value = string_substring(arg, equal_sign_index + 1, strlen(arg));
    return (flag_key_value_t){.key = key, .value = value};
  }
  return (flag_key_value_t){.key = arg, .value = NULL};
}

// Figure out what parser to use for the value, parse it, and then use
// the address in the flag descriptor to write the flag value to where
// the user requested.
char* parse_and_write_value(flag_descriptor_t* flag,
                            flag_key_value_t key_value) {
  switch (flag->flag_type) {
  case flag_type_boolean:
    return parse_and_write_boolean(flag, key_value);

  case flag_type_string:
    *cast(char**, flag->write_back_ptr) = key_value.value;
    return NULL;

  case flag_type_uint64:
    return parse_and_write_uint64(flag, key_value);

  case flag_type_enum:
    return parse_and_write_enum(flag, key_value);

  default:
    fatal_error(ERROR_ILLEGAL_STATE);
    break;
  }
  return "<ILLEGAL-STATE-NOT-REACHED>";
}

char* parse_and_write_boolean(flag_descriptor_t* flag,
                              flag_key_value_t key_value) {
  char* val = key_value.value;
  if (string_equal("true", val) || string_equal("t", val)
      || string_equal("1", val)) {
    *cast(boolean_t*, flag->write_back_ptr) = true;
  } else if (string_equal("false", val) || string_equal("f", val)
             || string_equal("0", val)) {
    *cast(boolean_t*, flag->write_back_ptr) = false;
  } else {
    return string_printf("boolean flag %s does not accept value %s",
                         key_value.key, key_value.value);
  }
  return NULL;
}

char* parse_and_write_uint64(flag_descriptor_t* flag,
                             flag_key_value_t key_value) {
  value_result_t val_result = string_parse_uint64(key_value.value);
  if (is_ok(val_result)) {
    *cast(uint64_t*, flag->write_back_ptr) = val_result.u64;
  } else {
    return string_printf("uint64_t flag %s does not accept value %s",
                         key_value.key, key_value.value);
  }
  return NULL;
}

char* parse_and_write_enum(flag_descriptor_t* flag,
                           flag_key_value_t key_value) {
  value_result_t val_result
      = string_tree_find(flag->enum_values, key_value.value);
  if (is_ok(val_result)) {
    switch (flag->enum_size) {
    case 64:
      // TODO(jawilson): switch on size, check upper bits, etc.
      *cast(uint64_t*, flag->write_back_ptr) = val_result.u64;
      return NULL;
    case 32:
      // TODO(jawilson): switch on size, check upper bits, etc.
      *cast(uint32_t*, flag->write_back_ptr) = val_result.u64;
      return NULL;

    default:
      fatal_error(ERROR_ILLEGAL_STATE);
      break;
    }
  }
  return string_printf("Flag %s does not accept the argument value %s",
                       key_value.key, key_value.value);

  // TODO(jawilson): allow specifying by value if allowed
  /*
  value_result_t val_result = string_parse_uint64(key_value.value);
  if (is_ok(val_result)) {
    *cast(uint64_t*, flag->write_back_ptr) = val_result.u64;
  } else {
    fatal_error(ERROR_ILLEGAL_STATE);
  }
  */
}

/**
 * @function flag_print_help
 *
 * Print help according to the flags and "sub-commands" that have been
 * defined.
 */
void flag_print_help(FILE* out, char* message) {
  /*
  if (config->command_descriptors) {
    fprintf(stdout, "Usage: %s <command> <flags*> <files*>\n",
            config->program_name);
  } else {
    fprintf(stdout, "Usage: %s <flags*> <files*>\n", config->program_name);
  }
  if (config->program_description) {
    fprintf(stdout, "\n%s\n", config->program_description);
  }
  if (config->command_descriptors) {
    fprintf(stdout, "\nCommands:\n");
    for (int i = 0; i < config->command_descriptors->length; i++) {
      command_line_command_descriptor_t* command_descriptor
          = value_array_get(config->command_descriptors, i).ptr;
      fprintf(stdout, "    %s [[%s]]\n", command_descriptor->name,
              command_descriptor->help_string);
    }
  }
  if (config->flag_descriptors) {
    fprintf(stdout, "\nFlags:\n");
    for (int i = 0; i < config->flag_descriptors->length; i++) {
      command_line_flag_descriptor_t* flag
          = value_array_get(config->flag_descriptors, i).ptr;
      fprintf(stdout, "    --%s=<flag-value> (%s)\n", flag->long_name,
              flag->help_string);
    }
  }
  */
  fprintf(out, "flag_print_help() is not yet implemented!");
}
#line 2 "fatal-error.c"
/**
 * @file fatal-error.c
 *
 * The intent is that everything but a normal program exit will end up
 * here. (To catch SIGSIGV errors you may call
 * configure_fatal_errors() first with catch_sigsegv set.)
 *
 * Note that you can use fatal_error's to your advantage by setting
 * the environment variable ARMYKNIFE_FATAL_ERROR_SLEEP_SECONDS to
 * some value to give yourself enough time to attach a debugger.
 *
 * In this case C's macros are paying off as the file and line number
 * are easy to obtain.
 *
 * TODO(jawilson): environment variable to be quieter...
 */

// ======================================================================
// This is block is extraced to fatal-error.h
// ======================================================================

#ifndef _FATAL_ERROR_H_
#define _FATAL_ERROR_H_

struct fatal_error_config_S {
  boolean_t catch_sigsegv;
};

typedef struct fatal_error_config_S fatal_error_config_t;

/**
 * @constants error_code_t
 */
typedef enum {
  ERROR_UKNOWN,
  ERROR_SIGSEGV,
  ERROR_ACCESS_OUT_OF_BOUNDS,
  ERROR_BAD_COMMAND_LINE,
  ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER,
  ERROR_ILLEGAL_ENUM_VALUE,
  ERROR_ILLEGAL_INITIAL_CAPACITY,
  ERROR_ILLEGAL_NULL_ARGUMENT,
  ERROR_ILLEGAL_ZERO_HASHCODE_VALUE,
  ERROR_ILLEGAL_RANGE,
  ERROR_MEMORY_ALLOCATION,
  ERROR_MEMORY_FREE_NULL,
  ERROR_NOT_REACHED,
  ERROR_REFERENCE_NOT_EXPECTED_TYPE,
  ERROR_UNIMPLEMENTED,
  ERROR_OPEN_LOG_FILE,
  ERROR_TEST,
  ERROR_INTERNAL_ASSERTION_FAILURE,
  ERROR_BAD_ALLOCATION_SIZE,
  ERROR_ILLEGAL_ARGUMENT,
  ERROR_MEMORY_START_PADDING_ERROR,
  ERROR_MEMORY_END_PADDING_ERROR,
  ERROR_FATAL,
  ERROR_ILLEGAL_STATE,
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);
extern void configure_fatal_errors(fatal_error_config_t config);

/**
 * @macro fatal_error
 *
 * Terminates the program with a fatal error.
 */
#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */

// ======================================================================

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

fatal_error_config_t fatal_error_config = {0};

void segmentation_fault_handler(int signal_number) {
  fatal_error(ERROR_SIGSEGV);
}

void configure_fatal_errors(fatal_error_config_t config) {
  fatal_error_config = config;
  if (config.catch_sigsegv) {
    signal(SIGSEGV, segmentation_fault_handler);
  }
}

void print_fatal_error_banner();
void print_backtrace();
void print_error_code_name(int error_code);

char* get_program_path() {
  char buf[4096];
  int n = readlink("/proc/self/exe", buf, sizeof(buf));
  if (n > 0) {
    return string_duplicate(buf);
  } else {
    return "<program-path-unknown>";
  }
}

void _Noreturn fatal_error_impl(char* file, int line, int error_code) {
  print_fatal_error_banner();
  print_backtrace();
  fprintf(stderr, "%s:%d: FATAL ERROR %d", file, line, error_code);
  print_error_code_name(error_code);
  char* sleep_str = getenv("ARMYKNIFE_FATAL_ERROR_SLEEP_SECONDS");
  if (sleep_str != NULL) {
    value_result_t sleep_time = string_parse_uint64(sleep_str);
    if (is_ok(sleep_time)) {
      fprintf(stderr,
              "Sleeping for %lu seconds so you can attach a debugger.\n",
              sleep_time.u64);
      fprintf(stderr, "  gdb -tui %s %d\n", get_program_path(), getpid());
      sleep(sleep_time.u64);
    }
  }
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
  case ERROR_ILLEGAL_ARGUMENT:
    return "ERROR_ILLEGAL_ARGUMENT";
  case ERROR_MEMORY_START_PADDING_ERROR:
    return "ERROR_MEMORY_START_PADDING_ERROR";
  case ERROR_MEMORY_END_PADDING_ERROR:
    return "ERROR_MEMORY_END_PADDING_ERROR";

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

#include <limits.h>
#include <stdint.h>

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name);

extern void buffer_write_file(buffer_t* bytes, char* file_name);

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_read_until(buffer_t* buffer, FILE* input, char end_of_line);

int file_peek_byte(FILE* input);

boolean_t file_eof(FILE* input);

void file_copy_stream(FILE* input, FILE* output, boolean_t until_eof,
                      uint64_t size);

#endif /* _IO_H_ */

// ======================================================================

#include <stdio.h>

// This is optional...
#include <sys/stat.h>

/**
 * @function buffer_append_file_contents
 *
 * Completely reads a file and appends the contents to the passed in
 * buffer. This is often much more convenient than streaming a file.
 */
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

/**
 * @function buffer_write_file
 *
 * Writes the contents of the buffer to the given file.
 */
void buffer_write_file(buffer_t* bytes, char* file_name) {
  FILE* file = fopen(file_name, "w");
  fwrite(&bytes->elements, 1, bytes->length, file);
  fclose(file);
}

/**
 * @function buffer_read_until
 *
 * Read from a FILE* until either the end of file is reached or a
 * particular "end-of-line" character is read. Every character except
 * the end_of_line character is appended to the buffer.
 *
 * This can be used to read normal Unix "lines" or another use is to
 * read "lines" that end with NUL (U+0000) or until say a seperator
 * like "," is encountered.
 *
 * This function (and string_tokenize) should take a character set
 * which can unify their interface and allow U+0000 to be a member of
 * the set (unlike using a C string (aka char*) as the character set).
 *
 * TODO(jawilson): write a proper test!
 */
buffer_t* buffer_read_until(buffer_t* buffer, FILE* input, char end_of_line) {
  while (!feof(input)) {
    int ch = fgetc(input);
    if (ch < 0) {
      return buffer;
    }
    if (ch == end_of_line) {
      return buffer;
    }
    buffer = buffer_append_byte(buffer, ch);
  }
  return buffer;
}

/**
 * @function file_peek_char
 *
 * Returns the next byte from the input (as an int not u8_t) or -1 if
 * the end of the file input has been reached. A a byte is read, then
 * the byte is "pushed back" into the input stream so that if
 * file_peek_char, fgetc, or a host of other functions attempt to read
 * the input then
 */
int file_peek_byte(FILE* input) {
  if (feof(input)) {
    return -1;
  }
  int result = fgetc(input);
  // ungetc doesn't "push back" -1 according to
  // https://en.cppreference.com/w/c/io/ungetc
  // But who is going to trust that...
  if (result >= 0) {
    ungetc(result, input);
  }
  return result;
}

/**
 * Return true if an input stream is at the end of the file. I don't
 * know what "feof" really does but it doesn't actually do this.
 */
boolean_t file_eof(FILE* input) {
  return feof(input) || file_peek_byte(input) < 0;
}

#define FILE_COPY_STREAM_BUFFER_SIZE 1024

/**
 * Copy some or all of an input stream to an output stream.
 */
void file_copy_stream(FILE* input, FILE* output, boolean_t until_eof,
                      uint64_t size) {
  if (until_eof) {
    size = ULLONG_MAX;
  }

  uint8_t buffer[FILE_COPY_STREAM_BUFFER_SIZE];
  while (size > 0) {
    int minimum = size < FILE_COPY_STREAM_BUFFER_SIZE
                      ? size
                      : FILE_COPY_STREAM_BUFFER_SIZE;
    uint64_t n_read = fread(buffer, 1, minimum, input);
    if (n_read == 0) {
      break;
    }
    fwrite(buffer, 1, n_read, output);
    size -= n_read;
  }
}
#line 2 "logger.c"

/**
 * @file logger.c
 *
 * A "logger" is a type of code instrumentation and provides the
 * ability to put explicit "print statements" into your code without
 * necessarily having a large impact on the performance of that code
 * as long as the logger is turned off for that "level" of detail.
 *
 * If you've temporarily inserted print statements into your program,
 * you've probably already learned that logging is very useful and
 * also somewhat expensive in terms of run-time performance. [^1]
 *
 * Since logging is expensive, it is useful to be able to turn it on
 * and off (even without recompiling) which is done based on the
 * logging level. When logging is turned off, the cost is meant to be
 * equivalent to a load, compare, and branch though logging can effect
 * the C compiler's optimizations, so it is **not** recommended to be
 * left in critical loops. Obviously if the code is compiled into the
 * binary, even if the code to skip the logging doesn't considerably
 * increase run-time performance, it may still have an impact for
 * example on the output binary size.
 *
 * The default log level is "WARN" though it is possible to override
 * this with #define ARMYKNIFE_LIB_DEFAULT_LOG_LEVEL <level> when
 * including the library implementation (or from your build command
 * which allows C preprocessor definitions to be injected into your
 * source code, one reason you may want a debug vs production builds).
 *
 * Additionally, when the first log statement is encountered, we
 * examine the environment variable named ARMYKNIFE_LIB_LOG_LEVEL if
 * you want to adjust the level after compilation. Future versions
 * will certainly provide more control such as turn on logging only
 * for specific files as well as giving the C compiler enough
 * information to remove some logging code completely from the
 * binary).
 *
 * There are a set number of levels and they are defined like so:
 *
 * OFF = 0
 * TRACE = 1
 * DEBUG = 2
 * INFO = 3
 * WARN = 4
 * FATAL = 5
 *
 * The most overlooked part of logging may be that putting PII or
 * other information into logs may violate GDPR and other privacy laws
 * depending on how the logs are processed and retained. Our
 * recommendation is to never intentionally log PII. It's especially
 * important to keep this in mind if you are developing an internet
 * application that the user isn't running on their own machine which
 * isn't an initial focus of this library.
 *
 * [^1]: For this implementation, getting a timestamp is probably one
 * kernel call and doing the actual output, since logging is less
 * useful when buffered, requires at least another kernel
 * call. Finally, formatting strings for human readability is
 * relatively expensive itself. For example, printing a large number
 * may require dozens or hundreds of cycles while adding two numbers
 * may take less than a single cycle on a modern pipelined processor).
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdarg.h>
#include <stdio.h>

#define LOGGER_OFF 0
#define LOGGER_TRACE 1
#define LOGGER_DEBUG 2
#define LOGGER_INFO 3
#define LOGGER_WARN 4
#define LOGGER_FATAL 5

struct logger_state_S {
  boolean_t initialized;
  int level;
  char* logger_output_filename;
  FILE* output;
};

typedef struct logger_state_S logger_state_t;

#ifndef LOGGER_DEFAULT_LEVEL
#define LOGGER_DEFAULT_LEVEL LOGGER_WARN
#endif /* LOGGER_DEFAULT_LEVEL */

logger_state_t global_logger_state
    = (logger_state_t){.level = LOGGER_DEFAULT_LEVEL};

extern void logger_init(void);

__attribute__((format(printf, 4, 5))) extern void
    logger_impl(char* file, int line_number, int level, char* format, ...);

/**
 * @macro log_none
 *
 * This will never ever log and should have essentially zero impact on
 * compilation (including detecting errors). In other words it should
 * behave like an empty statment ";".
 *
 * On the other hand, errors in these statements will not be caught
 * and therefore it can't be depended on to keep working as you
 * refactor code and decide later that you want to turn it on.
 */
#define log_none(format, ...)                                                  \
  do {                                                                         \
  } while (0);

/**
 * @macro log_off
 *
 * This will never log however the compiler *should* still check to
 * make sure the code is legal and compiles. Any sufficiently smart
 * compiler with some level of optimization turned on should not
 * change it's code generation strategy at all if you leave one of these
 * statements in your source code and you should easily be able to
 * upgrade them to a real level later.
 */
#define log_off(format, ...)                                                   \
  do {                                                                         \
    if (0) {                                                                   \
      logger_impl(__FILE__, __LINE__, LOGGER_TRACE, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

/**
 * @macro log_trace
 *
 * Log at the TRACE level using printf style formatting.
 */
#define log_trace(format, ...)                                                 \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_TRACE) {                           \
      logger_impl(__FILE__, __LINE__, LOGGER_TRACE, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

/**
 * @macro log_debug
 *
 * Log at the DEBUG level using printf style formatting.
 */
#define log_debug(format, ...)                                                 \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_DEBUG) {                           \
      logger_impl(__FILE__, __LINE__, LOGGER_DEBUG, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

/**
 * @macro log_info
 *
 * Log at the INFO level using printf style formatting.
 */
#define log_info(format, ...)                                                  \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_INFO) {                            \
      logger_impl(__FILE__, __LINE__, LOGGER_INFO, format, ##__VA_ARGS__);     \
    }                                                                          \
  } while (0)

/**
 * Determine if logging at the INFO level is enabled.
 */
static inline boolean_t should_log_info() {
  return global_logger_state.level <= LOGGER_INFO;
}

/**
 * @macro log_warn
 *
 * Log at the WARN level using printf style formatting.
 */
#define log_warn(format, ...)                                                  \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_WARN) {                            \
      logger_impl(__FILE__, __LINE__, LOGGER_WARN, format, ##__VA_ARGS__);     \
    }                                                                          \
  } while (0)

/**
 * @macro log_fatal
 *
 * Log at the FATAL level using printf style formatting.
 *
 * Typically this is only done before invoking fatal_error though I
 * don't have a convenient way to enforce this.
 */
#define log_fatal(format, ...)                                                 \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_FATAL) {                           \
      logger_impl(__FILE__, __LINE__, LOGGER_FATAL, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

#endif /* _LOGGER_H_ */

value_result_t parse_log_level_enum(char* str) {
  if (strcmp("FATAL", str) == 0 || strcmp("fatal", str) == 0) {
    return (value_result_t){.u64 = LOGGER_FATAL};
  } else if (strcmp("WARN", str) == 0 || strcmp("warn", str) == 0) {
    return (value_result_t){.u64 = LOGGER_WARN};
  } else if (strcmp("INFO", str) == 0 || strcmp("info", str) == 0) {
    return (value_result_t){.u64 = LOGGER_INFO};
  } else if (strcmp("DEBUG", str) == 0 || strcmp("debug", str) == 0) {
    return (value_result_t){.u64 = LOGGER_DEBUG};
  } else if (strcmp("TRACE", str) == 0 || strcmp("trace", str) == 0) {
    return (value_result_t){.u64 = LOGGER_TRACE};
  } else if (strcmp("OFF", str) == 0 || strcmp("off", str) == 0) {
    return (value_result_t){.u64 = LOGGER_OFF};
  } else {
    return (value_result_t){.nf_error = NF_ERROR_NOT_PARSED_AS_EXPECTED_ENUM};
  }
}

/**
 * @function logger_init
 *
 * This function modifies the logging level based on the environment
 * variable ARMYKNIFE_LIB_LOG_LEVEL (which currently must be a
 * number).
 *
 * While not required to actually use logging, the logging level will
 * be set to LOGGER_WARN unless you change it in a debugger, and those
 * logging statements will be sent to stderr which is probably not
 * convenient.
 */
void logger_init(void) {
  char* level_string = getenv("ARMYKNIFE_LIB_LOG_LEVEL");
  if (level_string != NULL) {
    value_result_t parsed = string_parse_uint64(level_string);
    if (is_ok(parsed)) {
      global_logger_state.level = parsed.u64;
    } else {
      value_result_t parsed = parse_log_level_enum(level_string);
      if (is_ok(parsed)) {
        global_logger_state.level = parsed.u64;
      } else {
        log_warn("%s could not be converted to a log level.", level_string);
      }
    }
  }

  fprintf(stderr, "Level is set to %d", global_logger_state.level);

  char* output_file_name = getenv("ARMYKNIFE_LIB_LOG_FILE");

  // It's pretty standard to include the "pid" in the filename at
  // least when writing to /tmp/. We aren't quite doing that yet...
  //
  // pid_t pid = getpid(); -- pid is a number of some sort...

  if (output_file_name != NULL) {
    global_logger_state.output = fopen(output_file_name, "w");
    if (!global_logger_state.output) {
      fatal_error(ERROR_OPEN_LOG_FILE);
    }
    // Set the stream to unbuffered
    // if (setvbuf(log_file, NULL, _IONBF, 0) != 0) {
    // perror("Failed to set stream to unbuffered");
    // exit(EXIT_FAILURE);
    // }
    global_logger_state.logger_output_filename = output_file_name;
  } else {
    global_logger_state.output = stderr;
    global_logger_state.initialized = true;
  }
}

// Convert the level to a human readable string (which will also
// appear as the name in the log file).
char* logger_level_to_string(int level) {
  switch (level) {
  case LOGGER_OFF:
    return "LOGGER_OFF";
  case LOGGER_TRACE:
    return "TRACE";
  case LOGGER_DEBUG:
    return "DEBUG";
  case LOGGER_INFO:
    return "INFO";
  case LOGGER_WARN:
    return "WARN";
  case LOGGER_FATAL:
    return "FATAL";
  default:
    return "LEVEL_UNKNOWN";
  }
}

/**
 * @function logger_impl
 *
 * This is the non macro version entry point into the logger. Normally
 * it wouldn't be called directly since it is less convenient than the
 * macro versions.
 */
__attribute__((format(printf, 4, 5))) void
    logger_impl(char* file, int line_number, int level, char* format, ...) {

  FILE* output = global_logger_state.output;

  // Ensure that logging to somewhere will happen though a later call
  // to logger_init() may send the output to somewhere else.
  if (output == NULL) {
    output = stderr;
  }

  if (level >= global_logger_state.level) {
    fprintf(output, "%s ", logger_level_to_string(level));
    va_list args;
    va_start(args, format);
    fprintf(output, "%s:%d ", file, line_number);
    vfprintf(output, format, args);
    fprintf(output, "\n");
    va_end(args);
  }
}
#line 2 "random.c"

/**
 * @file random.c
 *
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
extern uint64_t random_next_uint64(random_state_t* state);
extern uint64_t random_next_uint64_below(random_state_t* state,
                                         uint64_t maximum);

#endif /* _RANDOM_H_ */

/**
 * @function random_state_for_test
 *
 * Return a consistent initial random state for tests.
 */
random_state_t random_state_for_test(void) {
  return (random_state_t){.a = 0x1E1D43C2CA44B1F5, .b = 0x4FDD267452CEDBAC};
}

static inline uint64_t rotl(uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

/**
 * @function random_next
 *
 * Return a random uint64_t from the current state (and update the
 * state).
 */
uint64_t random_next(random_state_t* state) {
  uint64_t s0 = state->a;
  uint64_t s1 = state->b;
  uint64_t result = rotl(s0 * 5, 7) * 9;
  s1 ^= s0;
  state->a = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
  state->b = rotl(s1, 37);                   // c

  return result;
}

/**
 * @function random_next_uint64_below
 *
 * Return a random `uint64_t` that is below some maximum. As much as
 * the underlying random number generartor allows, this should be
 * uniform.
 */
uint64_t random_next_uint64_below(random_state_t* state, uint64_t maximum) {
  if (maximum == 0) {
    fatal_error(ERROR_ILLEGAL_ARGUMENT);
  }
#if 1
  // This is simpler and works well in practice (it seems).
  return random_next(state) % maximum;
#else
  // This version in theory should be a bit more fair than modulous
  // but I can't really detect a difference.
  int mask = (1ULL << (uint64_highest_bit_set(maximum) + 1)) - 1;
  while (1) {
    uint64_t n = random_next(state);
    n &= mask;
    if (n < maximum) {
      return n;
    }
  }
#endif /* 0 */
}
#line 2 "string-alist.c"
/**
 * @file string-alist.c
 *
 * An association list (a type of map) from a string to a value_t.
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

/**
 * @macro string_alist_foreach
 *
 * Allows iteration over the keys and values in a string association
 * list.
 */
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

/**
 * @function alist_insert
 *
 * Insert a new key and value into an assocation list.
 */
string_alist_t* alist_insert(string_alist_t* list, char* key, value_t value) {
  string_alist_t* result = (malloc_struct(string_alist_t));
  result->next = alist_delete(list, key);
  result->key = key;
  result->value = value;
  return result;
}

/**
 * @function alist_delete
 *
 * Delete the key and associated value from the given association
 * list. Neither the key nor the value associated are themselves
 * freed.
 */
string_alist_t* alist_delete(string_alist_t* list, char* key) {
  // This appears to be logically correct but could easily blow out
  // the stack with a long list.
  if (list == NULL) {
    return list;
  }
  if (strcmp(key, list->key) == 0) {
    string_alist_t* result = list->next;
    free_bytes(list);
    return result;
  }
  list->next = alist_delete(list->next, key);
  return list;
}

/**
 * @function alist_find
 *
 * Find the value associate with the given key. Use is_ok() or
 * is_not_ok() to see if the value is valid (i.e., if the key was
 * actually found).
 */
value_result_t alist_find(string_alist_t* list, char* key) {
  while (list) {
    if (strcmp(key, list->key) == 0) {
      return (value_result_t){.val = list->value};
    }
    list = list->next;
  }
  return (value_result_t){.nf_error = NF_ERROR_NOT_FOUND};
}
#line 2 "string-hashtable.c"
/**
 * @file string-hashtable.c
 *
 * A hash map of string to a value_t.
 *
 * It's high unlikely we are close to JVM level of performance in part
 * because we may be using a slower (but higher quality) hashing
 * function and this generally does not pay off. We also use chaining
 * instead of open addressing since this allowed the most code reuse
 * and a simpler implementation.
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

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_ht_insert(string_hashtable_t* ht, char* key, value_t value);

__attribute__((warn_unused_result)) extern string_hashtable_t*
    string_ht_delete(string_hashtable_t* ht, char* key);

extern value_result_t string_ht_find(string_hashtable_t* ht, char* key);

/**
 * @macro string_ht_foreach
 *
 * Allows traversing all elements of a hashtable in an unspecified
 * order.
 */
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
 * @function make_string_hashtable
 *
 * Create a hashtable with the given number of buckets. This
 * implementation currently never grows a hashtable so you may want to
 * start with a healthy initial capacity.
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
 * @function string_ht_insert
 *
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
 * @function string_ht_delete
 *
 * Delete an association from the hashtable. It is not an error to
 * delete a key that doesn't exist in the hashtable.
 */
string_hashtable_t* string_ht_delete(string_hashtable_t* ht, char* key) {
  uint64_t hashcode = string_hash(key);
  int bucket = hashcode % ht->n_buckets;
  string_alist_t* list = ht->buckets[bucket];
  ht->buckets[bucket] = alist_delete(list, key);
  return ht;
}

/**
 * @function string_ht_find
 *
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
 * @file string-tree.c
 *
 * This is a balanced binary tree to associate a string to a value.
 *
 * Generally a string_alist is prefered for small "maps", and
 * string_hashtable is prefered for large maps, but string_tree is the
 * easiest way to get sorted results (which is most important for
 * reproducibility).
 *
 * Currently we are using "AA" trees (see
 * https://en.wikipedia.org/wiki/AA_tree) since it has simpler code
 * than many other balanced trees (like red-block trees) and the
 * Wikipedia article and paper spell out *most* of the non-trivial
 * details.
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

/**
 * @macro string_tree_foreach
 *
 * Perform an inorder traversal of a string-tree.
 *
 * key_var is created in a new block scope with type char*.
 *
 * value_var is created in a new block scope with type value_t and you
 * will probably want to use something like ".ptr" or ".u64" on the
 * value to obtain the actual value.
 *
 * statements should be a normal C block, aka, something like:
 * ```
 * {
 *   statement1();
 *   statement2();
 * }
 * ```
 *
 * Unforunately it is not possible to use "break" or "continue" with
 * this style of loop (and worse, there will be no compilation error
 * or warning if you accidentally do that...)
 */
#define string_tree_foreach(tree, key_var, value_var, statements)              \
  do {                                                                         \
    int stack_n_elements = 0;                                                  \
    string_tree_t* stack[64];                                                  \
    string_tree_t* current = tree;                                             \
    while (current != NULL || stack_n_elements > 0) {                          \
      while (current != NULL) {                                                \
        stack[stack_n_elements++] = current;                                   \
        current = current->left;                                               \
      }                                                                        \
      current = stack[--stack_n_elements];                                     \
      char* key_var = current->key;                                            \
      value_t value_var = current->value;                                      \
      statements;                                                              \
      current = current->right;                                                \
    }                                                                          \
  } while (0)

#endif /* _STRING_TREE_H_ */

/**
 * @function string_tree_find
 *
 * Find the value associate with the key in the tree.
 */
value_result_t string_tree_find(string_tree_t* t, char* key) {
  if (t == NULL) {
    return (value_result_t){.nf_error = NF_ERROR_NOT_FOUND};
  }
  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    return string_tree_find(t->left, key);
  } else if (cmp_result > 0) {
    return string_tree_find(t->right, key);
  } else {
    return (value_result_t){
        .val = t->value,
    };
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
    R->level++;
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
 * @function string_tree_insert
 *
 * Insert an association of key and a value (or update the current
 * value stored in the tree).
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
    // Either key or t->key might need to be freed but it isn't even
    // possible to tell if either has been "malloced" so good luck
    // figuring that out.
    t->value = value;
    return t;
  }

  t = string_tree_skew(t);
  t = string_tree_split(t);

  return t;
}

static inline uint64_t min_level(uint32_t a, uint32_t b) {
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
 * @function string_tree_delete
 *
 * Delete the association of key (if it exists in the tree). It is not
 * an error to delete a key that isn't present in the table.
 */
string_tree_t* string_tree_delete(string_tree_t* t, char* key) {

  if (t == NULL) {
    return t;
  }

  int cmp_result = strcmp(key, t->key);
  if (cmp_result < 0) {
    t->left = string_tree_delete(t->left, key);
  } else if (cmp_result > 0) {
    t->right = string_tree_delete(t->right, key);
  } else {
    if (string_tree_is_leaf(t)) {
      // Since we are a leaf, nothing special to do except make sure
      // this leaf node is no longer in the tree. wikipedia says
      // "return right(T)" which is technically correct, but this is
      // clearer.
      return NULL;
    } else if (t->left == NULL) {
      string_tree_t* L = string_tree_successor(t);
      // Note: wikipedia or the orginal article may have a bug. Doing
      // the delete and then the key/value assignment leads to a
      // divergence with a reference implementation.
      t->key = L->key;
      t->value = L->value;
      t->right = string_tree_delete(t->right, L->key);
    } else {
      string_tree_t* L = string_tree_predecessor(t);
      // Note: wikipedia or the orginal article may have a bug. Doing
      // the delete and then the key/value assignment leads to a
      // divergence with a reference implementation.
      t->key = L->key;
      t->value = L->value;
      t->left = string_tree_delete(t->left, L->key);
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
extern char* uint64_to_string(uint64_t number);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern value_result_t string_parse_uint64(const char* string);
extern value_result_t string_parse_uint64_dec(const char* string);
extern value_result_t string_parse_uint64_hex(const char* string);
extern value_result_t string_parse_uint64_bin(const char* string);
extern char* string_duplicate(const char* src);
extern char* string_append(const char* a, const char* b);
extern char* string_left_pad(const char* a, int count, char ch);
__attribute__((format(printf, 1, 2))) extern char* string_printf(char* format,
                                                                 ...);

#endif /* _STRING_UTIL_H_ */

// ======================================================================

#include <stdlib.h>
#include <string.h>

uint64_t fasthash64(const void* buf, size_t len, uint64_t seed);

/**
 * @function string_is_null_or_empty
 *
 * Return true if the given string is NULL or strlen is zero.
 */
int string_is_null_or_empty(const char* str) {
  return (str == NULL) || (strlen(str) == 0);
}

/**
 * @function string_equal
 *
 * Return true if two strings are equal.
 */
int string_equal(const char* str1, const char* str2) {
  if (string_is_null_or_empty(str1)) {
    return string_is_null_or_empty(str2);
  }
  return strcmp(str1, str2) == 0;
}

/**
 * @function string_starts_with
 *
 * Return true if str1 starts with all of str2.
 */
int string_starts_with(const char* str1, const char* str2) {
  return strncmp(str1, str2, strlen(str2)) == 0;
}

/**
 * @function string_ends_with
 *
 * Return true if str1 ends with all of str2.
 */
int string_ends_with(const char* str1, const char* str2) {
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);

  if (len2 > len1) {
    return 0;
  }

  return strcmp(str1 + (len1 - len2), str2) == 0;
}

/**
 * @function string_contains_char
 *
 * Return true if str contains that given character ch.
 */
boolean_t string_contains_char(const char* str, char ch) {
  return string_index_of_char(str, ch) >= 0;
}

/**
 * @function string_index_of_char
 *
 * Return the index of the given character in a string or a value less
 * than zero if the character isn't inside of the string.
 */
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

/**
 * @function string_hash
 *
 * Return a fast but generally high-quality 64bit hash of an input
 * string.
 */
uint64_t string_hash(const char* str) {
  return fasthash64(str, strlen(str), 0);
}

/**
 * @function string_substring
 *
 * Return a substring of the given string as a newly allocated string.
 */
char* string_substring(const char* str, int start, int end) {
  uint64_t len = strlen(str);
  if (start >= len || start >= end || end < start) {
    fatal_error(ERROR_ILLEGAL_ARGUMENT);
  }
  int result_size = end - start + 1;
  char* result = (char*) (malloc_bytes(result_size));
  for (int i = start; (i < end); i++) {
    result[i - start] = str[i];
  }
  result[result_size - 1] = '\0';
  return result;
}

value_result_t string_parse_uint64_dec(const char* string) {
  uint64_t len = strlen(string);
  uint64_t integer = 0;

  if (len == 0) {
    return (value_result_t){.u64 = 0,
                            .nf_error = NF_ERROR_NOT_PARSED_AS_NUMBER};
  }

  for (int i = 0; i < len; i++) {
    char ch = string[i];
    if (ch < '0' || ch > '9') {
      return (value_result_t){.u64 = 0,
                              .nf_error = NF_ERROR_NOT_PARSED_AS_NUMBER};
    }
    uint64_t digit = string[i] - '0';
    integer = integer * 10 + digit;
  }

  return (value_result_t){.u64 = integer, .nf_error = NF_OK};
}

/**
 * Parse a sequence of zeros and ones. A prefix like 0b should be
 * stripped before calling this routine and the string must only
 * contain the digits 0 and 1.
 */
value_result_t string_parse_uint64_bin(const char* string) {
  uint64_t len = strlen(string);
  uint64_t integer = 0;

  if (len == 0) {
    return (value_result_t){.u64 = 0,
                            .nf_error = NF_ERROR_NOT_PARSED_AS_NUMBER};
  }

  for (int i = 0; i < len; i++) {
    char ch = string[i];
    if (ch < '0' || ch > '1') {
      return (value_result_t){.u64 = 0,
                              .nf_error = NF_ERROR_NOT_PARSED_AS_NUMBER};
    }
    uint64_t digit = string[i] - '0';
    integer = integer << 1 | digit;
  }

  return (value_result_t){.u64 = integer, .nf_error = NF_OK};
}

static inline boolean_t is_hex_digit(char ch) {
  return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f');
}

static inline uint64_t hex_digit_to_value(char ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  } else {
    return (ch - 'a') + 10;
  }
}

/**
 * Parse a sequence of characters "0123456789abcdef" to an uint64_t. A
 * prefix like 0x should be striped before calling this routine.
 *
 */
value_result_t string_parse_uint64_hex(const char* string) {
  uint64_t len = strlen(string);
  uint64_t integer = 0;

  if (len == 0) {
    return (value_result_t){.u64 = 0,
                            .nf_error = NF_ERROR_NOT_PARSED_AS_NUMBER};
  }

  for (int i = 0; i < len; i++) {
    char ch = string[i];
    if (!is_hex_digit(ch)) {
      return (value_result_t){.u64 = 0,
                              .nf_error = NF_ERROR_NOT_PARSED_AS_NUMBER};
    }
    uint64_t digit = hex_digit_to_value(ch);
    integer = integer << 4 | digit;
  }

  return (value_result_t){.u64 = integer, .nf_error = NF_OK};
}

/**
 * @function string_parse_uint64
 *
 * Parse a string as a uint64_t.
 *
 * If the string begins with "0x", the the number should be a well
 * formed hexidecimal number (in all lower-case).
 *
 * If the string begins with "0b", the the number should be a well
 * formed binary number.
 *
 * Ortherwise the number should be a well-formed decimal number.
 *
 * While currently overflow is not detected, that is likely to be
 * detected in future versions of the library.
 */
value_result_t string_parse_uint64(const char* string) {
  if (string_starts_with(string, "0x")) {
    return string_parse_uint64_hex(&string[2]);
  } else if (string_starts_with(string, "0b")) {
    return string_parse_uint64_bin(&string[2]);
  } else {
    return string_parse_uint64_dec(string);
  }
}

/**
 * @function string_duplicate
 *
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
 * @function string_append
 *
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
 * @function uint64_to_string
 *
 * Convert a uint64_t number to a string.
 */
char* uint64_to_string(uint64_t number) {
  char buffer[32];
  sprintf(buffer, "%lu", number);
  return string_duplicate(buffer);
}

/**
 * @function string_left_pad
 *
 * Prefix a string with left padding (if necessary) to make it at
 * least N bytes long.
 */
char* string_left_pad(const char* str, int n, char ch) {
  int str_length = strlen(str);
  if (str_length > n) {
    return string_duplicate(str);
  }
  char* result = (char*) malloc_bytes(n + 1);
  for (int i = 0; i < n - str_length; i++) {
    result[i] = ch;
  }
  for (int i = 0; i < str_length; i++) {
    result[i + (n - str_length)] = str[i];
  }
  result[n] = '\0';
  return result;
}

// Allows tests to make the temporary buffer small to more easily test
// the case where vsnprintf is called twice because the first time it
// was called we didn't have a large enough buffer.
#ifndef STRING_PRINTF_INITIAL_BUFFER_SIZE
#define STRING_PRINTF_INITIAL_BUFFER_SIZE 1024
#endif /* STRING_PRINTF_INITIAL_BUFFER_SIZE */

/**
 * @function string_printf
 *
 * Perform printf to a buffer and return the result as a dynamically
 * allocated string. The string is automatically allocated to the
 * appropriate size.
 */
__attribute__((format(printf, 1, 2))) char* string_printf(char* format, ...) {
  char buffer[STRING_PRINTF_INITIAL_BUFFER_SIZE];
  int n_bytes = 0;
  do {
    va_list args;
    va_start(args, format);
    n_bytes
        = vsnprintf(buffer, STRING_PRINTF_INITIAL_BUFFER_SIZE, format, args);
    va_end(args);
  } while (0);

  if (n_bytes < STRING_PRINTF_INITIAL_BUFFER_SIZE) {
    char* result = (char*) malloc_bytes(n_bytes + 1);
    strcat(result, buffer);
    return result;
  } else {
    char* result = (char*) malloc_bytes(n_bytes + 1);
    va_list args;
    va_start(args, format);
    int n_bytes_second = vsnprintf(result, n_bytes + 1, format, args);
    va_end(args);
    if (n_bytes_second != n_bytes) {
      fatal_error(ERROR_INTERNAL_ASSERTION_FAILURE);
    }
    return result;
  }
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

/**
 * @macro test_fail
 *
 * Immediately fail a test after printing a message (supplied the same
 * way as printf, log_fatal, etc.)
 *
 * This is a macro instead of a function because it automatically
 * passes the current file and line number to test_fail_and_exit which
 * enables uniform and convenient error messagages according to GNU
 * guidelines (so they will look like gcc compiler errors if your IDE
 * understands those).
 *
 * It is *advised* (at least for now) to not pass complicated
 * expressions to `test_fail` if those are likely to fail. Instead run
 * the test in a debugger and set a break-point on
 * `test_fail_and_exit`.
 */
#define test_fail(format, ...)                                                 \
  do {                                                                         \
    test_fail_and_exit(__FILE__, __LINE__, format, ##__VA_ARGS__);             \
  } while (0)


/**
 * @macro test_assert
 *
 * Evaluate the condition and fail the test if the result is false.
 */
#define test_assert(condition)                                                 \
  do {                                                                         \
    if (!(condition))                                                          \
      test_fail("A test assertion failed. Condition expression was: %s",       \
                #condition);                                                   \
  } while (0)

#endif /* _TEST_H_ */

/**
 * @function test_fail_and_exit
 *
 * Set a break-point here to debug a test but normally you will use
 * the macro `test_fail` since it is much more convenient.
 */
__attribute__((format(printf, 3, 4))) void
    test_fail_and_exit(char* file_name, int line_number, char* format, ...) {
  va_list args;
  fprintf(stdout, "%s:%d: ", file_name, line_number);
  va_start(args, format);
  vfprintf(stdout, format, args);
  fprintf(stdout, "\n");
  va_end(args);
  exit(1);
}
#line 2 "tokenizer.c"
/**
 * @file tokenizer.c
 *
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

extern value_array_t* string_tokenize(const char* str, const char* delimiters);
extern value_array_t* buffer_tokenize(buffer_t* buffer, const char* delimiters);
extern value_array_t* tokenize_memory_range(uint8_t* start, uint64_t length,
                                            const char* delimiters);

// TODO(jawilson):

#endif /* _TOKENIZER_H_ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_duplicate(value_array_t* token_array, const char* data);

/**
 * @function string_tokenize
 *
 * Tokenize a string into a a value_array_t of (non-empty) strings.
 *
 * Delimiters terminate the current token and are thrown away. The
 * delimiters string is treated as a sequence of delimiter characters,
 * it does not mean a delimiter can be multiple characters.
 */
value_array_t* string_tokenize(const char* str, const char* delimiters) {
  return tokenize_memory_range((uint8_t*) str, strlen(str), delimiters);
}

/**
 * @function buffer_tokenize
 *
 * Tokenize the current contents of a buffer into a value_array_t of
 * (non-empty) strings. The input buffer should contain a valid UTF-8
 * encoded string.
 *
 * NUL bytes inside the buffer are automatically treated as an
 * additional delimiter.
 */
value_array_t* buffer_tokenize(buffer_t* buffer, const char* delimiters) {
  return tokenize_memory_range(&(buffer->elements[0]), buffer->length,
                               delimiters);
}

/**
 * @function tokenize_memory_range
 *
 * Tokenize a memory range into a value_array_t of (non-empty) strings. The
 * range should contain a valid UTF-8 encoded string.
 *
 * NUL bytes are automatically treated as an additional delimiter.
 */
value_array_t* tokenize_memory_range(uint8_t* str, uint64_t length,
                                     const char* delimiters) {
  value_array_t* result = make_value_array(1);
  char token_data[1024];
  int cpos = 0;
  for (int i = 0; (i < length); i++) {
    uint8_t ch = str[i];
    if (ch == 0 || string_contains_char(delimiters, ch)) {
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

// Add a *copy* of the string named data to the token list.
void add_duplicate(value_array_t* token_array, const char* data) {
  value_array_add(token_array, str_to_value(string_duplicate(data)));
}
#line 2 "uint64.c"
/**
 * @file uint64.c
 *
 * Implement a couple of useful operations on uint64_t (which can
 * often be used for smaller types).
 */

#ifndef _UINT64_H_
#define _UINT64_H_

#include <stdint.h>

extern int uint64_highest_bit_set(uint64_t n);

#endif /* _UINT64_H_ */

/**
 * @function uint64_highest_bit_set
 *
 */
int uint64_highest_bit_set(uint64_t n) {
  if (n >= 1ULL << 32) {
    return uint64_highest_bit_set(n >> 32) + 32;
  } else if (n >= 1ULL << 16) {
    return uint64_highest_bit_set(n >> 16) + 16;
  } else if (n >= 1ULL << 8) {
    return uint64_highest_bit_set(n >> 8) + 8;
  } else if (n >= 1ULL << 4) {
    return uint64_highest_bit_set(n >> 4) + 4;
  } else if (n >= 1ULL << 2) {
    return uint64_highest_bit_set(n >> 2) + 2;
  } else if (n >= 1ULL << 1) {
    return uint64_highest_bit_set(n >> 1) + 1;
  } else {
    return 0;
  }
}
#line 2 "value.c"

#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>

/**
 * @file value.c
 *
 * A major part of the armyknife library is giving basic "collection"
 * capabilities to C.
 *
 * In an ideal world, C would allow structures to be parameterized so
 * that a field holding a "key" or a "value" could change size (based
 * on its static parameters).
 *
 * Since that capability doesn't currently exist, instead we take an
 * approach where keys and values are always the same size (64
 * bits). While this doesn't allow storing *anything* as a key or
 * value, it does allow storing a *pointer to anything* as a key or
 * value. This is actually very similar to how Java collections work
 * except we can store primitive values like integers and doubles
 * without "boxing".
 *
 * When "searching" a collection for a key, we want to be able to
 * return "not found" (and potentially other "non-fatal" error
 * conditions). For this reason we also have the value_result_t
 * structure, which is a pair of a value_t and a
 * non_fatal_error_code_t. (It's actually slightly more complicated
 * because `tcc` treats anonymous unions a little differently than
 * `gcc` and `clang` so we work around that by repeating the fields of
 * a value to make value_result_t a bit more convenient to work with).
 *
 * Sometimes value_result_t is also used by non collection based
 * functions, such as parsing an integer, so that other non-fatal
 * errors can be communicated back to the caller.
 *
 * The contract with returning a non-fatal errors is that the state of
 * the system is in a good state to continue processing (or they get
 * to decide if the error should be fatal). Users will thus expect
 * that when a non-fatal error is returned that no global state
 * modification has ocurred.
 *
 * value_t's are typically only passed into functions while
 * optional_value_result_t's are typically returned from functions.
 *
 * When a value_result_t is returned you must always check for an
 * error code before using the value component of the result. `is_ok`
 * and `is_not_ok` make this slightly easier and easier to read.
 *
 * value_t's and value_result_t's carry no type information that can
 * be queried at runtime and by their nature C compilers are going to
 * do a very incomplete job of statically type checking these. For
 * example you can easily put a double into a collection and
 * successfully get back this value and use it as a very suspicious
 * pointer and the compiler will not warn you about this. So
 * collections aren't as safe as in other languages at either compile
 * or run-time. (Java's collections (when generic types are *not*
 * used), are not "safe" at compile time but are still dynamically
 * safe.)
 *
 * On the positive side, the lack of dynamic typing means you haven't
 * paid a price to maintain these and in theory your code can run
 * faster.
 *
 * If C had a richer type-system, namely generic types, we could catch
 * all all potential type errors at compile time and potentially even
 * allow storing "values" larger than 64bits "inline" with a little
 * more magic.
 *
 * The most common things to use as keys are strings, integers, and
 * pointers (while common association values are strings, integers,
 * pointers and booleans).
 *
 * Our primary goal is to make collections convenient. Using typedef
 * and inline functions you can also make these safer at compile time.
 */

/**
 * @typedef value_t
 *
 * An un-typed union of integers, doubles, and pointers.
 */
typedef union {
  uint64_t u64;
  uint64_t i64;
  char* str;
  void* ptr;
  void* dbl;
} value_t;

/**
 * @enum non_fatal_error_code_t
 *
 * These are user recoverable errors and when a non-recoverable error
 * is returned, the state of the system should be left in a
 * recoverable state.
 */
typedef enum {
  NF_OK,
  NF_ERROR_NOT_FOUND,
  NF_ERROR_NOT_PARSED_AS_NUMBER,
  NF_ERROR_NOT_PARSED_AS_EXPECTED_ENUM,
} non_fatal_error_code_t;

/**
 * @typedef value_result_t
 *
 * A pair of a value_t and a non-fatal error code. When the error code
 * is set, the value_t should never be looked at (most likely will be
 * "zero" or a "nullptr" but you shouldn't trust that).
 */
typedef struct {
  union {
    uint64_t u64;
    uint64_t i64;
    char* str;
    void* ptr;
    void* dbl;
    value_t val;
  };
  // TODO(jawilson): change the name of the field after checking if
  // the compiler helps at all here!
  non_fatal_error_code_t nf_error;
} value_result_t;

#define boolean_to_value(x) ((value_t){.u64 = x})
#define u64_to_value(x) ((value_t){.u64 = x})
#define i64_to_value(x) ((value_t){.i64 = x})
#define str_to_value(x) ((value_t){.str = x})
#define ptr_to_value(x) ((value_t){.ptr = x})
#define dbl_to_value(x) ((value_t){.dbl = x})

/**
 * @function is_ok
 *
 * Return true if the given value_result_t contains a legal value
 * instead of an error condition.
 */
static inline boolean_t is_ok(value_result_t value) {
  return value.nf_error == NF_OK;
}

/**
 * @function is_not_ok
 *
 * Return true if the given value_result_t contains an error, such as
 * NF_ERROR_NOT_FOUND.
 */
static inline boolean_t is_not_ok(value_result_t value) {
  return value.nf_error != NF_OK;
}

#define cast(type, expr) ((type) (expr))

#endif /* _VALUE_H_ */
#line 2 "value-array.c"

/**
 * @file value-array.c
 *
 * This file contains a growable array of "values".
 *
 * Certain algorithms require that growth occurs based on the current
 * capacity of an array, not a fixed amount. For now we simply double
 * the current capacity when more space in the backing array is
 * required though we may scale this back to something more like 1.5X
 * for "large" arrays to save space.
 */

#ifndef _VALUE_ARRAY_H_
#define _VALUE_ARRAY_H_

struct value_array_S {
  uint32_t length;
  uint32_t capacity;
  value_t* elements;
};

/**
 * @typedef value_array_t
 *
 * A growable array of 64bit "values" (so integers, doubles, and
 * pointers).
 */
typedef struct value_array_S value_array_t;

extern value_array_t* make_value_array(uint32_t initial_capacity);
extern value_t value_array_get(value_array_t* array, uint32_t index);
extern void value_array_replace(value_array_t* array, uint32_t index,
                                value_t element);
extern void value_array_add(value_array_t* array, value_t element);
extern void value_array_push(value_array_t* array, value_t element);
extern value_t value_array_pop(value_array_t* array);
extern void value_array_insert_at(value_array_t* array, uint32_t position,
                                  value_t element);
extern value_t value_array_delete_at(value_array_t* array, uint32_t position);

#endif /* _VALUE_ARRAY_H_ */

/**
 * @function make_value_array
 *
 * Make a value array with the given initial capacity (which must be >
 * 0). When the array runs out of capacity because of calls to add,
 * push, etc., then the backing array is automatically increased.
 *
 * If either the initial_capacity is zero or if `malloc()` can't
 * allocate then a fata _error occurs.
 */
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

void value_array_ensure_capacity(value_array_t* array,
                                 uint32_t required_capacity) {
  if (array->capacity < required_capacity) {
    uint32_t new_capacity = array->capacity * 2;
    if (new_capacity < required_capacity) {
      new_capacity = required_capacity;
    }
    value_t* new_elements
        = (value_t*) (malloc_bytes(sizeof(value_t) * new_capacity));
    for (int i = 0; i < array->length; i++) {
      new_elements[i] = array->elements[i];
    }
    array->capacity = new_capacity;
    free_bytes(array->elements);
    array->elements = new_elements;
    return;
  }
}

/**
 * @function value_array_get
 *
 * Get the value stored at index `index`. If the index is outside of
 * the range of valid elements, then a fatal_error is signaled.
 */
value_t value_array_get(value_array_t* array, uint32_t index) {
  if (index < array->length) {
    return array->elements[index];
  }
  fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
#ifdef __TINYC__
  /* gcc and clang know fatal_error is _Noreturn but tcc doesn't */
  return (value_t){.u64 = 0};
#endif
}

/**
 * @function value_array_replace
 *
 * Replace the value at a given `index`. If the index is outside of
 * the range of valid elements, then a `fatal_error` is signaled.
 */
void value_array_replace(value_array_t* array, uint32_t index,
                         value_t element) {
  if (index < array->length) {
    array->elements[index] = element;
    return;
  }
  fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
}

/**
 * @function value_array_add
 *
 * Add an element to the end of an array. If more space is required
 * then the backing array is automatically resized. This resizing
 * means that a fatal_error() may occur if malloc() can not satisfy the
 * allocation request.
 */
void value_array_add(value_array_t* array, value_t element) {
  value_array_ensure_capacity(array, array->length + 1);
  array->elements[(array->length)++] = element;
}

/**
 * @function value_array_push
 *
 * This is a synonym for value_array_add which serves to make it more
 * obvious that the array is actually being used like a stack.
 */
void value_array_push(value_array_t* array, value_t element) {
  value_array_add(array, element);
}

/**
 * @function value_array_pop
 *
 * Returns the last element of the array (typically added via push)
 * and modifies the length of the array so that the value isn't
 * accessible any longer. (We also "zero out" the element in case you
 * are using a conservative garbage collector.)
 *
 * If the array is currently empty, then
 * `fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS)` is called.
 */
value_t value_array_pop(value_array_t* array) {
  if (array->length == 0) {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
  uint32_t last_index = array->length - 1;
  value_t result = value_array_get(array, last_index);
  array->elements[last_index] = u64_to_value(0);
  array->length--;
  return result;
}

/**
 * @function value_array_insert_at
 *
 * Insert an element into some existing position in the array (or at
 * the end if position == the current array length).
 *
 * This operation is not efficient for large arrays as potentially the
 * entire array must be moved to new locations (there are
 * data-structures like trees that can make this more efficient though
 * such a data-structure isn't in this library yet).
 *
 * If the position is > than the length length, then
 * fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.
 *
 * If memory allocation is required and malloc() fails, then
 * fatal_error(ERROR_MEMORY_ALLOCATION) is called.
 */
void value_array_insert_at(value_array_t* array, uint32_t position,
                           value_t element) {
  if (position == array->length) {
    value_array_add(array, element);
    return;
  }

  if (position > array->length) {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
    return;
  }

  value_array_ensure_capacity(array, array->length + 1);

  // This is the standard loop but we now need to use a signed index
  // because when the position is zero, zero - 1 is 0xffffffff which
  // is still greater than zero (and hence greater than position).
  for (int64_t i = array->length - 1; i >= position; i--) {
    array->elements[i + 1] = array->elements[i];
  }
  array->length++;
  array->elements[position] = element;
}

/**
 * @function value_array_delete_at
 *
 * Deletes the element at the given position (and return it so that it
 * can potentially be freed by the caller).
 *
 * If the position doesn't point to a valid element then
 * fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.
 */
value_t value_array_delete_at(value_array_t* array, uint32_t position) {
  value_t result = value_array_get(array, position);
  for (int i = position; i < array->length - 1; i++) {
    array->elements[i] = array->elements[i + 1];
  }
  array->length--;
  return result;
}
#endif /* C_ARMYKNIFE_LIB_IMPL */
