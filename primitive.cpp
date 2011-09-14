#include <string>
#include <sstream>
#include <error.h>

#include "value.h"

#include "primitive.h"
#include "builtin_primitives.h"
#include "storage.h"
#include "pointer.h"
#include "pair.h"


const
uint8_t PRIMITIVE_TYPE_MASK = 0x03;

const
uint8_t ARITY_ANY = 0xFF;


std::string primitive_format(value_t value) {
	std::stringstream sstream;

	double_storage_t* storage = (double_storage_t*) unwrap_pointer(value);
	
	primitive_descriptor_t* descriptor = 
		(primitive_descriptor_t*) storage->first_slot;
	

	sstream << "Primitive Function \'" << descriptor->name 
	        << "\' receiving " << (unsigned int) descriptor->min_arity 
			<< " to " << (unsigned int) descriptor->max_arity << " arguments."
	        << std::endl;

	return sstream.str();
}

value_t make_primitive(primitive_descriptor_t* descriptor) {
	// make_primitive takes ownership of "descriptor" parameter. Has to live
	// throughout entire session.
	double_storage_t* storage = alloc_double_storage();

	storage->header = make_header(true, PRIMITIVE_TYPE_MASK, MARK_POLICY_NONE);
	storage->first_slot = (uint64_t) descriptor;

	return wrap_pointer(storage);
}


void primitive_apply(context_t* context, value_t func) {
	double_storage_t* storage = (double_storage_t*) unwrap_pointer(func);
	
	primitive_descriptor_t* descriptor = 
		(primitive_descriptor_t*) storage->first_slot;

	int32_t argument_count = context->value_stack_size;
	if (argument_count < descriptor->min_arity ||
		argument_count > descriptor->max_arity)	 {
		error(1, 0, "Expected %d to %d arguments for primitive '%s', got %d",
		            descriptor->min_arity,
					descriptor->max_arity,
					descriptor->name.c_str(),
					argument_count);
	}
	primitive_t f_ptr = descriptor->handler;
	context->value_stack_size = argument_count;
	(*f_ptr)(context);
}

