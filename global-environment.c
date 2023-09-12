#include <stdlib.h>

#include "environment.h"
#include "global-environment.h"
#include "primitive.h"

void add_basic_primtives(environment_t* env);

environment_t* make_global_environment() {
  environment_t* result = make_environment(NULL);
  add_basic_primtives(result);
  return result;
}

void add_basic_primtives(environment_t* env) {
  environment_define(env, "+",
                     tagged_reference(TAG_PRIMITIVE, &primtive_function_plus));
}
