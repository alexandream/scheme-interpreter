#ifndef __BUILTIN_PRIMITIVES_H__
#define __BUILTIN_PRIMITIVES_H__

#include "value.h"
#include "evaluator.h"

void BP_not(context_t* context);
void BP_and(context_t* context);
void BP_or(context_t* context);

void BP_eqP(context_t* context);

void BP_cons(context_t* context);
void BP_car(context_t* context);
void BP_cdr(context_t* context);
void BP_list(context_t* context);

void BP_apply(context_t* context);

void BP_symbolP(context_t* context);
void BP_pairP(context_t* context);

void BP_fixnum_PLUS(context_t* context);
void BP_max_fixnum(context_t* context);
void BP_min_fixnum(context_t* context);
void BP_collect(context_t* context);
#endif
