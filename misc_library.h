#ifndef __MISC_LIBRARY_H__
#define __MISC_LIBRARY_H__

#include "evaluator.h"

void BP_booleanP(context_t* context);
void BP_not(context_t* context);

void BP_eqP(context_t* context);

void BP_apply(context_t* context);

void BP_gensym(context_t* context);

void BP_macro_expand(context_t* context);

void BP_collect(context_t* context);

void BP_echo(context_t* context);

void BP_unspecifiedP(context_t* context);
void BP_procedureP(context_t* context);

void BP_symbolP(context_t* context);
void BP_symbol_to_string(context_t* context);
void BP_symbol_from_string(context_t* context);
#endif
