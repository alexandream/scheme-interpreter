#include <string>
#include <sstream>
#include <error.h>
#include <assert.h>
#include <stdio.h>

#include "fixnum.h"
#include "value.h"

const
value_t FIXNUM_MAX = wrap_fixnum(0x3FFFFFFFFFFFFFFF),
        FIXNUM_MIN = wrap_fixnum(0xC000000000000000);

std::string fixnum_format(value_t value, char radix) {
	assert(is_fixnum(value));
	std::ostringstream sstream;
    int64_t fixnum = unwrap_fixnum(value); 
    std::string result;
	switch(radix) {
        case 'd':
            sstream << fixnum;
            result = sstream.str();
            break;
        case 'o':
            sstream << std::oct << fixnum;
            result = std::string("#o") + sstream.str();
            break;
        case 'x':
            sstream << std::hex << fixnum;
            result = std::string("#x") + sstream.str();
            break;
        case 'b':
            result = "#b";
            while(fixnum > 0) {
                result.insert( 2, 1, (fixnum % 2 == 0) ? '0'
                                                       : '1');
                fixnum /= 2;
            }
            
            break;
        default:
            error(1, 0, "Invalid radix.");
            break;
    }
	return result;
}


value_t fixnum_preprocess(const std::string& original_str) {
	std::string str(original_str);
	size_t exact_pos = str.find("#e");
	if (exact_pos != std::string::npos)
	{
		str.erase(exact_pos, 2);
	}
	char radix = 'd';
	int multiplier = 1;
	if (str[0] == '#') {
		radix = str[1];
		str = str.substr(2);
	}
	if (str[0] == '-' || str[0] == '+') {
		if (str[0] == '-') multiplier = -1;
		str = str.substr(1);	
	}
	int64_t num = 0;
	std::stringstream ss(str);
	switch(radix) {
		case 'd':
			ss >> num;
			break;
		case 'x':
			ss >> std::hex >> num;
			break;
		case 'o':
			ss >> std::oct >> num;
			break;
		case 'b':
			int length = str.length();
			for (int i = 0; i < length; i++) {
				num *= 2;
				num += (str[i] == '1')? 1 : 0;
			}
			break;
	}
	return wrap_fixnum(multiplier*num);
}
value_t fixnum_make_from_string(const std::string& str) {
	int64_t num;
	std::stringstream ss(str);
	
	ss >> num;
	return wrap_fixnum(num);
}
