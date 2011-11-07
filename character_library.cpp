#include <cctype>

#include "value.h"
#include "character.h"
#include "character_library.h"

#include "fixnum.h"

typedef bool (*char_relop)(char, char);
typedef bool (*char_pred)(char);

static bool RELOP_char_eqP(char c1, char c2);
static bool RELOP_char_ltP(char c1, char c2);
static bool RELOP_char_gtP(char c1, char c2);
static bool RELOP_char_lteP(char c1, char c2);
static bool RELOP_char_gteP(char c1, char c2);

static bool PRED_char_alphabeticP(char c);
static bool PRED_char_numericP(char c);
static bool PRED_char_whitespaceP(char c);
static bool PRED_char_upper_caseP(char c);
static bool PRED_char_lower_caseP(char c);

static void BP_AUX_char_relop(context_t* context, char_relop func);
static void BP_AUX_char_ci_relop(context_t* context, char_relop func);
static void BP_AUX_char_pred(context_t* context, char_pred func);

void BP_charP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	bool result = (is_character(param));
	BUILTIN_RETURN(context, wrap_boolean(result));
}


void BP_char_eqP(context_t* context) {
	BP_AUX_char_relop(context, RELOP_char_eqP);
}

void BP_char_ltP(context_t* context) {
	BP_AUX_char_relop(context, RELOP_char_ltP);
}

void BP_char_gtP(context_t* context) {
	BP_AUX_char_relop(context, RELOP_char_gtP);
}

void BP_char_lteP(context_t* context) {
	BP_AUX_char_relop(context, RELOP_char_lteP);
}

void BP_char_gteP(context_t* context) {
	BP_AUX_char_relop(context, RELOP_char_gteP);
}

void BP_char_ci_eqP(context_t* context) {
	BP_AUX_char_ci_relop(context, RELOP_char_eqP);
}

void BP_char_ci_ltP(context_t* context) {
	BP_AUX_char_ci_relop(context, RELOP_char_ltP);
}

void BP_char_ci_gtP(context_t* context) {
	BP_AUX_char_ci_relop(context, RELOP_char_gtP);
}

void BP_char_ci_lteP(context_t* context) {
	BP_AUX_char_ci_relop(context, RELOP_char_lteP);
}

void BP_char_ci_gteP(context_t* context) {
	BP_AUX_char_ci_relop(context, RELOP_char_gteP);
}

void BP_char_alphabeticP(context_t* context) {
	BP_AUX_char_pred(context, PRED_char_alphabeticP);
}

void BP_char_numericP(context_t* context) {
	BP_AUX_char_pred(context, PRED_char_numericP);
}

void BP_char_whitespaceP(context_t* context) {
	BP_AUX_char_pred(context, PRED_char_whitespaceP);
}

void BP_char_upper_caseP(context_t* context) {
	BP_AUX_char_pred(context, PRED_char_upper_caseP);
}

void BP_char_lower_caseP(context_t* context) {
	BP_AUX_char_pred(context, PRED_char_lower_caseP);
}


void BP_char_upcase(context_t* context) {
	char c = unwrap_character(pair_left(context->value_stack));
	value_t result = wrap_character(toupper(c));
	BUILTIN_RETURN(context, result);
}
void BP_char_downcase(context_t* context) {
	char c = unwrap_character(pair_left(context->value_stack));
	value_t result = wrap_character(tolower(c));
	BUILTIN_RETURN(context, result);
}

void BP_char_2_integer(context_t* context) {
	char c = unwrap_character(pair_left(context->value_stack));
	value_t result = wrap_fixnum((int64_t) c);
	BUILTIN_RETURN(context, result);
}

void BP_integer_2_char(context_t* context) {
	int64_t i = unwrap_fixnum(pair_left(context->value_stack));
	value_t result;

	if (i >= 0 && i < 128) {
		result = wrap_character((char) i);
	}
	else {
		result = UNSPECIFIED;
	}
	BUILTIN_RETURN(context, result);
}

// --
// -- Auxiliary Functions
// --

static
void BP_AUX_char_relop(context_t* context, char_relop func) {
	value_t params = context->value_stack;
	char c1 = unwrap_character(pair_left(params));
	char c2 = unwrap_character(pair_left(pair_right(params)));
	bool result = func(c1, c2);
	BUILTIN_RETURN(context, wrap_boolean(result));
}

static
void BP_AUX_char_ci_relop(context_t* context, char_relop func) {
	value_t params = context->value_stack;
	char c1 = tolower(unwrap_character(pair_left(params)));
	char c2 = tolower(unwrap_character(pair_left(pair_right(params))));
	bool result = func(c1, c2);
	BUILTIN_RETURN(context, wrap_boolean(result));
}

static
void BP_AUX_char_pred(context_t* context, char_pred func) {
	value_t params = context->value_stack;
	char c = unwrap_character(pair_left(params));
	bool result = func(c);
	BUILTIN_RETURN(context, wrap_boolean(result));
}

static
bool RELOP_char_eqP(char c1, char c2) {
	return c1 == c2;
}
static
bool RELOP_char_ltP(char c1, char c2) {
	return c1 < c2;
}
static
bool RELOP_char_gtP(char c1, char c2) {
	return c1 > c2;
}
static
bool RELOP_char_lteP(char c1, char c2) {
	return c1 <= c2;
}
static
bool RELOP_char_gteP(char c1, char c2) {
	return c1 >= c2;
}


static 
bool PRED_char_alphabeticP(char c) {
	return isalpha(c);
}

static
bool PRED_char_numericP(char c) {
	return isdigit(c);
}

static
bool PRED_char_whitespaceP(char c) {
	return isspace(c);
}

static
bool PRED_char_upper_caseP(char c) {
	return isupper(c);
}

static
bool PRED_char_lower_caseP(char c) {
	return islower(c);
}
