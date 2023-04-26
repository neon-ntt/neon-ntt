
/*
 * This file is dual licensed
 * under Apache 2.0 or public domain at https://github.com/pq-crystals/dilithium
 */

#ifndef ROUNDING_H
#define ROUNDING_H
#include "params.h"
#include <stdint.h>

int32_t power2round(int32_t *a0, int32_t a);

int32_t decompose(int32_t *a0, int32_t a);

unsigned int make_hint(int32_t a0, int32_t a1);

int32_t use_hint(int32_t a, unsigned int hint);

#endif
