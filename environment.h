#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "boolean.h"
#include "pair.h"
#include "symbol-table.h"

typedef struct environment_S {
  struct environment_S* parent;
  pair_t* bindings;
  symbol_table_t symbols;
} environment_t;

extern environment_t* make_environment();
extern boolean_t environment_has_binding(environment_t* env, char* var_name);
extern tagged_reference_t environment_get(environment_t* env, char* var_name);
extern void environment_set(environment_t* env, char* var_name,
                            tagged_reference_t value);
extern void environment_define(environment_t* env, char* var_name,
                               tagged_reference_t value);

#endif /* _ENVIRONMENT_H_ */
