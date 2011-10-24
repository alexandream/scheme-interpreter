#include <assert.h>

#include "macro.h"
#include "value.h"
#include "function.h"
#include "primitive.h"
#include "symbol.h"
#include "environment.h"
#include "evaluator.h"
#include "pair.h"


binding_map_t rewriters;

static
value_t expansion(value_t expression);
static inline
value_t macro_expand_1(value_t rewriter, value_t args);


value_t macro_expand(value_t expression) {
    if (!is_pair(expression)) {
        return expression;
    }
    else {
        return expansion(expression);
    }
}

binding_map_t* list_macro_rewriters(void) {
    return &rewriters;
}

void macro_set_rewriter(value_t name, value_t rewriter) {
    assert(is_symbol(name));
    assert(is_function(rewriter) || is_primitive(rewriter));
    // TODO: Check if parameter lists are compatible with rewriters.
    
    rewriters[name] = rewriter;
}

value_t macro_get_rewriter(value_t name) {
    binding_map_t::iterator binding = rewriters.find(name);

    value_t result = UNDEFINED;
    if (binding != rewriters.end()) {
        result = binding->second;
    }
    return result;
}

static inline
bool is_macro_expansion(value_t expression, 
                        value_t* out_name,
                        value_t* out_args,
                        value_t* out_rewriter) {

    if (!is_pair(expression))
            return false;

    value_t name = pair_left(expression);
    
    if (!is_symbol(name))
        return false;
    value_t rewriter = macro_get_rewriter(name);
    if (rewriter == UNDEFINED)
        return false;
    
    *out_name = name;
    *out_args = pair_right(expression);
    *out_rewriter = rewriter;
    return true;
}

static inline
value_t expansion(value_t expression) {
    value_t name,
            args,
            rewriter;
    while (is_macro_expansion(expression, &name, &args, &rewriter)) {
        expression = macro_expand_1(rewriter, args);
    }
    return expression;

}

static inline
value_t macro_expand_1(value_t rewriter, value_t args) {
    context_t* context = make_context(GLOBAL_ENVIRONMENT);
    context->accumulator = rewriter;
    context->value_stack_size = pair_linked_length(args);

    value_t op_halt = make_list(OP_HALT, 0);
    protect_value(op_halt); 

    value_t op_apply = make_list(OP_APPLY, 0);
    protect_value(op_apply);

    value_t rewriter_constant = make_list(OP_CONSTANT, rewriter, op_apply, 0);
    protect_value(rewriter_constant);

    value_t args_argument = make_list(OP_ARGUMENT, rewriter_constant, 0);
    protect_value(args_argument);

    value_t args_constant = make_list(OP_CONSTANT, args, args_argument, 0);
    protect_value(args_constant);
    
    value_t frame = make_list(OP_FRAME, op_halt, args_constant, 0);

    context->next_expr = frame;

    unprotect_storage(5);
    evaluate(context);
	value_t result = context->accumulator;
	dispose_context(context);
    return result;
}

