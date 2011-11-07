#include <iostream>
#include <sstream>
#include <stdio.h>
#include <error.h>

#include "value.h"

#include "reader.h"
#include "pair.h"
#include "garbage_collector.h"
#include "evaluator.h"
#include "compiler.h"
#include "printer.h"
#include "formatter.h"
#include "special.h"
#include "symbol.h"
#include "scanner.h"
#include "storage.h"
#include "pointer.h"
#include "environment.h"
#include "primitive.h"
#include "builtin_primitives.h"

value_t GLOBAL_ENVIRONMENT;

primitive_descriptor_t primitives[] = {
	// UTILS
	{ "gensym", BP_gensym, 0, 0 },
    { "macro-expand", BP_macro_expand, 1, 1},
	// BOOLEANS
	{ "not",   BP_not,   1, 1 },
	// EQUIVALENCE
	{ "eq?",    BP_eqP,   2, 2 },
	{ "eqv?",   BP_eqP,   2, 2 },
    { "equal?", BP_eqP,   2, 2 },
	// PAIRS
	{ "cons",     BP_cons,     2, 2 },
	{ "car",      BP_car,      1, 1 },
	{ "cdr",      BP_cdr,      1, 1 },
	{ "set-car!", BP_set_carB, 2, 2 },
	{ "set-cdr!", BP_set_cdrB, 2, 2 },
	{ "list",     BP_list,     0, ARITY_ANY },
	
	// CONTROL
	{ "apply", BP_apply, 2, ARITY_ANY },
	// FIXNUM
	// -- Mathematical operations
	{ "+",       BP_fixnum_PLUS,  0, ARITY_ANY },
	{ "*",       BP_fixnum_MUL,   0, ARITY_ANY },
	{ "-",       BP_fixnum_MINUS, 1, ARITY_ANY },
	{ "/",       BP_fixnum_DIV,   1, ARITY_ANY },
	// -- Relational operators
	{ "<",       BP_fixnum_LT,    0, ARITY_ANY },
	{ ">",       BP_fixnum_GT,    0, ARITY_ANY },
	{ "<=",      BP_fixnum_LTE,   0, ARITY_ANY },
	{ ">=",      BP_fixnum_GTE,   0, ARITY_ANY },
	{ "=",       BP_fixnum_EQ,    0, ARITY_ANY },
	// -- Quotient/Remainder/Modulo
	{ "truncate-remainder",  BP_fixnum_truncate_remainder,   2, 2 },
    { "truncate-quotient",   BP_fixnum_truncate_quotient,    2, 2 },
	{ "floor-remainder",     BP_fixnum_floor_remainder,      2, 2 },
    { "floor-quotient",      BP_fixnum_floor_quotient,       2, 2 },
	{ "ceiling-remainder",   BP_fixnum_ceil_remainder,       2, 2 },
    { "ceiling-quotient",    BP_fixnum_ceil_quotient,        2, 2 },
	{ "round-remainder",     BP_fixnum_round_remainder,      2, 2 },
    { "round-quotient",      BP_fixnum_round_quotient,       2, 2 },
	{ "euclid-remainder",    BP_fixnum_euclid_remainder,     2, 2 },
    { "euclid-quotient",     BP_fixnum_euclid_quotient,      2, 2 },
    // -- Conversion to/from string
    { "number->string", BP_fixnum_to_string,   1, 2 },
    { "string->number", BP_fixnum_from_string, 1, 2 },

	// CHARACTER
	// -- Relational operators
	{ "char=?",     BP_char_eqP,     2, 2 },
	{ "char<?",     BP_char_ltP,     2, 2 },
	{ "char>?",     BP_char_gtP,     2, 2 },
	{ "char<=?",    BP_char_lteP,    2, 2 },
	{ "char>=?",    BP_char_gteP,    2, 2 },
	{ "char-ci=?",  BP_char_ci_eqP,  2, 2 },
	{ "char-ci<?",  BP_char_ci_ltP,  2, 2 },
	{ "char-ci>?",  BP_char_ci_gtP,  2, 2 },
	{ "char-ci<=?", BP_char_ci_lteP, 2, 2 },
	{ "char-ci>=?", BP_char_ci_gteP, 2, 2 },
	// -- Category predicates
	{ "char-alphabetic?", BP_char_alphabeticP, 1, 1 },
	{ "char-numeric?",    BP_char_numericP,    1, 1 },
	{ "char-whitespace?", BP_char_whitespaceP, 1, 1 },
	{ "char-upper-case?", BP_char_upper_caseP, 1, 1 },
	{ "char-lower-case?", BP_char_lower_caseP, 1, 1 },
	// -- Case handling
	{ "char-upcase",   BP_char_upcase,   1, 1 },
	{ "char-downcase", BP_char_downcase, 1, 1 },
	{ "char-foldcase", BP_char_downcase, 1, 1 },
	// -- Conversion to/from integer
	{ "char->integer", BP_char_2_integer, 1, 1 },
	{ "integer->char", BP_integer_2_char, 1, 1 },

	// STRING
	// -- Constructors
	{ "make-string",  BP_make_string, 1, 2 },
	{ "string",       BP_string,      0, ARITY_ANY },
	// -- Accessors
	{ "string-length", BP_string_length, 1, 1 },
	{ "string-ref",    BP_string_ref,    2, 2 },
	{ "string-set!",   BP_string_setB,   3, 3 },

    // SYMBOL
    { "symbol->string", BP_symbol_to_string,   1, 1 },
    { "string->symbol", BP_symbol_from_string, 1, 1 },

	// TEST
	{ "collect",    BP_collect,    0, 0 },
	{ "min_fixnum", BP_min_fixnum, 0, 0 },
	{ "max_fixnum", BP_max_fixnum, 0, 0 },
	{ "echo",    BP_echo, 1, 2 },

	// TYPE PREDICATE
	{ "symbol?",      BP_symbolP,      1, 1 },
	{ "pair?",        BP_pairP,        1, 1 },
	{ "boolean?",     BP_booleanP,     1, 1 },
	{ "unspecified?", BP_unspecifiedP, 1, 1 },
	{ "integer?",     BP_fixnumP,      1, 1 },
    { "procedure?",   BP_procedureP,   1, 1 },
	{ "char?",        BP_charP,        1, 1 },
	{ "string?",      BP_stringP,      1, 1 },

	// TOMBSTONE
	{ "",      0,        0, 0 }
};


void bind_primitive(value_t env, primitive_descriptor_t* descriptor) {
	value_t symbol = make_symbol(descriptor->name);
	value_t primitive = make_primitive(descriptor);
	
	environment_add(env, symbol, primitive);
}

void load_file(context_t* context, const char* fname) {
	value_t input;
	FILE *fp = fopen(fname, "r");
	if (fp == NULL) {
		error(1, 0, "Unable to load definitions file 'definitions.scm'");
	}
	scanner_push(fp);

	while ( (input = read()) != END_OF_FILE ) {
		protect_value(input);
		value_t op_halt = make_list(OP_HALT, 0);
		protect_value(op_halt);

		context->next_expr = compile(input, op_halt);
		unprotect_storage(2);
		evaluate(context);
	}
	scanner_pop();
}

int main_repl(int argc, char ** argv) {
	value_t input;
	value_t result;
	std::cerr << "Welcome to Stutter." << std::endl;

	value_t global_env = make_environment(UNSPECIFIED);
	GLOBAL_ENVIRONMENT = global_env;
	// Register primitives into toplevel environment.
	for (int i = 0; primitives[i].handler != 0; i++) {
		bind_primitive(global_env, &primitives[i]);
	}
	
	context_t* context = make_context(global_env);

	// Register other built-ins, non primitives.
	load_file(context, "definitions.scm");

	scanner_push(stdin);
	while(true) {
		std::cerr << "> ";
		input = read();
		protect_value(input);
		if (input == END_OF_FILE) {
			std::cout << std::endl;
			unprotect_storage(1);
			break;
		}
	
		value_t op_halt = make_list(OP_HALT, 0);
		protect_value(op_halt);

		context->next_expr = compile(input, op_halt);
#ifdef DEBUG_COMPILER
		std::cerr << "; " << format(context->next_expr) << std::endl;
#endif
		unprotect_storage(2);
		result = evaluate(context);
		println(result);
	}
	return 0;
}

int (*prog_pool[])(int, char**) = {
	main_repl
};

int main(int argc, char** argv) {
	int index = 0;

	init_storage();

	if (argc > 1) {
		int i;
		std::string arg(argv[1]);
		std::stringstream stream(arg);
		if (stream >> i) {
			index = i;
		}
	}
	return prog_pool[index](argc, argv);
}
