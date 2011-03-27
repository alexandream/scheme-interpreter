#include <sstream>
#include <string>
#include <cassert>
#include <map>

#include "symbol.h"
#include "atom.h"

typedef std::map<std::string, cell_ptr_t> symbol_map_t;

static 
symbol_map_t symbol_pool;

// FIXME: This needs to point to a node that represents the type.
const cell_ptr_t SYMBOL_TYPE = 0;

cell_ptr_t make_symbol_cell(const std::string& str) {
	cell_ptr_t result;
	cell_t& cell = cell_alloc(result);
	META_TYPE(cell, CELL_META_TYPE_SIMPLE_ATOM);
	ATOM_TYPE(cell, SYMBOL_TYPE);
	ATOM_DATA(cell, (uint32_t) new std::string(str));
	GC_INFO(cell, CELL_GC_FORCE_MARK);
	return result;
}

cell_ptr_t symbol_make_from_string(const std::string& str) {
	// ATTENTION: This assumes that checking whether the string represents a
	// symbol or not was already done by an upper layer, like the lexer.
		
	symbol_map_t::iterator iter = symbol_pool.find(str);
	
	cell_ptr_t result;
	if (iter != symbol_pool.end()) {
		result = iter->second;
	}
	else {
		result = make_symbol_cell(str);
		symbol_pool[str] = result;
	}
	return result;
}	


std::string symbol_display_format(cell_ptr_t ptr) {
	// ATTENTION: This assumes that checking if the cell is a symbol cell is
	// already done by an upper layer, like the dispatcher.
	return "'" + *((std::string*) ATOM_DATA(ptr)); }
