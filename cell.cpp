#include "cell.h"
#include <iostream>
cell_t* cell_pool = 0;
uint32_t cell_pool_size = 0;
uint32_t cell_pool_last = 0;

static const
uint32_t CELL_POOL_MAX_SIZE = 65536;

const 
uint32_t CELL_CAR_MASK       = 0x0FFFFFFF, // Mask the flag bits off
         CELL_FLAG_MASK      = 0xF0000000, // Mask the flag bits on
         CELL_META_TYPE_MASK = 0xC0000000,
         CELL_GC_INFO_MASK   = 0x30000000,
         CELL_GC_STATE_MASK  = 0x10000000,
         CELL_GC_MARK_MASK   = 0x20000000;

const
uint8_t CELL_META_TYPE_CONS_CELL   = 0x00,
        CELL_META_TYPE_SIMPLE_ATOM = 0x01,
        CELL_META_TYPE_EXTENDED    = 0x02;


const
cell_ptr_t NIL         = 0x0FFFFFFF,
           TRUE        = 0x0FFFFFFE,
           FALSE       = 0x0FFFFFFD,
           UNDEFINED   = 0x0FFFFFFC,
           UNSPECIFIED = 0x0FFFFFFB;


cell_t NIL_CELL         = { 0x40000000, 0x00000000 },
       TRUE_CELL        = { 0x40000000, 0x00000000 },
       FALSE_CELL       = { 0x40000000, 0x00000000 },
       UNDEFINED_CELL   = { 0x40000000, 0x00000000 },
       UNSPECIFIED_CELL = { 0x40000000, 0x00000000 };


__attribute__ ((constructor))
static void cell_pool_init(void) {
	cell_pool = new cell_t[CELL_POOL_MAX_SIZE];
	cell_pool_size = CELL_POOL_MAX_SIZE;
}

cell_t& cell_alloc(cell_ptr_t& ptr) {
	assert(cell_pool_last < cell_pool_size);

	ptr = cell_pool_last++;

	return cell_pool[ptr];
}


cell_t& cell_alloc(void) {
	cell_ptr_t unused;
	return cell_alloc(unused);
}

