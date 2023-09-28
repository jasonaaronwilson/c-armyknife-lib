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
