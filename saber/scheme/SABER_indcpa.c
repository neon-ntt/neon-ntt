/*=============================================================================
This file has been adapted from the implementation
(available at, Public Domain https://github.com/KULeuven-COSIC/SABER)
of "Saber: Module-LWR based key exchange, CPA-secure encryption and CCA-secure KEM"
by : Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, and Frederik Vercauteren
Jose Maria Bermudo Mera, Michiel Van Beirendonck, Andrea Basso.
=============================================================================*/

#include <arm_neon.h>

#include <string.h>
#include "SABER_indcpa.h"
#include "pack_unpack.h"
#include "randombytes.h"
#include "fips202.h"
#include "cbd.h"
#include "SABER_params.h"

#include "NTT_params.h"
#include "ntt.h"

#define h1 (1 << (SABER_EQ - SABER_EP - 1))
#define h2 ((1 << (SABER_EP - 2)) - (1 << (SABER_EP - SABER_ET - 1)) + (1 << (SABER_EQ - SABER_EP - 1)))

extern void __asm_10_to_32(uint32_t *des, const uint8_t *src);
extern void __asm_13_to_32(uint32_t *des, const uint8_t *src);
extern void __asm_16_to_32(uint32_t *des, const uint16_t *src);
extern void __asm_round(uint16_t *des, const uint32_t *src);
extern void __asm_enc_add_msg(uint16_t *des, const uint32_t *src, const uint16_t *msg, const uint16_t const_h1);
extern void __asm_dec_get_msg(uint16_t *des, const uint32_t *src, const uint16_t *c_msg, const uint16_t const_h2);

void indcpa_kem_keypair(uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t sk[SABER_INDCPA_SECRETKEYBYTES]){


    uint16_t s[SABER_L][SABER_N];

    uint32_t A_NTT[SABER_L][SABER_L][SABER_N];
    uint32_t s_NTT[SABER_L][SABER_N];
    uint32_t s_NTT_asymmetric[SABER_L][SABER_N];
    uint16_t acc[SABER_L][SABER_N];

    uint8_t seed_A[SABER_SEEDBYTES];
    uint8_t seed_s[SABER_NOISE_SEEDBYTES];

    uint8_t shake_A_buf[SABER_L * SABER_POLYVECBYTES];
    uint8_t shake_s_buf[SABER_L * SABER_POLYCOINBYTES];

    randombytes(seed_A, SABER_SEEDBYTES);
    shake128(seed_A, SABER_SEEDBYTES, seed_A, SABER_SEEDBYTES); // for not revealing system RNG state
    randombytes(seed_s, SABER_NOISE_SEEDBYTES);

    shake128(shake_A_buf, sizeof(shake_A_buf), seed_A, SABER_SEEDBYTES);
    shake128(shake_s_buf, sizeof(shake_s_buf), seed_s, SABER_NOISE_SEEDBYTES);

    // expand A^T
    for(int i = 0; i < SABER_L; i++){
        for(int j = 0; j < SABER_L; j++){
            __asm_13_to_32(&(A_NTT[j][i][0]), shake_A_buf + (i * SABER_L + j) * SABER_POLYBYTES);
        }
    }

    // expand s
    for(int i = 0; i < SABER_L; i++){
        cbd(&(s[i][0]), shake_s_buf + i * SABER_POLYCOINBYTES);
        __asm_16_to_32(&(s_NTT[i][0]), &(s[i][0]));
    }

    // NTT(A^T)
    for(int i = 0; i < SABER_L; i++){
        for(int j = 0; j < SABER_L; j++){
            NTT(&(A_NTT[i][j][0]));
        }
    }

    // ( NTT(s), NTT_asymmetric(s) ) = NTT_heavy(s)
    for(int i = 0; i < SABER_L; i++){
        NTT_heavy(&(s_NTT_asymmetric[i][0]), &(s_NTT[i][0]));
    }

    // asymmetric_mul ( NTT(A^T), NTT(s), NTT_asymmetric(s)  )
    for(int i = 0; i < SABER_L; i++){
        __asm_asymmetric_mul(&(A_NTT[i][0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);
    }

    // iNTT giving A^T s
    for(int i = 0; i < SABER_L; i++){
        iNTT(&(A_NTT[i][0][0]));
    }

    // round for b
    for(int i = 0; i < SABER_L; i++){
        __asm_round(&(acc[i][0]), &(A_NTT[i][0][0]));
    }


    // pack
    POLVECq2BS(sk, s);
    POLVECp2BS(pk, acc);
    memcpy(pk + SABER_POLYVECCOMPRESSEDBYTES, seed_A, sizeof(seed_A));
}

void indcpa_kem_enc(const uint8_t m[SABER_KEYBYTES], const uint8_t seed_sp[SABER_NOISE_SEEDBYTES], const uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t ciphertext[SABER_BYTES_CCA_DEC]){

    uint16_t sp[SABER_L][SABER_N];
    uint16_t bp[SABER_L][SABER_N] = {0};
    uint16_t vp[SABER_N] = {0};
    uint16_t mp[SABER_N];
    const uint8_t *seed_A = pk + SABER_POLYVECCOMPRESSEDBYTES;

    uint32_t A_NTT[SABER_L][SABER_L][SABER_N];
    uint32_t b_NTT[SABER_L][SABER_N];
    uint32_t s_NTT[SABER_L][SABER_N];
    uint32_t s_NTT_asymmetric[SABER_L][SABER_N];

    uint8_t shake_A_buf[SABER_L * SABER_POLYVECBYTES];
    uint8_t shake_s_buf[SABER_L * SABER_POLYCOINBYTES];

    shake128(shake_A_buf, sizeof(shake_A_buf), seed_A, SABER_SEEDBYTES);
    shake128(shake_s_buf, sizeof(shake_s_buf), seed_sp, SABER_NOISE_SEEDBYTES);

    // expand A
    for(int i = 0; i < SABER_L; i++){
        for(int j = 0; j < SABER_L; j++){
            __asm_13_to_32(&(A_NTT[i][j][0]), shake_A_buf + (i * SABER_L + j) * SABER_POLYBYTES);
        }
    }

    // expand s'
    for(int i = 0; i < SABER_L; i++){
        cbd(&(sp[i][0]), shake_s_buf + i * SABER_POLYCOINBYTES);
        __asm_16_to_32(&(s_NTT[i][0]), &(sp[i][0]));
    }

    // expand b
    for(int i = 0; i < SABER_L; i++){
      __asm_10_to_32(&(b_NTT[i][0]), pk + i * (SABER_EP * SABER_N / 8));
    }

    // ( NTT(s'), NTT_asymmetric(s') ) = NTT_heavy(s')
    for(int i = 0; i < SABER_L; i++){
        NTT_heavy(&(s_NTT_asymmetric[i][0]), &(s_NTT[i][0]));
    }

    // NTT(A)
    for(int i = 0; i < SABER_L; i++){
        for(int j = 0; j < SABER_L; j++){
            NTT(&(A_NTT[i][j][0]));
        }
    }

    // asymmetric_mul( NTT(A), NTT(s'), NTT_asymmetric(s') )
    for(int i = 0; i < SABER_L; i++){
        __asm_asymmetric_mul(&(A_NTT[i][0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);
    }

    // iNTT giving A s'
    for(int i = 0; i < SABER_L; i++){
        iNTT(&(A_NTT[i][0][0]));
    }

    // round for b'
    for(int i = 0; i < SABER_L; i++){
        __asm_round(&(bp[i][0]), &(A_NTT[i][0][0]));
    }

    // NTT(b)
    for(int i = 0; i < SABER_L; i++){
        NTT(&(b_NTT[i][0]));
    }

    // asymmetric_mul( NTT(b^T), NTT(s'), NTT_asymmetric(s') )
    __asm_asymmetric_mul(&(b_NTT[0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);

    // iNTT giving b^T s'
    iNTT(&(b_NTT[0][0]));

    // expand m
    BS2POLmsg(m, mp);
    // round for v'
    __asm_enc_add_msg(&(vp[0]), &(b_NTT[0][0]), &(mp[0]), h1);

    // pack
    POLVECp2BS(ciphertext, bp);
    POLT2BS(ciphertext + SABER_POLYVECCOMPRESSEDBYTES, vp);

}

void indcpa_kem_dec(const uint8_t sk[SABER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[SABER_BYTES_CCA_DEC], uint8_t m[SABER_KEYBYTES])
{

    uint16_t v[SABER_N] = {0};
    uint16_t cm[SABER_N];

    uint32_t cipher_NTT[SABER_L][SABER_N];
    uint32_t s_NTT[SABER_L][SABER_N];
    uint32_t s_NTT_asymmetric[SABER_L][SABER_N];

    // expand c
    BS2POLT(ciphertext + SABER_POLYVECCOMPRESSEDBYTES, cm);

    // expand b'
    for(int i = 0; i < SABER_L; i++){
        __asm_10_to_32(&(cipher_NTT[i][0]), ciphertext + i * (SABER_EP * SABER_N / 8));
    }

    // expand s
    for(int i = 0; i < SABER_L; i++){
        __asm_13_to_32(&(s_NTT[i][0]), sk + i * SABER_POLYBYTES);
    }

    // NTT(b')
    for(int i = 0; i < SABER_L; i++){
        NTT(&(cipher_NTT[i][0]));
    }

    // ( NTT(s), NTT_asymmetric(s) ) = NTT_heavy(s)
    for(int i = 0; i < SABER_L; i++){
        NTT_heavy(&(s_NTT_asymmetric[i][0]), &(s_NTT[i][0]));
    }

    // asymmetric_mul( NTT(b'), NTT(s), NTT_asymmetric(s) )
    __asm_asymmetric_mul(&(cipher_NTT[0][0]), &(s_NTT[0][0]), &(s_NTT_asymmetric[0][0]), constants);

    // iNTT giving b'^T s
    iNTT(&(cipher_NTT[0][0]));

    // round for m
    __asm_dec_get_msg(&(v[0]), &(cipher_NTT[0][0]), &(cm[0]), h2);

    // pack
    POLmsg2BS(m, v);
}


