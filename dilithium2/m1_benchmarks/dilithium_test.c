#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "params.h"
#include "sign.h"
#include "randombytes.h"
#include "api.h"

#define NTESTS 1000
#define MLEN 32

static int test_sign(void)
{
    unsigned char pk[CRYPTO_PUBLICKEYBYTES];
    unsigned char sk[CRYPTO_SECRETKEYBYTES];
    unsigned char sm[MLEN + CRYPTO_BYTES];
    unsigned char m[MLEN];

    size_t mlen;
    size_t smlen;

    crypto_sign_keypair(pk, sk);

    randombytes(m, MLEN);
    crypto_sign(sm, &smlen, m, MLEN, sk);

    // By relying on m == sm we prevent having to allocate CRYPTO_BYTES twice
    if (crypto_sign_open(sm, &mlen, sm, smlen, pk))
    {
        printf("ERROR Signature did not verify correctly!\n");
        return -1;
    }

    return 0;
}

static int test_wrong_pk(void)
{
    unsigned char pk[CRYPTO_PUBLICKEYBYTES];
    unsigned char pk2[CRYPTO_PUBLICKEYBYTES];
    unsigned char sk[CRYPTO_SECRETKEYBYTES];
    unsigned char sm[MLEN + CRYPTO_BYTES];
    unsigned char m[MLEN];

    size_t mlen;
    size_t smlen;

    crypto_sign_keypair(pk2, sk);

    crypto_sign_keypair(pk, sk);

    randombytes(m, MLEN);
    crypto_sign(sm, &smlen, m, MLEN, sk);

    // By relying on m == sm we prevent having to allocate CRYPTO_BYTES twice
    if (crypto_sign_open(sm, &mlen, sm, smlen, pk2)){
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
  printf("Test successful\n");

  return 0;
}


