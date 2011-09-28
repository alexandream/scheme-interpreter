#include "finalizer.h"

#include "environment.h"




void finalize(value_t value) {
//	printf("Finalizing %s at: 0x%Lx\n", get_type(value), value);
	
    if (is_environment(value)) {
        environment_finalize(value);
    }
}
