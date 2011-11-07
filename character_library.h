#ifndef __CHARACTER_LIBRARY_H__
#define __CHARACTER_LIBRARY_H__

#include "evaluator.h"

void BP_charP(context_t* context);

void BP_char_eqP(context_t* context);
void BP_char_ltP(context_t* context);
void BP_char_gtP(context_t* context);
void BP_char_lteP(context_t* context);
void BP_char_gteP(context_t* context);

void BP_char_ci_eqP(context_t* context);
void BP_char_ci_ltP(context_t* context);
void BP_char_ci_gtP(context_t* context);
void BP_char_ci_lteP(context_t* context);
void BP_char_ci_gteP(context_t* context);


#endif
