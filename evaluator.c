#include <stdint.h>
#include <stdlib.h>

#include "evaluator.h"
#include "fatal-error.h"
#include "optional.h"
#include "pair.h"

#define HASHCODE_IF UINT64_C(12687957717205024595)
#define HASHCODE_SET_BANG 8292903574644452355
#define HASHCODE_QUOTE 10597478766694597373
#define HASHCODE_LAMBDA 11364329973434366565

tagged_reference_t eval(environment_t* env, tagged_reference_t expr) {

  // Handle self-evaluating values and variable lookups
  switch (expr.tag) {
  case TAG_NULL:
  case TAG_STRING:
  case TAG_UINT64_T:
  case TAG_ERROR_T:
    return expr;

  case TAG_READER_SYMBOL: {
    optional_t result = environment_get(env, (char*) expr.data);
    if (!optional_is_present(result)) {
      fatal_error(ERROR_VARIABLE_NOT_FOUND);
    }
    return optional_value(result);
  }
  }

  pair_t* lst = (pair_t*) expr.data;

  if (pair_list_length(lst) == 0) {
    return (tagged_reference_t){ERROR_CANT_EVAL_EMPTY_EXPRESSION, TAG_ERROR_T};
  }

  tagged_reference_t head = pair_list_get(lst, 0);
  if (head.tag == TAG_READER_SYMBOL) {
    uint64_t hashcode = string_hash(head.data);
    switch (hashcode) {
    case HASHCODE_IF:
      if (!string_equal(head.data, "if")) {
        break;
      }
      // TODO
      break;

    case HASHCODE_SET_BANG:
      // TODO
      break;
    }

    // TODO quote
    // TODO lambda
  }

  // perform an "application" (either macro or function call)

  tagged_reference_t fn = eval(env, head);
  pair_t* args = NULL;
  for (int i = 1; (i < pair_list_length(lst)); i++) {
    tagged_reference_t arg_expr = pair_list_get(lst, i);
    args = pair_list_append(args, make_pair(eval(env, arg_expr), NIL));
  }

  // TODO(jawilson): actually invoke the primitive or invoke a
  // closure.

  return NIL;
}
