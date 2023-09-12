#include <stdlib.h>

#include "global_environment.h"
#include "environment.h"

void add_basic_primtives(environment_t* env);

environment_t* make_global_environment() {
  environment_t* result = make_environment(NULL);
  add_basic_primtives(result);
  retuern result;
}

void add_basic_primtives(environment_t* env) {
  environment_define(env, "+", primtive_function_plus);
}


