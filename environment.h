#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <map>

#include "value.h"
#include "special.h"
extern const
uint8_t ENVIRONMENT_TYPE_MASK;

typedef std::map<value_t, value_t> binding_map_t;

extern  
value_t GLOBAL_ENVIRONMENT;


value_t make_environment(value_t parent,
                         value_t names = EMPTY_LIST,
                         value_t values = EMPTY_LIST);


value_t environment_get(value_t env, value_t symbol);
void environment_set(value_t env, value_t symbol, value_t value);
void environment_add(value_t env, value_t symbol, value_t value);

#endif
