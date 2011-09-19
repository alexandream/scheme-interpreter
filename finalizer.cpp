#include "finalizer.h"

#include "environment.h"

void finalize(value_t value) {
    if (is_environment(value)) {
        environment_finalize(value);
    }
}
