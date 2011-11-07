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

void BP_char_alphabeticP(context_t* context);
void BP_char_numericP(context_t* context);
void BP_char_whitespaceP(context_t* context);
void BP_char_upper_caseP(context_t* context);
void BP_char_lower_caseP(context_t* context);

void BP_char_upcase(context_t* context);
void BP_char_downcase(context_t* context);

void BP_char_2_integer(context_t* context);
void BP_integer_2_char(context_t* context);


#endif
