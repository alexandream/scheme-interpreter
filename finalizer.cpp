#include "finalizer.h"

#include "environment.h"
#include "string.h"



void finalize(value_t value) {
//	printf("Finalizing %s at: 0x%Lx\n", get_type(value), value);
	
    if (is_environment(value)) {
        environment_finalize(value);
    }
    else if (is_string(value)) {
        string_finalize(value);
    }
}
