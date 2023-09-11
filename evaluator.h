#ifndef _EVALUATOR_T_
#define _EVALUATOR_T_

#include "environment.h"
#include "tagged-reference.h"

extern tagged_reference_t eval(environment_t* env, tagged_reference_t expr);

#endif /* _EVALUATOR_T_ */
