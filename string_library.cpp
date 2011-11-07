#include <string>
#include <cctype>
#include <assert.h>

#include "value.h"
#include "string.h"
#include "string_library.h"

#include "fixnum.h"
#include "character.h"

void BP_stringP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	bool result = (is_string(param));
	BUILTIN_RETURN(context, wrap_boolean(result));
}

void BP_make_string(context_t* context) {
	value_t params = context->value_stack;
	value_t p_count = pair_left(params);
	value_t p_c = wrap_character('?');
	if (context->value_stack_size == 2) {
		p_c = pair_left(pair_right(params));
	}
	assert(is_character(p_c) && is_fixnum(p_count));
	
	int64_t count = unwrap_fixnum(p_count);
	char c = unwrap_character(p_c);
	
	assert(count >= 0 && count <= 65535);
	std::string str(count, c);

	value_t result = wrap_string(str);
	BUILTIN_RETURN(context, result);
}


void BP_string(context_t* context) {
	value_t params = context->value_stack;
	int32_t nparams = context->value_stack_size;
	
	std::string str(nparams, ' ');
	for (int i = 0; i < nparams; i++) {
		value_t elem = pair_left(params);
		assert(is_character(elem));
		char c = unwrap_character(elem);
		str[i] = c;
		params = pair_right(params);
	}
	value_t result = wrap_string(str);
	BUILTIN_RETURN(context, result);
}

void BP_string_length(context_t* context) {
	value_t param = pair_left(context->value_stack);
	assert(is_string(param));
	std::string* str = unwrap_internal_string(param);

	value_t result = wrap_fixnum((int64_t) str->length()); 
	BUILTIN_RETURN(context, result);
}

void BP_string_ref(context_t* context) {
	value_t params = context->value_stack;
	value_t str = pair_left(params);
	value_t index = pair_left(pair_right(params));

	assert(is_string(str) && is_fixnum(index));
	
	int64_t i = unwrap_fixnum(index);
	std::string* s = unwrap_internal_string(str);

	assert(i >= 0 && i < s->length());

	value_t result = wrap_character(s->at(i));
	BUILTIN_RETURN(context, result);
}


void BP_string_setB(context_t* context) {
	value_t params = context->value_stack;
	value_t str = pair_left(params);
	params = pair_right(params);
	value_t index = pair_left(params);
	value_t chr = pair_left(pair_right(params));

	assert(is_string(str) && is_fixnum(index) && is_character(chr));

	int64_t i = unwrap_fixnum(index);
	std::string* s = unwrap_internal_string(str);
	char c = unwrap_character(chr);

	assert(i >= 0 && i < s->length());

	s->replace(i, 1, 1, c);
	BUILTIN_RETURN(context, UNSPECIFIED);
}

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
