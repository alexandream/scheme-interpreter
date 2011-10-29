#ifndef __FIXNUM_LIBRARY_H__
#define __FIXNUM_LIBRARY_H__

#include "evaluator.h"

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
void BP_max_fixnum(context_t* context);
void BP_min_fixnum(context_t* context);

#endif
