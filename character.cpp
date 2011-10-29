#include <string>
#include <sstream>
#include <error.h>
#include <assert.h>
#include "character.h"
#include "special.h"
struct character_names_t {
    char character;
    char* name;
};

static
std::string get_character_name(char character) {
    std::string result;
    switch (character) {
    	case '\x00': result = "#\\null"; break;
    	case '\x07': result = "#\\alarm"; break;
    	case '\x08': result = "#\\backspace"; break;
    	case '\x09': result = "#\\tab"; break;
    	case '\x0A': result = "#\\newline"; break;
    	case '\x0D': result = "#\\return"; break;
    	case '\x1B': result = "#\\escape"; break;
    	case '\x20': result = "#\\space"; break;
    	case '\x7F': result = "#\\delete"; break;
        default: result = ""; break;
    }
    return result;
}


const
value_t CHARACTER_MAX = 0x0000007F00000002,
        CHARACTER_MIN = 0x0000000000000002;

value_t character_make_from_string(const std::string& str) {
    assert(str[0] == '#' && str[1] == '\\');
    value_t result = BOOLEAN_FALSE;
    size_t length = str.length();
    if (length == 3) {
        result = wrap_character(str[2]);
    }
    else if (str[2] == 'x' && length <= 5) {
        int code = 0;
        std::stringstream ss(str.substr(3));
        ss >> std::hex >> code;
        assert(code >= 0 && code <= 127);
        result = wrap_character((char)code);
    }
    else if (str == "#\\null") result = wrap_character('\x00');
    else if (str == "#\\alarm") result = wrap_character('\x07');
    else if (str == "#\\backspace") result = wrap_character('\x08');
    else if (str == "#\\tab") result = wrap_character('\x09');
    else if (str == "#\\newline") result = wrap_character('\x0A');
    else if (str == "#\\return") result = wrap_character('\x0D');
    else if (str == "#\\escape") result = wrap_character('\x1B');
    else if (str == "#\\space") result = wrap_character('\x20');
    else if (str == "#\\delete") result = wrap_character('\x7F');
    return result;
}

value_t character_preprocess(const std::string& original_str) {
    value_t result = character_make_from_string(original_str);
    if (result == BOOLEAN_FALSE) {
        error(1, 0, "Unknown character representation.");
    }
    return result; 
}

std::string character_format(value_t value) {
    char character = unwrap_character(value);
    std::string result = get_character_name(character);
    if (result == "") {
        std::stringstream sstream;
        sstream << "#\\";
        if (character < '\x20') {
            sstream << "x" << std::hex << (int) character;
        }
        else {
            sstream << character;
        }
        result = sstream.str();
    }
    return result;
}

