#ifndef REDUCE_H
#define REDUCE_H
#include "params.h"
#include <stdint.h>

#define DILITHIUM_QINV 58728449 // q^(-1) mod 2^32

int32_t montgomery_reduce(int64_t a);

int32_t reduce32(int32_t a);

int32_t caddq(int32_t a);

int32_t freeze(int32_t a);

#endif
