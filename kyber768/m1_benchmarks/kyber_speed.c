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

#include "NTT_params.h"

#define NTESTS 1000000

uint8_t seed[KYBER_SYMBYTES] = {0};

#define TIME(s) s = rdtsc();
// Result is clock cycles
#define  CALC(start, stop) (stop - start) / NTESTS;

int main(void)
{
  unsigned int i, j;
  unsigned char pk[KYBER_AARCH64_CRYPTO_PUBLICKEYBYTES] = {0};
  unsigned char sk[KYBER_AARCH64_CRYPTO_SECRETKEYBYTES] = {0};
  unsigned char ct[KYBER_AARCH64_CRYPTO_CIPHERTEXTBYTES] = {0};
  unsigned char key_a[KYBER_AARCH64_CRYPTO_BYTES] = {0};
  unsigned char key_b[KYBER_AARCH64_CRYPTO_BYTES] = {0};

  int16_t a[KYBER_K][KYBER_K][KYBER_N];
  int16_t vec[KYBER_K][KYBER_N];
  int16_t s[KYBER_K][KYBER_N];
  int16_t s_asymmetric[KYBER_K][KYBER_N >> 1];
  int16_t acc[KYBER_K][KYBER_N];

  int16_t poly[KYBER_N];


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

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    polyvec_ntt(s);
    for(j = 0; j < KYBER_K; j++){
      KYBER_AARCH64__asm_point_mul_extended(&(s_asymmetric[j][0]), &(s[j][0]), pre_asymmetric_table_Q1_extended, asymmetric_const);
    }
    for(j = 0; j < KYBER_K; j++){
      KYBER_AARCH64__asm_asymmetric_mul(&(a[j][0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const, acc[j]);
    }
    polyvec_invntt_to_mont(acc);

  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("MatrixVectorMul: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    KYBER_AARCH64__asm_asymmetric_mul(&(vec[0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const, poly);
    poly_invntt_tomont(poly);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("InnerProd (Enc): %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    polyvec_ntt(s);
    KYBER_AARCH64__asm_asymmetric_mul(&(vec[0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const, poly);
    poly_invntt_tomont(poly);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("InnerProd (Dec): %lld\n", ns);

//

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    NTT(poly);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("NTT: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    NTT(&(s[0][0]));
    KYBER_AARCH64__asm_point_mul_extended(&(s_asymmetric[0][0]), &(s[0][0]), pre_asymmetric_table_Q1_extended, asymmetric_const);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("NTT_heavy: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    KYBER_AARCH64__asm_asymmetric_mul(&(a[0][0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const, acc[0]);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("asymmetric_mul: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    iNTT(poly);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("iNTT: %lld\n", ns);



  return 0;
}
