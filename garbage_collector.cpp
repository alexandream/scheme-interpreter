#include "garbage_collector.h"

#include "evaluator.h"
#include "value.h"
#include "symbol.h"
#include "environment.h"
#include "storage.h"
#include "pair.h"

const 
uint8_t MARK_POLICY_NONE   = 0,
        MARK_POLICY_SECOND = 1,
        MARK_POLICY_FIRST  = 2,
        MARK_POLICY_BOTH   = 3;

void do_mark_phase();
void mark_context(context_t* context);
void mark_environment(value_t value);
void mark(value_t value);

void collect(void) {
	do_mark_phase();
	//do_sweep_phase();
}

void do_mark_phase() {
	context_list_t* context_list = list_active_contexts();
	
	context_list_t::iterator iter;
	for (iter = context_list->begin(); iter != context_list->end(); iter++) {
		context_t* context = *iter;
		mark_context(context);
	}
}

void mark_context(context_t* context) {
	mark(context->accumulator);
	mark(context->value_stack);
	mark(context->next_expr);
	mark(context->environment);
	mark(context->frame_stack);
}

void mark(value_t value) {
	// Immediate values (fixnums, bools, etc) are not marked, for they use no
	// heap storage. Symbols are, during the sweep phase, always considered
	// marked, so there is no point in marking them again here.
	if (is_immediate(value) || is_symbol(value)) return;

	// Detect already marked values, and do nothing to them.
	if (has_gc_mark(value)) return;
	
	if (is_environment(value)) {
		// Environments have to be handled differently because they have an
		// external collection of GC'd data in it (the bindings hash table).
		mark_environment(value);
	}
	else {
		set_gc_mark(value);
		if (must_mark_first(value))
			mark(pair_left(value));
		if (must_mark_second(value))
			mark(pair_right(value));
	}
}

void mark_environment(value_t value) {
	
}
