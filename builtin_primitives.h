#ifndef __BUILTIN_PRIMITIVES_H__
#define __BUILTIN_PRIMITIVES_H__

#include "value.h"
#include "evaluator.h"

void BP_gensym(context_t* context);
void BP_macro_expand(context_t* context);

void BP_not(context_t* context);
void BP_and(context_t* context);
void BP_or(context_t* context);

void BP_eqP(context_t* context);

void BP_cons(context_t* context);
void BP_car(context_t* context);
void BP_cdr(context_t* context);
void BP_set_carB(context_t* context);
void BP_set_cdrB(context_t* context);
void BP_list(context_t* context);

void BP_apply(context_t* context);

void BP_symbolP(context_t* context);
void BP_pairP(context_t* context);
void BP_booleanP(context_t* context);
void BP_unspecifiedP(context_t* context);
void BP_procedureP(context_t* context);
void BP_fixnumP(context_t* context);

void BP_fixnum_PLUS(context_t* context);
void BP_fixnum_MINUS(context_t* context);
void BP_fixnum_MUL(context_t* context);
void BP_fixnum_DIV(context_t* context);
void BP_fixnum_truncate_remainder(context_t* context);
void BP_fixnum_truncate_quotient(context_t* context);
void BP_fixnum_floor_quotient(context_t* context);
void BP_fixnum_floor_remainder(context_t* context);
void BP_fixnum_ceil_quotient(context_t* context);
void BP_fixnum_ceil_remainder(context_t* context);
void BP_fixnum_round_quotient(context_t* context);
void BP_fixnum_round_remainder(context_t* context);
void BP_fixnum_euclid_quotient(context_t* context);
void BP_fixnum_euclid_remainder(context_t* context);

void BP_fixnum_to_string(context_t* context);
void BP_fixnum_from_string(context_t* context);

void BP_fixnum_LT(context_t* context);
void BP_fixnum_GT(context_t* context);
void BP_fixnum_LTE(context_t* context);
void BP_fixnum_GTE(context_t* context);
void BP_fixnum_EQ(context_t* context);

void BP_symbol_to_string(context_t* context);
void BP_symbol_from_string(context_t* context);

void BP_max_fixnum(context_t* context);
void BP_min_fixnum(context_t* context);
void BP_collect(context_t* context);
void BP_echo(context_t* context);
#endif
