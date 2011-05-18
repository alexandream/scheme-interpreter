#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "value.h"


struct environment_t;

extern  
environment_t* GLOBAL_ENVIRONMENT;

environment_t * make_environment(environment_t *parent);

value_t environment_get(environment_t *env, value_t symbol);
void environment_set(environment_t *env, value_t symbol, value_t value);

#endif
