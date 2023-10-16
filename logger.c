#line 2 "logger.c"

/**
 * A logger provides a controlled way to insert "print statements"
 * into code in order to gain insight into what a program is doing
 * (and is no harder to use than "printf" so kind of a no-brainer).
 *
 * If you've temporarily inserted print statements into your program,
 * you've probably already learned that logging is actually quite
 * expensive in terms of run-time performance. (For this
 * implementation, getting a timestamp is probably one kernel call and
 * doing the actual output, since logging is less useful when
 * buffered, requires at least another kernel call. Finally,
 * formatting strings for human readability is realitively expensive
 * itself. For example, printing a large number may require dozens or
 * hundreds of cycles while adding two numbers may take less than a
 * single cycle on a modern pipelined processor).
 *
 * Since logging is expensive, it is useful to be able to turn it on
 * and off (even without recompiling) which is done based on the
 * logging level. When logging is turned off, the cost is meant to be
 * equivalent to a load, compare, and branch though logging can effect
 * the C compiler's optimizations, so it is **not** recommended to be
 * left in critical loops. Obviously if the code is compiled into the
 * binary, even if the code to skip the logging doesn't considerably
 * increase run-time, it may still have an impact which we hope to
 * minimize in the future with additional options.
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
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdarg.h>

#define LOGGER_OFF 0
#define LOGGER_TRACE 1
#define LOGGER_DEBUG 2
#define LOGGER_INFO 3
#define LOGGER_WARN 4
#define LOGGER_FATAL 5

struct logger_state_S {
  boolean_t initialized;
  int level;
};

typedef struct logger_state_S logger_state_t;

// FIXME : should be LOGGER_WARN
logger_state_t global_logger_state = (logger_state_t){.level = LOGGER_TRACE};

extern void logger_init(void);

__attribute__((format(printf, 4, 5))) extern void
    logger_impl(char* file, int line_number, int level, char* format, ...);

/**
 * This will never ever log and should have essentially zero impact on
 * compilation (including detecting errors). In other words it should
 * behave like an empty statment ";".
 *
 * On the other hand, errors in these statements will not be caught
 * and therefore it can't be depended on to keep working as you
 * refactor code and decide later that you want to turn it on.
 */
#define log_nope(format, ...)                                                  \
  do {                                                                         \
  } while (0);

/**
 * This will never log however the compiler *should* still check to
 * make sure the code is legal and compiles. Any sufficiently smart
 * compiler with some level of optimization turned on should not
 * change it's code generation strategy at all if you leave one of these
 * statements in your source code and you should easily be able to
 * upgrade them to a real level later.
 */
#define log_off(format, ...) \
  do {                                                                         \
  if (0) { \
      logger_impl(__FILE__, __LINE__, LOGGER_TRACE, format, ##__VA_ARGS__);    \
  }
  } while (0);

/**
 * Log at the TRACE level using printf style formatting.
 */
#define log_trace(format, ...)                                                 \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_TRACE) {                           \
      logger_impl(__FILE__, __LINE__, LOGGER_TRACE, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

/**
 * Log at the DEBUG level using printf style formatting.
 */
#define log_debug(format, ...)                                                 \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_DEBUG) {                           \
      logger_impl(__FILE__, __LINE__, LOGGER_DEBUG, format, ##__VA_ARGS__);    \
    }                                                                          \
  } while (0)

/**
 * Log at the INFO level using printf style formatting.
 */
#define log_info(format, ...)                                                  \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_INFO) {                            \
      logger_impl(__FILE__, __LINE__, LOGGER_INFO, format, ##__VA_ARGS__);     \
    }                                                                          \
  } while (0)

/**
 * Log at the WARN level using printf style formatting.
 */
#define log_warn(format, ...)                                                  \
  do {                                                                         \
    if (global_logger_state.level <= LOGGER_WARN) {                            \
      logger_impl(__FILE__, __LINE__, LOGGER_WARN, format, ##__VA_ARGS__);     \
    }                                                                          \
  } while (0)

/**
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

/**
 * This routine modifies the logging level based on the environment
 * variable ARMYKNIFE_LIB_LOG_LEVEL (which currently must be a
 * number).
 *
 * While not required to actually use logging, the logging level will
 * be set to LOGGER_WARN unless you change it in a debugger, etc.
 */
void logger_init() {
  char* level_string = getenv("ARMYKNIFE_LIB_LOG_LEVEL");
  if (level_string != NULL) {
    uint64_t level = string_parse_uint64(level_string);
    global_logger_state.level = level;
  }
  global_logger_state.initialized = true;
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
 * This is the non macro version entry point into the logger. Normally
 * it wouldn't be called directly since it is inconvenient.
 */
__attribute__((format(printf, 4, 5))) 
void logger_impl(char* file, int line_number, int level, char* format, ...) {
  if (level >= global_logger_state.level) {
    fprintf(stderr, "%s ", logger_level_to_string(level));
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s:%d ", file, line_number);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
  }
}
