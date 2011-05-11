#include <iostream>
#include <assert.h>

#include "memory.h"
#include "pointer.h"
#include "cell.h"

static const int BLOCK_SIZE = 65536;

cell_t  *cell_pool = NULL;
uint32_t next_cell = 0;

value_t alloc_cell(void) {
	assert(next_cell < BLOCK_SIZE);
	if (!cell_pool) {
		cell_pool = new cell_t[BLOCK_SIZE];
		next_cell = 0;
	}
	void *ptr = (void*) (cell_pool + next_cell);
	value_t result = wrap_pointer(ptr);
	next_cell++;
	return result;
}

