#ifndef __BOOLEAN_H__
#define __BOOLEAN_H__

#include <string>

#include "cell.h"


cell_ptr_t boolean_make_from_string(const std::string& str);

std::string boolean_display_format(cell_ptr_t cell);

#endif
