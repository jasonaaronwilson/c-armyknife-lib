/**
 * @file global-environment.c
 *
 * This file contains the routine to build a global environment
 * populated with typical scheme routines. (It won't be unusual to
 * have multiple global environments created while debugging or
 * assembling.)
 *
 * The debugger creates a child environment populated with debugger
 * functions and data which keeps the global environment "clean".
 */

#include <stdlib.h>

#include "environment.h"
#include "global-environment.h"
#include "primitive.h"

#define unimplemented(name)                                                    \
  do {                                                                         \
  } while (0)
#define written_in_scheme(name)                                                \
  do {                                                                         \
  } while (0)
#define not_a_primitive(name)                                                  \
  do {                                                                         \
  } while (0)
#define math_function(name)                                                    \
  do {                                                                         \
  } while (0)
#define io_function(name)                                                      \
  do {                                                                         \
  } while (0)

void add_basic_primtives(environment_t* env);

environment_t* make_global_environment() {
  environment_t* result = make_environment(NULL);
  add_basic_primtives(result);
  return result;
}

// See
// https://github.com/johnwcowan/r7rs-work/blob/master/SmallLanguageIdentifiers.md

// Note: all arithmetic operations are for *signed* integers. In order
// to support unsigned 64bit math, we need to add several unsigned:foo
// functions.

void add_basic_primtives(environment_t* env) {
  /* clang-format off */
  environment_define(env, "-",
                     tagged_reference(TAG_PRIMITIVE, &primtive_function_sub));
  environment_define(env, "*",
                     tagged_reference(TAG_PRIMITIVE, &primtive_function_sub));
  not_a_primitive("...");
  environment_define(env, "/",
                     tagged_reference(TAG_PRIMITIVE, &primtive_function_div));
  not_a_primitive("_");
  environment_define(env, "+",
                     tagged_reference(TAG_PRIMITIVE, &primtive_function_plus));
  unimplemented("<");
  unimplemented("<=");
  unimplemented("=");
  unimplemented("=>");
  unimplemented(">");
  unimplemented(">=");
  unimplemented("abs");
  math_function("acos");
  not_a_primitive("and");
  math_function("angle");
  written_in_scheme("append");
  written_in_scheme("apply");
  math_function("asin");
  written_in_scheme("assoc");
  written_in_scheme("assq");
  written_in_scheme("assv");
  math_function("atan");
  not_a_primitive("begin");
  io_function("binary-port?");
  unimplemented("/ boolean?");
  unimplemented("boolean=?");
  unimplemented("bytevector");
  unimplemented("bytevector?");
  unimplemented("bytevector-append");
  unimplemented("bytevector-copy");
  unimplemented("bytevector-copy!");
  unimplemented("bytevector-length");
  unimplemented("bytevector-u8-ref");
  unimplemented("bytevector-u8-set!");
  written_in_scheme("caaaar");
  written_in_scheme("caaadr");
  written_in_scheme("caaar");
  written_in_scheme("caadar");
  written_in_scheme("caaddr");
  written_in_scheme("caadr");
  written_in_scheme("caar");
  written_in_scheme("cadaar");
  written_in_scheme("cadadr");
  written_in_scheme("cadar");
  written_in_scheme("caddar");
  written_in_scheme("cadddr");
  written_in_scheme("caddr");
  written_in_scheme("cadr");
  unimplemented("call/cc");
  unimplemented("call-with-current-continuation");
  io_function("call-with-input-file");
  io_function("call-with-output-file");
  io_function("call-with-port");
  // call-with-values
  io_function("call-with-input-file");
  unimplemented("/ car");
  not_a_primitive("case");
  not_a_primitive("case-lambda");
  written_in_scheme("cdaaar");
  written_in_scheme("cdaadr");
  written_in_scheme("cdaar");
  written_in_scheme("cdadar");
  written_in_scheme("cdaddr");
  written_in_scheme("cdadr");
  written_in_scheme("cdar");
  written_in_scheme("cddaar");
  written_in_scheme("cddadr");
  written_in_scheme("cddar");
  written_in_scheme("cdddar");
  written_in_scheme("cddddr");
  written_in_scheme("cdddr");
  written_in_scheme("cddr");
  unimplemented("cdr");
  unimplemented("ceiling");
  unimplemented("char?");
  written_in_scheme("char<?");
  written_in_scheme("char<=?");
  written_in_scheme("char=?");
  written_in_scheme("char>?");
  written_in_scheme("char>=?");
  unimplemented("char->integer");
  written_in_scheme("char-alphabetic?");
  written_in_scheme("char-ci<?");
  written_in_scheme("char-ci<=?");
  written_in_scheme("char-ci=?");
  written_in_scheme("char-ci>?");
  written_in_scheme("char-ci>=?");
  written_in_scheme("char-downcase");
  written_in_scheme("char-foldcase");
  written_in_scheme("char-lower-case?");
  written_in_scheme("char-numeric?");
  io_function("char-ready?");
  written_in_scheme("char-upcase");
  written_in_scheme("char-upper-case?");
  written_in_scheme("char-whitespace?");
  io_function("close-input-port");
  io_function("close-output-port");
  io_function("close-port");
  math_function("command-line");
  math_function("complex?");
  not_a_primitive("cond");
  not_a_primitive("cond-expand");
  unimplemented("cons");
  math_function("cos");
  io_function("/ current-error-port");
  io_function("/ current-input-port");
  // current-jiffy
  io_function("current-output-port");
  // current-second
  not_a_primitive("define");
  // define-record-type
  not_a_primitive("define");
  not_a_primitive("define-syntax");
  not_a_primitive("define-values");
  // delay
  // delay-force
  io_function("delete-file");
  math_function("denominator");
  math_function("digit-value");
  io_function("display");
  not_a_primitive("do");
  not_a_primitive("dynamic-wind");
  not_a_primitive("else");
  unimplemented("emergency-exit");
  unimplemented("environment");
  io_function("eof-object");
  io_function("eof-object?");
  // eq?
  // equal?
  // eqv?
  // error
  // error-object?
  // error-object-irritants
  // error-object-message
  // eval
  // even?
  // exact
  // exact?
  // exact-integer?
  // exact-integer-sqrt
  // exit
  // exp
  // expt
  // features
  io_function("file-exists?");
  math_function("finite?");
  // floor
  // floor/
  // floor-quotient
  // floor-remainder
  io_function("flush-output-port");
  written_in_scheme("force");
  written_in_scheme("for-each");
  written_in_scheme("gcd");
  // get-environment-variable
  // get-environment-variables
  // get-output-bytevector
  // get-output-string
  // guard
  not_a_primitive("if");
  math_function("imag-part");
  // import
  // include
  // include-ci
  math_function("inexact");
  math_function("inexact");
  math_function("inexact?");
  math_function("infinite?");
  io_function("input-port?");
  io_function("input-port-open?");
  unimplemented("integer?");
  unimplemented("integer->char");
  // interaction-environment
  // interaction-environment
  // jiffies-per-second
  not_a_primitive("lambda");
  written_in_scheme("lcm");
  written_in_scheme("length");
  not_a_primitive("let");
  not_a_primitive("let*");
  not_a_primitive("let*-values");
  not_a_primitive("let*-values");
  not_a_primitive("let*-values");
  not_a_primitive("let*-values");
  not_a_primitive("let*-values");
  not_a_primitive("let*-values");
  // list
  // list?
  written_in_scheme("list->string");
  written_in_scheme("list->vector");
  written_in_scheme("list-copy");
  written_in_scheme("list-ref");
  written_in_scheme("list-set!");
  written_in_scheme("list-tail");
  io_function("load");
  // log
  // magnitude
  // make-bytevector
  // make-list
  // make-parameter
  // make-polar
  // make-promise
  // make-rectangular
  unimplemented("make-string");
  unimplemented("/ make-vector");
  written_in_scheme("map");
  written_in_scheme("max");
  written_in_scheme("member");
  written_in_scheme("memq");
  written_in_scheme("memv");
  written_in_scheme("min");
  // modulo
  math_function("nan?");
  written_in_scheme("negative?");
  io_function("newline");
  unimplemented("not");
  unimplemented("null?");
  written_in_scheme("number?");
  unimplemented("number->string");
  // numerator
  // odd?
  io_function("open-binary-input-file");
  io_function("open-binary-output-file");
  io_function("open-input-bytevector");
  io_function("open-input-file");
  io_function("open-input-string");
  io_function("open-output-bytevector");
  io_function("open-output-file");
  io_function("open-output-string");
  not_a_primitive("or");
  io_function("output-port?");
  io_function("output-port-open?");
  // pair?
  // parameterize
  io_function("peek-char");
  io_function("peek-u8");
  io_function("port?");
  // positive?
  // procedure?
  not_a_primitive("quasiquote");
  not_a_primitive("quote");
  math_function("quotient");
  // raise
  // raise-continuable
  math_function("rational?");
  math_function("rationalize");
  math_function("read");
  math_function("read-bytevector");
  math_function("read-bytevector!");
  math_function("read-char");
  math_function("read-line");
  math_function("read-string");
  math_function("read-u8");
  math_function("real?");
  math_function("real-part");
  // remainder
  written_in_scheme("reverse");
  // round
  // scheme-report-environment
  // set!
  // set-car!
  // set-cdr!
  math_function("sin");
  math_function("sqrt");
  written_in_scheme("square");
  // string
  // string?
  written_in_scheme("string<?");
  written_in_scheme("string<=?");
  // string=?
  written_in_scheme("string>?");
  written_in_scheme("string>=?");
  written_in_scheme("string->list");
  // string->number
  // string->symbol
  // string->utf8
  written_in_scheme("string->vector");
  // string-append
  written_in_scheme("string-ci<?");
  written_in_scheme("string-ci<=?");
  written_in_scheme("string-ci=?");
  written_in_scheme("string-ci>?");
  written_in_scheme("string-ci>=?");
  written_in_scheme("string-copy");
  written_in_scheme("string-copy!");
  written_in_scheme("string-downcase");
  written_in_scheme("string-fill!");
  written_in_scheme("string-foldcase");
  written_in_scheme("string-for-each");
  // string-length
  written_in_scheme("string-map");
  // string-ref
  // string-set!
  written_in_scheme("string-upcase");
  written_in_scheme("substring");
  // symbol?
  // symbol=?
  // symbol->string
  not_a_primitive("syntax-error");
  not_a_primitive("syntax-rules");
  math_function("tan");
  io_function("textual-port?");
  math_function("truncate");
  math_function("truncate/");
  math_function("truncate-quotient");
  math_function("truncate-remainder");
  io_function("u8-ready?");
  not_a_primitive("unless");
  not_a_primitive("unquote");
  not_a_primitive("unquote-splicing");
  // utf8->string
  // values
  // vector
  // vector?
  written_in_scheme("vector->list");
  // vector->string
  written_in_scheme("vector-append");
  written_in_scheme("vector-copy");
  written_in_scheme("vector-copy!");
  written_in_scheme("vector-fill!");
  written_in_scheme("vector-for-each");
  // vector-length
  written_in_scheme("vector-map");
  // vector-ref
  // vector-set!
  not_a_primitive("when");
  // with-exception-handler
  io_function("with-input-from-file");
  io_function("with-output-to-file");
  io_function("write");
  io_function("write-bytevector");
  io_function("write-char");
  io_function("write-shared");
  io_function("write-simple");
  io_function("write-string");
  io_function("write-u8");
  written_in_scheme("zero?");

  /* clang-format on */

  // ==========================================================================
  // Some additional primitives so that we can write primitives in scheme
  // ==========================================================================
  /*
  environment_define(env, "comet-vm:get-tag",
                     tagged_reference(TAG_PRIMITIVE,
  &primtive_function_get_tag));
  */
}
