#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "pair.h"
#include "tagged-reference.h"

typedef tagged_reference_t (*primitive_t)(pair_t* args);

extern tagged_reference_t primtive_function_plus(pair_t* args);

#endif /* _PRIMITIVE_H_ */
