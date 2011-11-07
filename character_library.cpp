#include <cctype>

#include "value.h"
#include "character.h"
#include "character_library.h"

typedef bool (*char_relop)(char, char);

static bool RELOP_char_eqP(char c1, char c2);
static bool RELOP_char_ltP(char c1, char c2);
static bool RELOP_char_gtP(char c1, char c2);
static bool RELOP_char_lteP(char c1, char c2);
static bool RELOP_char_gteP(char c1, char c2);

static void BP_AUX_char_relop(context_t* context, char_relop func);
static void BP_AUX_char_ci_relop(context_t* context, char_relop func);

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
