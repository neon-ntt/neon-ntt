
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

#define NTESTS 1000
#define MLEN 32
#define CTXLEN 14

static int test_sign(void)
{
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t sm[MLEN + CRYPTO_BYTES];
    uint8_t m[MLEN + CRYPTO_BYTES];
    uint8_t ctx[CTXLEN] = {0};

    size_t mlen;
    size_t smlen;

    crypto_sign_keypair(pk, sk);

    randombytes(m, MLEN);
    crypto_sign(sm, &smlen, m, MLEN, ctx, CTXLEN, sk);

    // By relying on m == sm we prevent having to allocate CRYPTO_BYTES twice
    if (crypto_sign_open(sm, &mlen, sm, smlen, ctx, CTXLEN, pk))
    {
        printf("ERROR Signature did not verify correctly!\n");
        return -1;
    }

    return 0;
}

static int test_wrong_pk(void)
{
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t pk2[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t sm[MLEN + CRYPTO_BYTES];
    uint8_t m[MLEN];
    uint8_t ctx[CTXLEN] = {0};

    size_t mlen;
    size_t smlen;

    crypto_sign_keypair(pk2, sk);

    crypto_sign_keypair(pk, sk);

    randombytes(m, MLEN);
    crypto_sign(sm, &smlen, m, MLEN, ctx, CTXLEN, sk);

    // By relying on m == sm we prevent having to allocate CRYPTO_BYTES twice
    if (crypto_sign_open(sm, &mlen, sm, smlen, ctx, CTXLEN, pk2)){
        return 0;
    }
    printf("ERROR Signature did verify correctly under wrong public key!\n");
    return -1;

}

int main(void)
{
  unsigned int i;
  int r;

  for(i=0;i<NTESTS;i++) {
    r  = test_sign();
    r |= test_wrong_pk();
    if(r)
      return 1;
  }

  printf("CRYPTO_SECRETKEYBYTES:  %d\n",CRYPTO_SECRETKEYBYTES);
  printf("CRYPTO_PUBLICKEYBYTES:  %d\n",CRYPTO_PUBLICKEYBYTES);
  printf("CRYPTO_BYTES:  %d\n",CRYPTO_BYTES);
  printf("Test successful\n");

  return 0;
}


