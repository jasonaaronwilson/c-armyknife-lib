#line 2 "compound-literal.c"
/**
 * @file compound-literal.c
 *
 * This file exists solely for interfacing with early versions of the
 * omni-c compiler where it is temporarily more convenient to have a
 * keyword. This is similar to usage of cast() macro (though in the
 * case of cast, sometimes it's a lot easier to read because of all
 * the parens in the cast syntax and following expression).
 */

// ======================================================================
// This section is extraced to compound-literal.h
// ======================================================================

#ifndef _COMPOUND_LITERAL_H_
#define _COMPOUND_LITERAL_H_

#define compound_literal(type, initializer) ((type) initializer)

#endif /* _COMPOUND_LITERAL_H_ */
