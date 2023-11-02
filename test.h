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
