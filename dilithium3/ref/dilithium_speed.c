
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
#include "params.h"
#include "sign.h"
#include "randombytes.h"
#include "cycles.h"
#include "api.h"

#include "poly.h"
#include "polyvec.h"

// #define NTESTS 1000000
#define NTESTS 10000
#define MLEN 32
#define CTXLEN 14

#define TIME(s) s = get_cycle();
// Result is clock cycles
#define  CALC(start, stop) (stop - start) / NTESTS;

static unsigned char m[NTESTS][MLEN];

int main(void)
{
  unsigned int i;

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

//   struct timespec start, stop;
  uint64_t ns;
  uint64_t start, stop;


// Init performance counter
  init_counter();
//

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_sign_keypair(pk, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_sign_keypair: %llu\n", ns);


  for(i=0;i<NTESTS;i++){
    randombytes(m[i], MLEN);
  }

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_sign(sm, &smlen, m[i], MLEN, ctx, CTXLEN, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_sign: %llu\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_sign_open(m[i], &mlen, sm, smlen, ctx, CTXLEN, pk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_sign_open: %llu\n", ns);


  TIME(start);
  for(i=0;i<NTESTS;i++) {
    polyvecl_ntt(&s1hat);
    polyvec_matrix_pointwise_montgomery(&t1, mat, &s1hat);
    polyveck_reduce(&t1);
    polyveck_invntt_tomont(&t1);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("MatrixVectorMul: %llu\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    poly_ntt(&t);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("NTT: %llu\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    polyvecl_pointwise_acc_montgomery(&t, &mat[0], &s1hat);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("long base_mul acc: %llu\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    poly_invntt_tomont(&t);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("iNTT: %llu\n", ns);




  return 0;
}

