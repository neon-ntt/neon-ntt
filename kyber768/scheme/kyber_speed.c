#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "api.h"
#include "kem.h"
#include "indcpa.h"
#include "params.h"
#include "indcpa.h"
#include "poly.h"
#include "polyvec.h"
#include "ntt.h"

#if __APPLE__
#define __AVERAGE__
#else
#define __MEDIAN__
#endif
#include "cycles.h"

#define NTESTS 1000000

uint64_t time0, time1;
uint64_t cycles[NTESTS];

int main(void){

    uint8_t pk[KYBER_AARCH64_CRYPTO_PUBLICKEYBYTES] = {0};
    uint8_t sk[KYBER_AARCH64_CRYPTO_SECRETKEYBYTES] = {0};
    uint8_t ct[KYBER_AARCH64_CRYPTO_CIPHERTEXTBYTES] = {0};
    uint8_t key_a[KYBER_AARCH64_CRYPTO_BYTES] = {0};
    uint8_t key_b[KYBER_AARCH64_CRYPTO_BYTES] = {0};

    int16_t A[KYBER_K][KYBER_K][KYBER_N];
    int16_t s[KYBER_K][KYBER_N];
    int16_t s_asymmetric[KYBER_K][KYBER_N];
    int16_t acc[KYBER_K][KYBER_N];

    int32_t poly1[KYBER_K][KYBER_N];
    int32_t poly2[KYBER_K][KYBER_N];
    int32_t poly2_asymmetric[KYBER_K][KYBER_N >> 1];
    int32_t polyout[KYBER_K][KYBER_N];

    // Init performance counter
    init_counter();

    WRAP_FUNC("crypto_kem_keypair",
              cycles, time0, time1,
              crypto_kem_keypair(pk, sk));
    WRAP_FUNC("crypto_kem_enc",
              cycles, time0, time1,
              crypto_kem_enc(ct, key_b, pk));
    WRAP_FUNC("crypto_kem_dec",
              cycles, time0, time1,
              crypto_kem_dec(key_a, ct, sk));

    WRAP_FUNC("NTT cycles",
              cycles, time0, time1,
              NTT((int16_t *)(&poly1[0][0])));

    WRAP_FUNC("NTT_heavy",
              cycles, time0, time1,
              NTT((int16_t *)(&poly2[0][0])); \
              KYBER_AARCH64__asm_point_mul_extended((int16_t *)(&poly2_asymmetric[0][0]), (int16_t *)(&poly2[0][0]), pre_asymmetric_table_extended, asymmetric_const));

    WRAP_FUNC("point_mul",
              cycles, time0, time1,
              KYBER_AARCH64__asm_point_mul_extended((int16_t *)(&poly2_asymmetric[0][0]), (int16_t *)(&poly2[0][0]), pre_asymmetric_table_extended, asymmetric_const));

    WRAP_FUNC("asymmetric_mul (dim x base_mul)",
              cycles, time0, time1,
              KYBER_AARCH64__asm_asymmetric_mul((int16_t *)polyout, (int16_t *)(&poly1[0][0]), (int16_t *)(&poly2[0][0]), (int16_t *)(&poly2_asymmetric[0][0]), asymmetric_const));

    WRAP_FUNC("iNTT",
              cycles, time0, time1,
              iNTT((int16_t *)(&poly1[0][0])));

    WRAP_FUNC("MatrixVectorMul",
              cycles, time0, time1,
              for (size_t j = 0; j < KYBER_K; j++){ \
                  NTT(&(s[0][0])); \
                  KYBER_AARCH64__asm_point_mul_extended(&(s_asymmetric[j][0]), &(s[j][0]), pre_asymmetric_table_extended, asymmetric_const); \
              } \
              for (size_t j = 0; j < KYBER_K; j++){ \
                  KYBER_AARCH64__asm_asymmetric_mul(&(acc[j][0]), &(A[j][0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const); \
              } \
              for (size_t j = 0; j < KYBER_K; j++){ \
                  iNTT(&(acc[j][0])); \
              });

    WRAP_FUNC("InnerProd (Enc)",
              cycles, time0, time1,
              KYBER_AARCH64__asm_asymmetric_mul(&(acc[0][0]), &(A[0][0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const); \
              iNTT(&(acc[0][0])));

    WRAP_FUNC("InnerProd (Dec)",
              cycles, time0, time1,
              for (size_t j = 0; j < KYBER_K; j++){ \
                  NTT(&(s[0][0])); \
                  KYBER_AARCH64__asm_point_mul_extended(&(s_asymmetric[j][0]), &(s[j][0]), pre_asymmetric_table_extended, asymmetric_const); \
              } \
              KYBER_AARCH64__asm_asymmetric_mul(&(acc[0][0]), &(A[0][0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const); \
              iNTT(&(acc[0][0])));

    return 0;

}
