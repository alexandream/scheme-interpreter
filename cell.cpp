#include "cell.h"


const 
uint32_t CELL_CONTENT_MASK      = 0x0FFFFFFF, // Mask the flag bits off
         CELL_FLAG_MASK      = 0xF0000000, // Mask the flag bits on
         CELL_META_TYPE_MASK = 0xC0000000,
         CELL_GC_INFO_MASK   = 0x30000000,
         CELL_GC_STATE_MASK  = 0x10000000,
         CELL_GC_MARK_MASK   = 0x20000000;

const
uint8_t CELL_META_TYPE_CONS_CELL   = 0x00,
        CELL_META_TYPE_SIMPLE_ATOM = 0x01,
        CELL_META_TYPE_EXTENDED    = 0x02;
