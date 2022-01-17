#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "params.h"
#include "sign.h"
#include "randombytes.h"
#include "m1cycles.h"
#include "api.h"


#include "poly.h"
#include "polyvec.h"

#define NTESTS 1000000
// #define NTESTS 10000
#define MLEN 32

#define TIME(s) s = rdtsc();
// Result is clock cycles
#define  CALC(start, stop) (stop - start) / NTESTS;


static unsigned char m[NTESTS][MLEN];

int main()
{
  unsigned int i;

  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char sk[CRYPTO_SECRETKEYBYTES];
  unsigned char sm[MLEN + CRYPTO_BYTES];

  poly t;

  polyvecl mat[K];
  polyvecl s1hat;
  polyveck t1;

  size_t mlen;
  size_t smlen;

//   struct timespec start, stop;
  long long ns;
  long long start, stop;


// Init performance counter
  setup_rdtsc();
//

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_sign_keypair(pk, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_sign_keypair: %lld\n", ns);


  for(i=0;i<NTESTS;i++){
    randombytes(m[i], MLEN);
  }

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_sign(sm, &smlen, m[i], MLEN, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_sign: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_sign_open(m[i], &mlen, sm, smlen, pk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_sign_open: %lld\n", ns);


  TIME(start);
  for(i=0;i<NTESTS;i++) {
    polyvecl_ntt(&s1hat);
    polyvec_matrix_pointwise_montgomery(&t1, mat, &s1hat);
    polyveck_reduce(&t1);
    polyveck_invntt_tomont(&t1);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("MatrixVectorMul: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    poly_ntt(&t);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("NTT: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    polyvecl_pointwise_acc_montgomery(&t, &mat[0], &s1hat);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("long base_mul acc: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    poly_invntt_tomont(&t);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("iNTT: %lld\n", ns);




  return 0;
}

