#ifndef __MACRO_H__
#define __MACRO_H__

#include "value.h"
#include "environment.h"

void    macro_set_rewriter(value_t name, value_t rewriter);
value_t macro_get_rewriter(value_t name);

binding_map_t* list_macro_rewriters(void);
value_t macro_expand(value_t expression);
#endif
