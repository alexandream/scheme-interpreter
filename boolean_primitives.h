#ifndef __BOOLEAN_PRIMITIVES_H__
#define __BOOLEAN_PRIMITIVES_H__

#include "value.h"

value_t boolean_primitive_not(value_t params);
value_t boolean_primitive_and(value_t params);
value_t boolean_primitive_or(value_t params);

#endif
