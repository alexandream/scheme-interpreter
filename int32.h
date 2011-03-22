#ifndef __INT32_H__
#define __INT32_H__

#include <string>

#include "cell.h"


void int32_init_cell_from_string(cell_t& result, const std::string& str);
std::string int32_display_format(const cell_t& cell);
#endif
