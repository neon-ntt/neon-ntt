
/*
 * CC0 1.0 Universal or the following MIT License
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"
#include "config.h"
#include "params.h"
#include "sign.h"
#include "randombytes.h"
#include "cycles.h"

#include "poly.h"
#include "polyvec.h"

// #define NTESTS 1000000
#define NTESTS 10000
#define MLEN 32
#define CTXLEN 14

uint64_t time0, time1;
uint64_t cycles[NTESTS];

#define __AVERAGE__
// #define __MEDIAN__

#ifdef __AVERAGE__

#define LOOP_INIT(__clock0, __clock1) { \
    __clock0 = get_cycle(); \
}
#define LOOP_TAIL(__f_string, records, __clock0, __clock1) { \
    __clock1 = get_cycle(); \
    printf(__f_string " average cycles: %lld\n", (__clock1 - __clock0) / NTESTS); \
}
#define BODY_INIT(__clock0, __clock1) {}
#define BODY_TAIL(records, __clock0, __clock1) {}

#elif defined(__MEDIAN__)

#include <stdlib.h>

static int cmp_uint64(const void *a, const void *b){
    return ((*((const uint64_t*)a)) - ((*((const uint64_t*)b))));
}

#define LOOP_INIT(__clock0, __clock1) {}
#define LOOP_TAIL(__f_string, records, __clock0, __clock1) { \
    qsort(records, sizeof(uint64_t), NTESTS, cmp_uint64); \
    printf(__f_string " median cycles: %lld\n", records[NTESTS >> 1]); \
}
#define BODY_INIT(__clock0, __clock1) { \
    __clock0 = get_cycle(); \
}
#define BODY_TAIL(records, __clock0, __clock1) { \
    __clock1 = get_cycle(); \
    records[i] = __clock1 - __clock0; \
}

#endif

#define WRAP_FUNC(__f_string, records, __clock0, __clock1, func){ \
    LOOP_INIT(__clock0, __clock1); \
    for(size_t i = 0; i < NTESTS; i++){ \
        BODY_INIT(__clock0, __clock1); \
        func; \
        BODY_TAIL(records, __clock0, __clock1); \
    } \
    LOOP_TAIL(__f_string, records, __clock0, __clock1); \
}

static unsigned char m[NTESTS][MLEN];

int main(void){

    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t sm[MLEN + CRYPTO_BYTES];
    uint8_t ctx[CTXLEN] = {0};

    poly t;

    polyvecl mat[K];
    polyvecl s1hat;
    polyveck t1;

    size_t mlen;
    size_t smlen;

    // Init performance counter
    init_counter();

    WRAP_FUNC("crypto_sign_keypair",
              cycles, time0, time1,
              crypto_sign_keypair(pk, sk));

    for(size_t i = 0; i < NTESTS; i++){
        randombytes(m[i], MLEN);
    }

    WRAP_FUNC("crypto_sign",
              cycles, time0, time1,
              crypto_sign(sm, &smlen, m[i], MLEN, ctx, CTXLEN, sk));

    WRAP_FUNC("crypto_sign_open",
              cycles, time0, time1,
              crypto_sign_open(m[i], &mlen, sm, smlen, ctx, CTXLEN, pk));

    WRAP_FUNC("MatrixVectorMul",
              cycles, time0, time1,
              polyvecl_ntt(&s1hat); \
              polyvec_matrix_pointwise_montgomery(&t1, mat, &s1hat); \
              polyveck_reduce(&t1); \
              polyveck_invntt_tomont(&t1));

    WRAP_FUNC("NTT",
              cycles, time0, time1,
              poly_ntt(&t));

    WRAP_FUNC("long base_mul acc",
              cycles, time0, time1,
              polyvecl_pointwise_acc_montgomery(&t, &mat[0], &s1hat));

    WRAP_FUNC("iNTT",
              cycles, time0, time1,
              poly_invntt_tomont(&t));

  return 0;
}

