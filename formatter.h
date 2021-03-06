#ifndef __FORMATTER_H__
#define __FORMATTER_H__

#include <string>

#include "value.h"

std::string format(value_t);
const char* get_type(value_t value);

#endif
