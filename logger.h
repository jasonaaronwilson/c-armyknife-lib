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
