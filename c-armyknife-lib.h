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

// SSCF generated file from: fatal-error.c

#line 19 "fatal-error.c"
#ifndef _FATAL_ERROR_H_
#define _FATAL_ERROR_H_

/**
 * @constants error_code_t
 */
typedef enum {
  ERROR_UKNOWN,
  ERROR_ACCESS_OUT_OF_BOUNDS,
  ERROR_BAD_COMMAND_LINE,
  ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER,
  ERROR_ILLEGAL_ENUM_VALUE,
  ERROR_ILLEGAL_INITIAL_CAPACITY,
  ERROR_ILLEGAL_NULL_ARGUMENT,
  ERROR_ILLEGAL_ZERO_HASHCODE_VALUE,
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
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);

/**
 * @macro fatal_error
 *
 * Terminates the program with a fatal error.
 */
#define fatal_error(code) fatal_error_impl(__FILE__, __LINE__, code)

#endif /* _FATAL_ERROR_H_ */
// SSCF generated file from: boolean.c

#line 13 "boolean.c"
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
// SSCF generated file from: value.c

#line 3 "value.c"
#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdint.h>

/**
 * @file value.c
 *
 * A major part of the armyknife library is giving basic "collection"
 * capabilities to C which is why we have values but it also turns out
 * the library seems more consistent if a few non collection based
 * routines also use a similar interface to communicate "failures"
 * with the caller hence optional_value_result_t's are used as the
 * result of string functions. A mental model is that
 * optional_value_result_t can only result from "query" kinds of
 * operations that can return before changing the global state (OR
 * they can restore the state to the same condition, unlikely for many
 * failures which is why we have fatal-error.c which currrently just
 * terminates the program because that is the safest thing to do in
 * most circumstance. If you attempt to look past the end of an array,
 * that is a fatal error though "trying" to parse an integer, that is
 * an optional_value_result_t.)
 *
 * value_t's are typically only passed into functions while
 * optional_value_result_t's are then typically returned from
 * functions. When a optional_value_result_t is returned you must
 * therefore explictly check the error conditions and while you should
 * always do this, I don't know how to make the C compiler agree that
 * you at least kind of pretended to look at the error code which is
 * something that standard C could do but wouldn't do without
 * pressure, aka, common use ).
 *
 * value_t's and optional_value_result_t's carry no type information
 * that can be queried at runtime and by their nature, most C
 * compilers are going to do a very incomplete job of statically type
 * checking these. For example you can easily put a double into a
 * collection and successfully get back a very suspicious pointer and
 * the compiler will not warn you about this. On the positive side,
 * this also means you haven't paid a price to prevent these errors at
 * runtime and so in theory your code can run faster. If C had a
 * richer type-system, I think we could catch all such errors at
 * compile time (and this would not make code any slower than this
 * approach since by definition, static compilation errors will not
 * change how correct code runs).
 *
 * Both value_t's and optional_value_result_t's have a fixed sized. On
 * a native 64bit architecture, a value_t can be passed in a single
 * register and optional_value_result_t can be returned in two
 * registers and while I have no comparitive benchmarks to prove this
 * strategy is in fact efficient, one has to wonder why this was put
 * into most 64bit C calling conventions (aka ABIs) if no one was
 * actually trying to make this efficient.
 *
 * value_t's are meant only to used used when putting associations
 * into a collection where you have now given up on having static type
 * checking in standard C because C lacks generic types. You gain
 * functionality without embracing inheritance or a feature not
 * supported by standard C but you lose quite a bit of type safety.
 *
 * While a value_t can't hold *everything* it can hold a pointer to
 * almost anything so it's a bit like Java's notion of a primitive
 * value plus Java's notion of a pointer mixed into a very unsafe
 * union. If this seems unsafe, which of course it is, I have some
 * news for you if you think C is ever safe.
 *
 * The most common things to key over are strings, integers, and
 * pointers (while common values are strings, integers, pointers and
 * booleans). We make these convient but not quite type safe though
 * you can make things safer using typedef and inline functions for
 * the pointer case if your are willing to take that step.
 *
 * Sometimes other techniques like unit testing work quite well at
 * finding these errors as well as higher level bugs so we recommend
 * that and even provide a very simple framework to do that.
 */

typedef union {
  uint64_t u64;
  uint64_t i64;
  char* str;
  void* ptr;
  void* dbl;
} value_t;

/**
 * Non fatal errors.
 *
 * These may only occur as part of query interface.
 */
typedef enum {
  NF_OK,
  NF_ERROR_NOT_FOUND,
  NF_ERROR_NOT_PARSED_AS_NUMBER,
  NF_ERROR_NOT_PARSED_AS_EXPECTED_ENUM,
} non_fatal_error_code_t;

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
extern uint64_t string_to_uint64(const char* str);
extern char* uint64_to_string(uint64_t number);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern value_result_t string_parse_uint64(const char* string);
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

#line 13 "buffer.c"
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

#line 9 "value-array.c"
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
extern void value_array_push(value_array_t* array, value_t element);
extern value_t value_array_pop(value_array_t* array);
extern void value_array_insert_at(value_array_t* array, uint32_t position,
                                  value_t element);
extern value_t value_array_delete_at(value_array_t* array, uint32_t position);

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

#line 19 "string-tree.c"
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
// SSCF generated file from: command-line-parser.c

#line 12 "command-line-parser.c"
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

#endif /* _TEST_H_ */
#ifdef C_ARMYKNIFE_LIB_IMPL
#line 2 "allocate.c"

/**
 * @file allocate.c
 *
 * This file contains wrappers around malloc and free to make them
 * more convenient and possibly safer (for example, allocated memory
 * is always zero'd and macros like malloc_struct are more readable
 * besides the clearing behavior).
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
 * or deallocation the lossy hashtable is examined to see if the
 * padding bytes have been perturbed which makes it possible to find
 * some memory overwrite errors earlier than waiting for the free call
 * (and even if the memory isn't freed.
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
 * Allocate amount bytes or cause a fatal error. The memory is also
 * zeroed.
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
 * Provides a simple typdef and true/false which sometimes makes code
 * more readable.
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
/**
 * @file buffer.c
 *
 * A growable array of bytes. These are useful for many purposes such
 * as building large strings.
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
#line 2 "fatal-error.c"
/**
 * @file fatal-error.c
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

/**
 * @constants error_code_t
 */
typedef enum {
  ERROR_UKNOWN,
  ERROR_ACCESS_OUT_OF_BOUNDS,
  ERROR_BAD_COMMAND_LINE,
  ERROR_DYNAMICALLY_SIZED_TYPE_ILLEGAL_IN_CONTAINER,
  ERROR_ILLEGAL_ENUM_VALUE,
  ERROR_ILLEGAL_INITIAL_CAPACITY,
  ERROR_ILLEGAL_NULL_ARGUMENT,
  ERROR_ILLEGAL_ZERO_HASHCODE_VALUE,
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
} error_code_t;

extern _Noreturn void fatal_error_impl(char* file, int line, int error_code);
extern const char* fatal_error_code_to_string(int error_code);

/**
 * @macro fatal_error
 *
 * Terminates the program with a fatal error.
 */
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

#include <stdint.h>

__attribute__((warn_unused_result)) extern buffer_t*
    buffer_append_file_contents(buffer_t* bytes, char* file_name);
extern void buffer_write_file(buffer_t* bytes, char* file_name);

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
 * exanmple on the output binary size.
 *
 * The default log level is "WARN" though it is possible to override
 * this with #define ARMYKNIFE_LIB_DEFAULT_LOG_LEVEL <level> when
 * including the library implementation (or from your build command
 * which allows C preprocessor definitions to be injected into your
 * source code, one reason you may want a debug vs production builds).
 *
 * Additionally, when the first log statement is encountered, we
 * examine the envrionment variable named ARMYKNIFE_LIB_LOG_LEVEL if
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
 * isn't an intial focus of this library.
 *
 * [^1]: For this implementatic, getting a timestamp is probably one
 * kernel call and doing the actual output, since logging is less
 * useful when buffered, requires at least another kernel
 * call. Finally, formatting strings for human readability is
 * realitively expensive itself. For example, printing a large number
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
      }
    }
  }

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
 * Return a random `uint64` that is below some maximum. As much as the
 * underlying random number generartor allows, this should be uniform.
 */
uint64_t random_next_uint64_below(random_state_t* state, uint64_t maximum) {
  if (maximum == 0) {
    fatal_error(ERROR_ILLEGAL_ARGUMENT);
  }
#if 0
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
 * Delete the association of key (if it exists in the tree).
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
extern uint64_t string_to_uint64(const char* str);
extern char* uint64_to_string(uint64_t number);
extern uint64_t string_hash(const char* str);
extern char* string_substring(const char* str, int start, int end);
extern value_result_t string_parse_uint64(const char* string);
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
  boolean_t at_least_one_number = false;
  uint64_t integer = 0;
  uint64_t digit;

  while (*string != '\0') {
    digit = *string - '0';
    integer = integer * 10 + digit;
    string++;
    at_least_one_number = true;
  }

  return (value_result_t){
      .u64 = integer,
      .nf_error = at_least_one_number ? NF_OK : NF_ERROR_NOT_PARSED_AS_NUMBER};
}

value_result_t string_parse_uint64_hex(const char* string) {
  boolean_t at_least_one_number = false;
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
    at_least_one_number = true;
  }

  return (value_result_t){
      .u64 = integer,
      .nf_error = at_least_one_number ? NF_OK : NF_ERROR_NOT_PARSED_AS_NUMBER};
}

value_result_t string_parse_uint64_bin(const char* string) {
  boolean_t at_least_one_number = false;
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
    at_least_one_number = true;
  }

  return (value_result_t){
      .u64 = integer,
      .nf_error = at_least_one_number ? NF_OK : NF_ERROR_NOT_PARSED_AS_NUMBER};
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
    return string_parse_uint64_hex(string);
  } else if (string_starts_with(string, "0b")) {
    return string_parse_uint64_bin(string);
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
    n_bytes = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
  } while (0);

  if (n_bytes < sizeof(buffer)) {
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

#endif /* _TEST_H_ */

/**
 * @function test_fail_and_exit
 *
 * Set a break-point here to debug a test but normally you will use
 * the macro `test-fail` since it is much more convenient.
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
 * @function tokenize
 *
 * Tokenize a string.
 *
 * Delimiters terminate the current token and are thrown away. The
 * delimiters string is treated as a sequence of delimiter characters,
 * it does not mean a delimiter can be multiple characters.
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

// Add a *copy* of the string named data to the token list.
void add_duplicate(value_array_t* token_array, const char* data) {
  value_array_add(token_array, str_to_value(string_duplicate(data)));
}
#line 2 "uint64.c"
/**
 * @file uint64.c
 *
 * Implement a couple of useful operations on uint64_t (which can
 * often be used for smaller types.
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
 * capabilities to C which is why we have values but it also turns out
 * the library seems more consistent if a few non collection based
 * routines also use a similar interface to communicate "failures"
 * with the caller hence optional_value_result_t's are used as the
 * result of string functions. A mental model is that
 * optional_value_result_t can only result from "query" kinds of
 * operations that can return before changing the global state (OR
 * they can restore the state to the same condition, unlikely for many
 * failures which is why we have fatal-error.c which currrently just
 * terminates the program because that is the safest thing to do in
 * most circumstance. If you attempt to look past the end of an array,
 * that is a fatal error though "trying" to parse an integer, that is
 * an optional_value_result_t.)
 *
 * value_t's are typically only passed into functions while
 * optional_value_result_t's are then typically returned from
 * functions. When a optional_value_result_t is returned you must
 * therefore explictly check the error conditions and while you should
 * always do this, I don't know how to make the C compiler agree that
 * you at least kind of pretended to look at the error code which is
 * something that standard C could do but wouldn't do without
 * pressure, aka, common use ).
 *
 * value_t's and optional_value_result_t's carry no type information
 * that can be queried at runtime and by their nature, most C
 * compilers are going to do a very incomplete job of statically type
 * checking these. For example you can easily put a double into a
 * collection and successfully get back a very suspicious pointer and
 * the compiler will not warn you about this. On the positive side,
 * this also means you haven't paid a price to prevent these errors at
 * runtime and so in theory your code can run faster. If C had a
 * richer type-system, I think we could catch all such errors at
 * compile time (and this would not make code any slower than this
 * approach since by definition, static compilation errors will not
 * change how correct code runs).
 *
 * Both value_t's and optional_value_result_t's have a fixed sized. On
 * a native 64bit architecture, a value_t can be passed in a single
 * register and optional_value_result_t can be returned in two
 * registers and while I have no comparitive benchmarks to prove this
 * strategy is in fact efficient, one has to wonder why this was put
 * into most 64bit C calling conventions (aka ABIs) if no one was
 * actually trying to make this efficient.
 *
 * value_t's are meant only to used used when putting associations
 * into a collection where you have now given up on having static type
 * checking in standard C because C lacks generic types. You gain
 * functionality without embracing inheritance or a feature not
 * supported by standard C but you lose quite a bit of type safety.
 *
 * While a value_t can't hold *everything* it can hold a pointer to
 * almost anything so it's a bit like Java's notion of a primitive
 * value plus Java's notion of a pointer mixed into a very unsafe
 * union. If this seems unsafe, which of course it is, I have some
 * news for you if you think C is ever safe.
 *
 * The most common things to key over are strings, integers, and
 * pointers (while common values are strings, integers, pointers and
 * booleans). We make these convient but not quite type safe though
 * you can make things safer using typedef and inline functions for
 * the pointer case if your are willing to take that step.
 *
 * Sometimes other techniques like unit testing work quite well at
 * finding these errors as well as higher level bugs so we recommend
 * that and even provide a very simple framework to do that.
 */

typedef union {
  uint64_t u64;
  uint64_t i64;
  char* str;
  void* ptr;
  void* dbl;
} value_t;

/**
 * Non fatal errors.
 *
 * These may only occur as part of query interface.
 */
typedef enum {
  NF_OK,
  NF_ERROR_NOT_FOUND,
  NF_ERROR_NOT_PARSED_AS_NUMBER,
  NF_ERROR_NOT_PARSED_AS_EXPECTED_ENUM,
} non_fatal_error_code_t;

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

#endif /* _VALUE_H_ */
#line 2 "value-array.c"

/**
 * @file value-array.c
 *
 * This file contains a growable array of values/pointers.
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
 * push, etc., then the backing array is automatically doubled in size
 * (this may change to a different fraction for "large arrays"
 * (greater than say 250 elements) in the future to save space).
 *
 * If the initial_capacity is zero or if malloc() can't allocate
 * everything, then a fatal_error() occurs.
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
 * Returns the last element of the array (typically added via push).
 *
 * If the array is currently empty, then
 * fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.
 */
value_t value_array_pop(value_array_t* array) {
  if (array->length == 0) {
    fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS);
  }
  uint32_t last_index = array->length - 1;
  value_t result = value_array_get(array, last_index);
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
