#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "api.h"
#include "kem.h"
#include "cbd.h"
#include "SABER_params.h"
#include "fips202.h"
#include "fips202x2.h"
#include "NTT.h"
#include "m1cycles.h"

#define NTESTS 1000000

#define TIME(s) s = rdtsc();
// Result is clock cycles 
#define  CALC(start, stop) (stop - start) / NTESTS;

long long times[NTESTS + 1];
long long offset;

extern void __asm_10_to_32(uint32_t *des, const uint8_t *src);
extern void __asm_13_to_32(uint32_t *des, const uint8_t *src);
extern void __asm_16_to_32(uint32_t *des, const uint16_t *src);
extern void __asm_round(uint16_t *des, const uint32_t *src, const uint16_t const_h1);
extern void __asm_enc_add_msg(uint16_t *des, const uint32_t *src, const uint16_t *msg, const uint16_t const_h1);
extern void __asm_dec_get_msg(uint16_t *des, const uint32_t *src, const uint16_t *c_msg, const uint16_t const_h2);

uint8_t seed[SABER_SEEDBYTES] = {0};

int main()
{
  unsigned int i,j,k;
  unsigned char pk[CRYPTO_PUBLICKEYBYTES] = {0};
  unsigned char sk[CRYPTO_SECRETKEYBYTES] = {0};
  unsigned char ct[CRYPTO_CIPHERTEXTBYTES] = {0};
  unsigned char key[CRYPTO_BYTES] = {0};
  long long start, stop;
  long long ns;


  uint8_t seed_A[SABER_SEEDBYTES];
  uint8_t seed_s[SABER_NOISE_SEEDBYTES];

  uint8_t shake_A_buf[SABER_L * SABER_POLYVECBYTES];
  uint8_t shake_s_buf[SABER_L * SABER_POLYCOINBYTES];

  uint8_t shake_A_buf_dummy[SABER_L * SABER_POLYVECBYTES];
  uint8_t shake_s_buf_dummy[SABER_L * SABER_POLYVECBYTES];

  uint32_t A_NTT[SABER_L][SABER_L][SABER_N];
  uint32_t s_NTT_asymmetric[SABER_L][SABER_N];
  uint32_t s_NTT[SABER_L][SABER_N];
  uint32_t acc_NTT[SABER_L][SABER_N];

  // setup cycles
  setup_rdtsc();

// CCA

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_keypair(pk, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_keypair: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_enc(ct, key, pk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_enc: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_dec(key, ct, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_dec: %lld\n", ns);

// shake

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    shake128x2(shake_A_buf, shake_A_buf_dummy, sizeof(shake_A_buf), seed_A, seed_A, SABER_SEEDBYTES);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("shake128x2 matrix: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    shake128x2(shake_s_buf, shake_s_buf_dummy, sizeof(shake_s_buf), seed_s, seed_s, SABER_NOISE_SEEDBYTES);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("shake128x2 secret: %lld\n", ns);

    TIME(start);
  for(i=0;i<NTESTS;i++) {
    shake128(shake_A_buf, sizeof(shake_A_buf), seed_A, SABER_SEEDBYTES);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("shake128 matrix: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    shake128(shake_s_buf, sizeof(shake_s_buf), seed_s, SABER_NOISE_SEEDBYTES);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("shake128 secret: %lld\n", ns);

// MatrixVectorMul, InnerProd(Dec), InnerProd(Enc)

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    for(j=0;j<SABER_L;j++){
      for(k=0;k<SABER_L;k++){
        NTT(A_NTT[j][k]);
      }
    }
    for(j=0;j<SABER_L;j++){
      NTT_heavy(s_NTT_asymmetric[j], s_NTT[j]);
    }
    for(j=0;j<SABER_L;j++){
      __asm_asymmetric_mul(&(A_NTT[j][0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);
    }
    for(j=0;j<SABER_L;j++){
      iNTT(&(A_NTT[j][0][0]));
    }
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("MatrixVectorMul: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    for(j=0;j<SABER_L;j++){
      NTT(A_NTT[0][j]);
    }
    __asm_asymmetric_mul(&(A_NTT[0][0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);
    iNTT(&(A_NTT[0][0][0]));
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("InnerProd(Enc): %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    for(j=0;j<SABER_L;j++){
      NTT(A_NTT[0][j]);
    }
    for(j=0;j<SABER_L;j++){
      NTT_heavy(s_NTT_asymmetric[j], s_NTT[j]);
    }
    __asm_asymmetric_mul(&(A_NTT[0][0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);
    iNTT(&(A_NTT[0][0][0]));
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("InnerProd(Dec): %lld\n", ns);

// NTT, NTT_heavy, __asm_asymmetric_mul, iNTT

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    NTT(s_NTT[0]);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("NTT: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    NTT_heavy(s_NTT_asymmetric[0], s_NTT[0]);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("NTT_heavy: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    __asm_asymmetric_mul(&(A_NTT[0][0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("asymmetric_mul: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    iNTT(acc_NTT[0]);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("iNTT: %lld\n", ns);

  return 0;
}
