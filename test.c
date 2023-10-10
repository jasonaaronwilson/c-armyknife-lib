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
