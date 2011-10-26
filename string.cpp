#include <string>
#include <sstream>

#include "string.h"


const uint8_t STRING_TYPE_MASK = 0x06;

value_t string_preprocess(const std::string& str) {
    
    return wrap_string(str.substr(1, str.size() - 2));
}

value_t wrap_string(const std::string& str) {
    double_storage_t* storage = alloc_double_storage();
    storage->header = make_header(true, STRING_TYPE_MASK, MARK_POLICY_NONE);
    storage->first_slot = (uint64_t) new std::string(str);

    return wrap_pointer(storage);
}

std::string string_format(value_t value) {
    std::stringstream sstream;
    double_storage_t* storage = (double_storage_t*) unwrap_pointer(value);
    std::string* string_pointer = (std::string*) storage->first_slot;
    sstream << "\"" << *string_pointer << "\"";
    return sstream.str();
}

void string_finalize(value_t value) {
    double_storage_t* storage = (double_storage_t*) unwrap_pointer(value);
    std::string* string_pointer = (std::string*) storage->first_slot;
    delete string_pointer;
}
