#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "api.h"
#include "kem.h"
#include "indcpa.h"
#include "params.h"
#include "indcpa.h"
#include "poly.h"
#include "polyvec.h"
#include "ntt.h"
#include "m1cycles.h"

#define NTESTS 100000

uint8_t seed[KYBER_SYMBYTES] = {0};

#define TIME(s) s = rdtsc();
// Result is clock cycles
#define  CALC(start, stop) (stop - start) / NTESTS;

int main(void)
{
  unsigned int i;
  unsigned char pk[KYBER_AARCH64_CRYPTO_PUBLICKEYBYTES] = {0};
  unsigned char sk[KYBER_AARCH64_CRYPTO_SECRETKEYBYTES] = {0};
  unsigned char ct[KYBER_AARCH64_CRYPTO_CIPHERTEXTBYTES] = {0};
  unsigned char key_a[KYBER_AARCH64_CRYPTO_BYTES] = {0};
  unsigned char key_b[KYBER_AARCH64_CRYPTO_BYTES] = {0};


//   struct timespec start, stop;
  long long ns;
  long long start, stop;


// Init performance counter
  setup_rdtsc();
//

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_keypair(pk, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_keypair: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_enc(ct, key_b, pk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_enc: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_dec(key_a, ct, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_dec: %lld\n", ns);

//




  return 0;
}
