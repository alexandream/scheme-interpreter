#ifndef __INT32_H__
#define __INT32_H__

#include <string>

#include "cell.h"


cell_ptr_t int32_make_from_string(const std::string& str);
std::string int32_display_format(cell_ptr_t cell);
#endif
