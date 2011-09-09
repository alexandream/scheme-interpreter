#include <string>
#include <sstream>

#include "fixnum.h"
#include "value.h"

const
value_t FIXNUM_MAX = wrap_fixnum(0x3FFFFFFFFFFFFFFF),
        FIXNUM_MIN = wrap_fixnum(0xC000000000000000);

std::string fixnum_format(value_t value) {
	// XXX: Assertion #003
	
	std::ostringstream sstream;
	sstream << unwrap_fixnum(value);
	return sstream.str();
}


value_t fixnum_make_from_string(const std::string& str) {
	int64_t num;
	std::stringstream ss(str);

	ss >> num;
	return wrap_fixnum(num);
}
