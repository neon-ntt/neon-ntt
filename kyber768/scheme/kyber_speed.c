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
#include "cycles.h"

#define NTESTS 100000

uint8_t seed[KYBER_SYMBYTES] = {0};

#define TIME(s) s = get_cycle();
// Result is clock cycles
#define  CALC(start, stop) (stop - start) / NTESTS;

static int cmp_uint64(const void *a, const void *b){
    return ((*((const uint64_t*)a)) - ((*((const uint64_t*)b))));
}

#ifdef __AVERAGE__

#define LOOP_INIT(__clock0, __clock1) { \
    __clock0 = GET_TIME; \
}
#define LOOP_TAIL(__f_string, records, __clock0, __clock1) { \
    __clock1 = GET_TIME; \
    printf(__f_string, (__clock1 - __clock0) / NTESTS); \
}
#define BODY_INIT(__clock0, __clock1) {}
#define BODY_TAIL(records, __clock0, __clock1) {}

#elif defined(__MEDIAN__)

#define LOOP_INIT(__clock0, __clock1) {}
#define LOOP_TAIL(__f_string, records, __clock0, __clock1) { \
    qsort(records, sizeof(uint64_t), NTESTS, cmp_uint64); \
    printf(__f_string, records[NTESTS >> 1]); \
}
#define BODY_INIT(__clock0, __clock1) { \
    __clock0 = GET_TIME; \
}
#define BODY_TAIL(records, __clock0, __clock1) { \
    __clock1 = GET_TIME; \
    records[i] = __clock1 - __clock0; \
}

#endif

int main(void)
{
  unsigned int i;
  uint8_t pk[KYBER_AARCH64_CRYPTO_PUBLICKEYBYTES] = {0};
  uint8_t sk[KYBER_AARCH64_CRYPTO_SECRETKEYBYTES] = {0};
  uint8_t ct[KYBER_AARCH64_CRYPTO_CIPHERTEXTBYTES] = {0};
  uint8_t key_a[KYBER_AARCH64_CRYPTO_BYTES] = {0};
  uint8_t key_b[KYBER_AARCH64_CRYPTO_BYTES] = {0};


//   struct timespec start, stop;
  uint64_t ns;
  uint64_t start, stop;


// Init performance counter
  init_counter();
//

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_keypair(pk, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_keypair: %llu\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_enc(ct, key_b, pk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_enc: %llu\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_dec(key_a, ct, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_dec: %llu\n", ns);

//




  return 0;
}
