#ifndef __MACRO_H__
#define __MACRO_H__

#include "value.h"

void    macro_set_rewriter(value_t name, value_t rewriter);
value_t macro_get_rewriter(value_t name);

value_t macro_expand(value_t expression);
#endif
