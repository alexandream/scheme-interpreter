#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <string>

#include "cell.h"

cell_ptr_t symbol_make_from_string(const std::string& str);
std::string symbol_display_format(cell_ptr_t cell);
#endif
