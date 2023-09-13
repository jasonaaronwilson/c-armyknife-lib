#include <stdlib.h>

#include "allocate.h"
#include "boolean.h"
#include "closure.h"
#include "environment.h"
#include "optional.h"
#include "pair.h"
#include "string-util.h"
#include "tagged-reference.h"

#define GLOBAL_ENVIRONMENT_BUCKETS 73
#define NESTED_ENVIRONMENT_BUCKETS 1

/**
 * Make an empty environment with the given parent. When parent is
 * NULL, the environment is optimized for lookup performance over
 * space.
 */
environment_t* make_environment(environment_t* parent) {
  // The global environment gets more buckets since it will have way
  // more symbols than a child environment. We can also avoid hashing
  // when n_buckets == 1.
  int n_buckets = (parent == NULL) ? 1 : 73;
  environment_t* result
      = (environment_t*) malloc_bytes(sizeof(environment_t) + n_buckets * 8);
  result->parent = parent;
  result->n_buckets = n_buckets;
  return result;
}

/**
 * Perform a lookup in the environment returning optional_empty() when
 * the var_name isn't found and otherwise the tagged_reference_t which
 * is bound to var_name (which can be NIL, etc.).
 */
optional_t environment_get(environment_t* env, char* var_name) {
  if (!env) {
    return optional_empty();
  }

  tagged_reference_t lst = NIL;
  if (env->n_buckets > 1) {
    uint64_t hash_code = string_hash(var_name);
    uint64_t bucket_number = hash_code % env->n_buckets;
    lst = env->buckets[bucket_number];
  } else {
    lst = env->buckets[0];
  }

  if (lst.tag == TAG_NULL) {
    return environment_get(env->parent, var_name);
  }

  // HERE: must be an "association" list. check the tag

  optional_t lookup_result = pair_assoc_list_lookup(untag_pair(lst), var_name);
  if (optional_is_present(lookup_result)) {
    return lookup_result;
  }

  return environment_get(env->parent, var_name);
}

// TODO(jawilson): implement!
void environment_set(environment_t* env, char* var_name,
                     tagged_reference_t value) {}

/**
 * Define a new variable in this environment (possibly shadowing an
 * older binding).
 */
void environment_define(environment_t* env, char* var_name,
                        tagged_reference_t value) {

  // Total hack to put a debug name on closures defined at the
  // top-level. This is completely optional behavior as the debug name
  // is not directly accessible.
  if (env->parent == NULL) {
    if (value.tag == TAG_CLOSURE_T) {
      closure_t* closure = untag_closure_t(value);
      if (closure->debug_name == NULL) {
        closure->debug_name = var_name;
      }
    }
  }

  // TODO(jawilson): reuse the older binding if redefining...
  uint64_t hash_code = string_hash(var_name);
  uint64_t bucket_number = hash_code % env->n_buckets;
  tagged_reference_t binding
      = cons(tagged_reference(TAG_STRING, var_name), value);
  env->buckets[bucket_number] = cons(binding, env->buckets[bucket_number]);
}
