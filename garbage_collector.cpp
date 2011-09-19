#include <iostream>

#include "garbage_collector.h"

#include "evaluator.h"
#include "finalizer.h"
#include "value.h"
#include "environment.h"
#include "macro.h"
#include "storage.h"
#include "symbol.h"
#include "pair.h"
#include "pointer.h"

const 
uint8_t MARK_POLICY_NONE   = 0x00,
        MARK_POLICY_FIRST  = 0x10,
        MARK_POLICY_SECOND = 0x20,
        MARK_POLICY_BOTH   = 0x30,
		GC_ALWAYS_MARKED   = 0x40,
		GC_HAS_MARK        = 0x80,
		GC_IS_IN_USE       = 0x08;
	
static
uint64_t mark_count, 
         sweep_count, 
         node_count;

void do_mark_phase(void);
void do_sweep_phase(void);
void mark_context(context_t* context);
void mark_environment(value_t value);
void mark(value_t value);

void collect(void) {
	mark_count = 0;
	sweep_count = 0;
	node_count = 0;
	do_mark_phase();
	do_sweep_phase();
	printf("Marked: %Ld (%lu in symbol list) nodes of %Ld. Freed %Ld.\n", mark_count, get_symbol_pool_size(), node_count, sweep_count);
}

void do_mark_phase(void) {
	context_list_t* context_list = list_active_contexts();
	
	context_list_t::iterator iter;
	for (iter = context_list->begin(); iter != context_list->end(); iter++) {
		context_t* context = *iter;
		mark_context(context);
	}
	binding_map_t* rewriters = list_macro_rewriters();

	binding_map_t::iterator iter2;
	for (iter2 = rewriters->begin(); iter2 != rewriters->end(); iter2++) {
		value_t rewriter = iter2->second;
		mark(rewriter);
	}
}

void do_sweep_phase(void) {
	int pool_size = 0;
	double_storage_t* pool = get_double_storage_pool(&pool_size);
	for(int i = 0; i < pool_size; i++) { 
		uint64_t header = pool[i].header;
		if (is_in_use(header)) {
			node_count++;
			if (!has_gc_mark(header)) {
				sweep_count++;
				finalize(wrap_pointer(&pool[i]));
				free_double_storage(&pool[i]);
			}
			else {
				clear_gc_mark(&(pool[i].header));
			}
		}
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
	// heap storage. Also ignore already marked values. Symbols are *always*
	// marked.
	if (is_immediate(value)) {
		return;
	}
	uint64_t* header_ptr = get_header(value);
	uint64_t header = *header_ptr;
	if (has_gc_mark(header)) {
		return;
	}
	
	mark_count++;
	set_gc_mark(header_ptr);
	if (is_environment(value)) {
		// Environments have to be handled differently because they have an
		// external collection of GC'd data in it (the bindings hash table).
		mark_environment(value);
	}
	else {
		if (must_mark_first(header)) {
			mark(pair_left(value));
		}
		if (must_mark_second(header)) {
			mark(pair_right(value));
		}
	}
}

void mark_environment(value_t value) {
	mark(pair_left(value));

	binding_map_t* bindings = environment_get_all_bindings(value);
	binding_map_t::iterator iter = bindings->begin();
	while(iter != bindings->end()) {
		mark(iter->second);
		iter++;
	}
}
