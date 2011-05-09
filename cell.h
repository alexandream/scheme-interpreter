#ifndef __CELL_H__
#define __CELL_H__

#include "value.h"

struct cell_t {
	value_t car;
	value_t cdr;
};
