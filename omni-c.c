#line 2 "omni-c.c"

#ifndef _OMNI_C_H_
#define _OMNI_C_H_

/**
 * @file omni-c.c
 *
 * These macros help c-armyknife-lib conform to what omni-c can
 * (currently) handle. If you are writing code within c-armyknife-lib,
 * then you must use these macros but if you are just using
 * c-amryknife-lib within your own C project, then you can ignore
 * these macros unless you strictly want to use them.
 */

/**
 * @macro cast
 *
 * Perform an unsafe cast of expr to the given type. This is much
 * easier for the omni-c compiler to handle because we know to parse a
 * type as the first argument even if we don't know what all the types
 * are yet. While omni-c uses this macro, if you are using plain C,
 * you don't have to use it.
 */
#define cast(type, expr) ((type) (expr))

#endif /* _OMNI_C_H_ */
