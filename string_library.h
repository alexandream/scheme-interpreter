#ifndef __STRING_LIBRARY_H__
#define __STRING_LIBRARY_H__

#include "evaluator.h"

void BP_stringP(context_t* context);

void BP_make_string(context_t* context);
void BP_string(context_t* context);

void BP_string_length(context_t* context);
void BP_string_ref(context_t* context);
void BP_string_setB(context_t* context);


void BP_string_eqP(context_t* context);
void BP_string_ltP(context_t* context);
void BP_string_gtP(context_t* context);
void BP_string_lteP(context_t* context);
void BP_string_gteP(context_t* context);

void BP_string_ci_eqP(context_t* context);
void BP_string_ci_ltP(context_t* context);
void BP_string_ci_gtP(context_t* context);
void BP_string_ci_lteP(context_t* context);
void BP_string_ci_gteP(context_t* context);

void BP_string_alphabeticP(context_t* context);
void BP_string_numericP(context_t* context);
void BP_string_whitespaceP(context_t* context);
void BP_string_upper_caseP(context_t* context);
void BP_string_lower_caseP(context_t* context);

void BP_string_upcase(context_t* context);
void BP_string_downcase(context_t* context);

void BP_string_substring(context_t* context);
void BP_string_append(context_t* context);
void BP_string_copy(context_t* context);
void BP_string_fillB(context_t* context);

void BP_string_2_list(context_t* context);
void BP_list_2_string(context_t* context);


#endif
