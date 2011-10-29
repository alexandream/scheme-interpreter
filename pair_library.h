#ifndef __PAIR_LIBRARY_H__
#define __PAIR_LIBRARY_H__

#include "evaluator.h"

void BP_pairP(context_t* context);
void BP_cons(context_t* context);
void BP_car(context_t* context);
void BP_cdr(context_t* context);
void BP_set_carB(context_t* context);
void BP_set_cdrB(context_t* context);
void BP_list(context_t* context);

#endif
