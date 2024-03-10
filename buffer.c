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

extern void buffer_clear(buffer_t* buffer);

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
 * @function buffer_length
 *
 * Clear a buffer without resizing it.
 */
void buffer_clear(buffer_t* buffer) {
  for (int i = 0; i < buffer->capacity; i++) {
    buffer->elements[i] = 0;
  }
  buffer->length = 0;
}

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
