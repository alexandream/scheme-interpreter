#ifndef __ATOM_H__
#define __ATOM_H__

#include "cell.h"

static inline
cell_ptr_t ATOM_TYPE(const cell_t& cell) {
	return CAR(cell);
}

static inline
cell_ptr_t ATOM_TYPE(cell_ptr_t ptr) {
	return ATOM_TYPE(PTR_LOOKUP(ptr));
}

static inline
void ATOM_TYPE(cell_t& cell, cell_ptr_t type) {
	CAR(cell, type);
}

static inline
void ATOM_TYPE(cell_ptr_t ptr, cell_ptr_t type) {
	ATOM_TYPE(PTR_LOOKUP(ptr), type);
}

static inline
uint32_t ATOM_DATA(const cell_t& cell) {
	return (uint32_t) CDR(cell);
}

static inline
uint32_t ATOM_DATA(cell_ptr_t ptr) {
	return ATOM_DATA(PTR_LOOKUP(ptr));
}

static inline
void ATOM_DATA(cell_t& cell, uint32_t data) {
	CDR(cell, data);
}

static inline
void ATOM_DATA(cell_ptr_t ptr, uint32_t data) {
	ATOM_DATA(PTR_LOOKUP(ptr), data);
}

#endif

