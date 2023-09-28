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
    exit(1);                                                                   \
  } while (0)

#endif /* _TEST_H_ */

// ======================================================================
// In order to not increase the runtime footprint, we don't actually
// have any non-macros (or inlined functions).
// ======================================================================
