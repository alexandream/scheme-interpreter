#include <iostream>
#include <sstream>
#include <string>
#include "fixnum.h"

const
value_t FIXNUM_MAX = wrap_fixnum(0x03FFFFFFFFFFFFFF),
        FIXNUM_MIN = wrap_fixnum(0x0FFFFFFFFFFFFFFF);


value_t fixnum_make_from_string(const std::string& str) {
	// ATTENTION: This assumes that checking whether the string represents a
	// fixnum or not was already done by an upper layer, like the lexer.

	int64_t num;
	std::stringstream ss(str);

	ss >> num;
	return wrap_fixnum(num);
}

std::string fixnum_format(value_t value) {
	// ATTENTION: This assumes that checking if the value is a valid fixnum is
	// already done by an upper layer, like the dispatcher.

	std::ostringstream oss;
	oss << unwrap_fixnum(value);
	return oss.str();
}
