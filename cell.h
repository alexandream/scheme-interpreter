#ifndef __CELL_H__
#define __CELL_H__

#include <stdint.h>
#include <cassert>

/* The contents of cell_t is more than the struct defined below. For starters,
 * it's "car" value uses it's upper 4 bits to store that cell's flags. The
 * flags are those that follow:
 * [ 31  | 30  | 29  | 28  | ... | 02  | 01  | 00  ] bits
 *    |     |     |     |
 *    |     |     |     +-- GC INFO LOWER BIT (STATE)
 *    |     |     +-- GC INFO UPPER BIT (MARK)
 *    |     +-- META-TYPE LOWER BIT
 *    +-- META-TYPE UPPER BIT
 * 
 * Where those are explained below:
 *
 *   GC INFO is a 2 bit value with following values:
 *      00 - State A: Unvisited cell.
 *      11 - State B: Have just visited cell and it's car part.
 *      10 - State C: Cell's cdr part visited. Completely visisted the cell.
 *      01 - State X: Cell is always "rooted". A GC pass will always mark.
 *         
 *   META TYPE is a 2 bit value that encode what "class" of element this cell
 *   represents. It can have the following values:
 *
 *      00: Cell is a true CONS CELL. Lower 28 bits of "car" point to it's car
 *          part, while lower 28 bits of "cdr" point to it's cdr part. The 
 *          remaining 4 bits in the cdr part are currently unused.
 *     
 *      01: Cell is an SIMPLE ATOM: one that needs only the storage inside the
 *          cell itself. The lower 28 bits of "car" point to a cell describing
 *          it's type. The whole cdr part is used to encode it's value.
 *          Some kinds of SIMPLE ATOMS are types like fixnums up to 32 bits,
 *          character values, symbols and floating point numbers with single
 *          precision (IEEE 754-1985)
 *     
 *      10: Cell is an EXTENDED VALUE: one that needs space outside the pool
 *          of allocated cells. The lower 28 bits of "car" point to a cell
 *          describing it's type. The lower 30 bits of "cdr" point to an index
 *          in secondary memory store used for extended memory usage. The upper
 *          2 bits of "cdr" are a new set of flags that encode the following
 *          information:
 *          
 *          00: Extended memory area can't be used to hold pointers to other
 *              cells and should, therefore, not be scanned by GC mark phase.
 *              This is mostly used for strings and user-defined low level
 *              types.
 *
 *          01: Extended memory area will be used to point to other cells,
 *              therefore the GC must also scan it during the mark phase.
 *              This is used for implementing things like vectors and 
 *              simple user-defined types (like structs).
 *
 *          10: Currently not used. Reserved for I'll probably need it later.
 *          11: Currently not used. Reserved for I'll probably need it later.
 *     
 *      11: Currently not used.
 *
 * 
 *
 *
 */

struct cell_t {
	uint32_t car;
	uint32_t cdr;
};

typedef uint32_t cell_ptr_t;

extern cell_t* cell_pool;
extern uint32_t cell_pool_size;

cell_t& cell_alloc(void);
cell_t& cell_alloc(cell_ptr_t& ptr);

extern const 
uint32_t CELL_CAR_MASK, 
         CELL_FLAG_MASK, 
         CELL_META_TYPE_MASK,
         CELL_GC_INFO_MASK,
         CELL_GC_STATE_MASK,
         CELL_GC_MARK_MASK;

extern const
uint8_t CELL_META_TYPE_CONS_CELL,
        CELL_META_TYPE_SIMPLE_ATOM,
        CELL_META_TYPE_EXTENDED;

extern const
cell_ptr_t NIL,
           TRUE,
           FALSE,
           UNDEFINED,
           UNSPECIFIED;

extern
cell_t NIL_CELL,
       TRUE_CELL,
       FALSE_CELL,
       UNDEFINED_CELL,
       UNSPECIFIED_CELL;


static inline 
cell_ptr_t CAR(const cell_t& cell) {
	return (cell.car & CELL_CAR_MASK);
}

static inline
void CAR(cell_t& cell, cell_ptr_t value) {
	cell.car |= (value & CELL_CAR_MASK);
}

static inline
cell_ptr_t CDR(const cell_t& cell) {
	return cell.cdr;
}

static inline
void CDR(cell_t& cell, cell_ptr_t value) {
	cell.cdr = value;
}
static inline
uint8_t FLAGS(const cell_t& cell) {
	// Mask the flag bits on and shifts down the flags to fit on 8 bits;
	return (uint8_t) ((cell.car & CELL_FLAG_MASK) >> 28);
}

static inline
uint8_t META_TYPE(const cell_t& cell) {
	return (uint8_t) ((cell.car & CELL_META_TYPE_MASK) >> 30);
}

static inline
void META_TYPE(cell_t& cell, uint8_t meta_type) {
	cell.car = (cell.car & CELL_CAR_MASK) | ((uint32_t) meta_type << 30);
}

static inline
uint8_t GC_INFO(const cell_t& cell) {
	return (uint8_t) ((cell.car & CELL_GC_INFO_MASK) >> 28);
}

static inline
bool GC_STATE(const cell_t& cell) {
	return (bool) ((cell.car & CELL_GC_STATE_MASK) >> 28);
}

static inline
bool GC_MARK(const cell_t& cell) {
	return (bool) ((cell.car & CELL_GC_MARK_MASK) >> 29);
}
//----------------------------------------------------------------------
//--  PTR BASED FUNCTIONS  ---------------------------------------------
//----------------------------------------------------------------------

static inline
bool SPECIAL_P(cell_ptr_t ptr) {
	return ((uint32_t) ptr) > 0x0FFFFFF0;
}
static inline
cell_t& PTR_LOOKUP(cell_ptr_t ptr) {
	if (! SPECIAL_P(ptr)) {
		assert(ptr < cell_pool_size);
		return cell_pool[ptr];
	}
	return NIL_CELL;
}

static inline
cell_ptr_t CAR(cell_ptr_t ptr) {
	return CAR(PTR_LOOKUP(ptr));
}

static inline
void CAR(cell_ptr_t ptr, cell_ptr_t car) {
	CAR(PTR_LOOKUP(ptr), car);
}

static inline
cell_ptr_t CDR(cell_ptr_t ptr) {
	return CDR(PTR_LOOKUP(ptr));
}

static inline
void CDR(cell_ptr_t ptr, cell_ptr_t cdr) {
	CDR(PTR_LOOKUP(ptr), cdr);
}

static inline
uint8_t FLAGS(cell_ptr_t ptr) {
	return FLAGS(PTR_LOOKUP(ptr));
}

static inline
uint8_t META_TYPE(cell_ptr_t ptr) {
	return META_TYPE(PTR_LOOKUP(ptr));
}

static inline
void META_TYPE(cell_ptr_t ptr, uint8_t meta_type) {
	META_TYPE(PTR_LOOKUP(ptr), meta_type);
}

static inline
uint8_t GC_INFO(cell_ptr_t ptr) {
	return GC_INFO(PTR_LOOKUP(ptr));
}

static inline
bool GC_STATE(cell_ptr_t ptr) {
	return GC_STATE(PTR_LOOKUP(ptr));
}




#endif
