/**
 * @file evaluator.c
 *
 * The file contains an interpreter for a very tiny subset of scheme
 * (for example there are only 64bit unsigned numbers right now...)
 * which is used to implement the debugger repl as well as expressions
 * in the assembler. See reader.c for a parser that reads textual and
 * converts it to the format suitable for expr (aka linked lists built
 * out of pairs plus various "atoms".
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "closure.h"
#include "evaluator.h"
#include "fatal-error.h"
#include "optional.h"
#include "pair.h"
#include "primitive.h"
#include "scheme-symbol.h"
#include "string-util.h"

// See the symbol-hash command line tool in this directory if you need
// to generate more. Example make symbol-hash && ./symbol-hash 'if'
// 'set!' 'quote' 'lambda'.

#define HASHCODE_IF UINT64_C(12687957717205024595)
#define HASHCODE_SET_BANG UINT64_C(8292903574644452355)
#define HASHCODE_QUOTE UINT64_C(10597478766694597373)
#define HASHCODE_LAMBDA UINT64_C(11364329973434366565)
#define HASHCODE_AND UINT64_C(11364329973434366565)
#define HASHCODE_OR UINT64_C(11364329973434366565)

tagged_reference_t eval_if_expression(environment_t* env,
                                      tagged_reference_t expr);
tagged_reference_t eval_assignment(environment_t* env, tagged_reference_t expr);
tagged_reference_t eval_application(environment_t* env,
                                    tagged_reference_t expr);
tagged_reference_t eval_lambda(environment_t* env, tagged_reference_t expr);

/**
 * This is the entry point to the evaluator. Dvaluate the given
 * expression and return a tagged_reference_t to the result of
 * interpreting it.
 */
tagged_reference_t eval(environment_t* env, tagged_reference_t expr) {

  // Handle self-evaluating values and variable lookups
  switch (expr.tag) {
  case TAG_NULL:
  case TAG_STRING:
  case TAG_UINT64_T:
  case TAG_ERROR_T:
    return expr;

  case TAG_SCHEME_SYMBOL: {
    optional_t result = environment_get(env, (char*) expr.data);
    if (!optional_is_present(result)) {
      fatal_error(ERROR_VARIABLE_NOT_FOUND);
    }
    return optional_value(result);
  }
  }

  pair_t* lst = untag_pair(expr);

  if (pair_list_length(lst) == 0) {
    return (tagged_reference_t){ERROR_CANT_EVAL_EMPTY_EXPRESSION, TAG_ERROR_T};
  }

  tagged_reference_t first = pair_list_get(lst, 0);
  if (first.tag == TAG_SCHEME_SYMBOL) {
    char* symbol_name = untag_reader_symbol(first);
    // We probably don't need all 64 bits and with so few special
    // forms, just doing a chain of sring_equal() calls may be faster
    // or nearly as fast though there is probably a break even point
    // where this is much faster...
    uint64_t hashcode = string_hash(symbol_name);
    switch (hashcode) {
    case HASHCODE_IF:
      if (!string_equal(symbol_name, "if")) {
        break;
      }
      return eval_if_expression(env, expr);
      break;

    case HASHCODE_SET_BANG:
      if (!string_equal(symbol_name, "set!")) {
        break;
      }
      return eval_assignment(env, expr);

    case HASHCODE_QUOTE:
      return pair_list_get(lst, 1);

    case HASHCODE_LAMBDA:
      return eval_lambda(env, expr);

#if 0
      // TODO
    case HASHCODE_AND:
    case HASHCODE_OR:
#endif /* 0 */
    }
  }

  return eval_application(env, expr);

  return NIL;
}

tagged_reference_t eval_if_expression(environment_t* env,
                                      tagged_reference_t expr) {
  pair_t* lst = untag_pair(expr);
  tagged_reference_t test_expr = pair_list_get(lst, 1);
  tagged_reference_t consequent_expr = pair_list_get(lst, 2);
  tagged_reference_t alternative_expr = NIL;
  if (pair_list_length(lst) >= 3) {
    alternative_expr = pair_list_get(lst, 3);
  }
  tagged_reference_t evaluated_expr = eval(env, test_expr);
  if (is_false(evaluated_expr)) {
    return eval(env, alternative_expr);
  } else {
    return eval(env, consequent_expr);
  }
}

tagged_reference_t eval_assignment(environment_t* env,
                                   tagged_reference_t expr) {
  // fixme
  return NIL;
}

/**
 * Evaluate and application, i.e., a function call.
 */
tagged_reference_t eval_application(environment_t* env,
                                    tagged_reference_t expr) {
  primitive_arguments_t arguments = {.n_args = 0};

  // The above should be sufficient but just clear the entire
  // structure while we are still in early development.
  memset(&arguments, 0, sizeof(arguments));

  // perform an "application" (aka, function call to a primitive or
  // closure).

  pair_t* lst = untag_pair(expr);

  tagged_reference_t fn = eval(env, pair_list_get(lst, 0));

  for (int i = 1; (i < pair_list_length(lst)); i++) {
    if (i >= MAX_PRIMITIVE_ARGS) {
      fatal_error(ERROR_MAX_PRIMITIVE_ARGS);
    }
    tagged_reference_t arg_expr = pair_list_get(lst, i);
    arguments.args[arguments.n_args++] = eval(env, arg_expr);
  }

  if (fn.tag == TAG_PRIMITIVE) {
    primitive_t primitive = untag_primitive(fn);
    return primitive(arguments);
  }

  // Must be a closure.
  closure_t* closure = untag_closure_t(fn);
  env = make_environment(closure->env);
  // make sure number of args are compatible.
  for (int i = 0; (i < closure->n_arg_names); i++) {
    environment_define(env, closure->arg_names[i], arguments.args[i]);
  }

  pair_t* sequence = untag_pair(closure->code);
  while (sequence->tail.tag != TAG_NULL) {
    eval(env, sequence->head);
    sequence = untag_pair(sequence->tail);
  }

  // Hopefully do a tail call to evaluate the last element of the
  // lambda expression's body;
  return eval(env, sequence->head);
}

/**
 * Make a closure
 */
tagged_reference_t eval_lambda(environment_t* env, tagged_reference_t expr) {
  pair_t* argument_list = untag_pair(pair_list_get(untag_pair(expr), 1));
  uint64_t n_args = pair_list_length(argument_list);
  closure_t* closure = allocate_closure(n_args);
  closure->code = cdr(cdr(expr));
  closure->env = env;
  closure->debug_name = NULL;
  closure->n_arg_names = n_args;
  for (int i = 0; i < n_args; i++) {
    closure->arg_names[i]
        = untag_scheme_symbol(pair_list_get(argument_list, i));
  }

  return tagged_reference(TAG_CLOSURE_T, closure);
}
