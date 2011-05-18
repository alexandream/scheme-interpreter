#include <string>
#include <sstream>

#include "value.h"

#include "primitive.h"
#include "pointer.h"


const
uint8_t PRIMITIVE_TYPE_MASK = 0x03;

std::string primitive_format(value_t value) {
	std::stringstream sstream;

	double_storage_t* storage = (double_storage_t*) unwrap_pointer(value);
	
	primitive_descriptor_t* descriptor = 
		(primitive_descriptor_t*) storage->second_slot;
	

	sstream << "Primitive Function \'" << descriptor->name 
	        << "\' receiving " << (unsigned int) descriptor->arity << " arguments."
	        << std::endl;

	return sstream.str();
}

value_t make_primitive(primitive_t func,
		               primitive_descriptor_t* descriptor) {
	double_storage_t* storage = alloc_double_storage();

	storage->header = make_header(true, PRIMITIVE_TYPE_MASK);
	storage->first_slot = (uint64_t) func;
	storage->second_slot = (uint64_t) descriptor;

	return wrap_pointer(storage);
}


value_t primitive_apply(value_t func, value_t param_list) {
	double_storage_t* storage = (double_storage_t*) unwrap_pointer(func);

	primitive_t f_ptr = (primitive_t) storage->first_slot;
	
	return (*f_ptr)(param_list);
}

