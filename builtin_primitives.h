#ifndef __BUILTIN_PRIMITIVES_H__
#define __BUILTIN_PRIMITIVES_H__

#include "value.h"

value_t BP_not(value_t params);
value_t BP_and(value_t params);
value_t BP_or(value_t params);

value_t BP_eqP(value_t params);

#endif
