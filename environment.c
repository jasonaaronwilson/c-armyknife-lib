#include <stdlib.h>

#include "allocate.h"
#include "boolean.h"
#include "environment.h"
#include "tagged-reference.h"

environment_t* make_environment() { return malloc_struct(environment_t); }

boolean_t environment_has_binding(environment_t* env, char* var_name) {
  return false;
}

tagged_reference_t environment_get(environment_t* env, char* var_name) {
  return NIL;
}

void environment_set(environment_t* env, char* var_name,
                     tagged_reference_t value) {}

void environment_define(environment_t* env, char* var_name,
                        tagged_reference_t value) {}
