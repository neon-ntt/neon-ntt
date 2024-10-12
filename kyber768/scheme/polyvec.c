
/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include <stdint.h>
#include <stddef.h>
#include <arm_neon.h>
#include <string.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-statement-expression"

/*************************************************
* Name:        polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void polyvec_ntt(int16_t r[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_ntt(r[i]);
    }
}

/*************************************************
* Name:        neon_polyvec_invntt_to_mont
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*              and multiply by Montgomery factor 2^16
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void polyvec_invntt_to_mont(int16_t r[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_invntt_tomont(r[i]);
    }
}

void polyvec_add_reduce(int16_t c[KYBER_K][KYBER_N], const int16_t a[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_add_reduce(c[i], a[i]);
    }
}

#define SHIFT_WITH_HOLES_B(a, mask_lo, mask_hi, shift_i) ({ \
            int8x16_t val; \
            do { \
                val = (int8x16_t)vorrq_s16((int16x8_t)vandq_s8(a, mask_lo), \
                                  vshrq_n_s16((int16x8_t)vandq_s8(a, mask_hi), shift_i)); \
            } while(0); \
            val; \
        })

#define SHIFT_WITH_HOLES_B_LAZY(a, mask_hi, shift_i) ({ \
            int8x16_t val; \
            do { \
                val = (int8x16_t)vorrq_s16((int16x8_t)a, \
                                 vshrq_n_s16((int16x8_t)vandq_s8(a, mask_hi), shift_i)); \
            } while(0); \
            val; \
        })

#define SHIFT_WITH_HOLES_B_VERY_LAZY(a, shift_i) ({ \
            int8x16_t val; \
            do { \
                val = (int8x16_t)vorrq_s16((int16x8_t)a, \
                                 vshrq_n_s16((int16x8_t)a, shift_i)); \
            } while(0); \
            val; \
        })

#define SHIFT_WITH_HOLES_H(a, mask_lo, mask_hi, shift_i) ({ \
            int16x8_t val; \
            do { \
                val = (int16x8_t)vorrq_s32((int32x4_t)vandq_s16(a, mask_lo), \
                                 vshrq_n_s32((int32x4_t)vandq_s16(a, mask_hi), shift_i)); \
            } while(0); \
            val; \
        })

#define SHIFT_WITH_HOLES_S(a, mask_lo, mask_hi, shift_i) ({ \
            int32x4_t val; \
            do { \
                val = (int32x4_t)vorrq_s64((int64x2_t)vandq_s32(a, mask_lo), \
                                 vshrq_n_s64((int64x2_t)vandq_s32(a, mask_hi), shift_i)); \
            } while(0); \
            val; \
        })

#if (KYBER_K == 2 || KYBER_K == 3)
static
void poly_compress10_neon(uint8_t r[320], const int16_t a[KYBER_N]){

    uint16_t t[8];
    int16x8_t avec, tvec;
    int16x8_t mask10 = vdupq_n_s16(0x3ff);
    int16x8_t zero = vdupq_n_s16(0);

    int16x8_t mask_h_lo = {0x3ff, 0, 0x3ff, 0, 0x3ff, 0, 0x3ff, 0};
    int16x8_t mask_h_hi = {0, 0x3ff, 0, 0x3ff, 0, 0x3ff, 0, 0x3ff};
    int32x4_t mask_w_lo = {0xfffff, 0, 0xfffff, 0};
    int32x4_t mask_w_hi = {0, 0xfffff, 0, 0xfffff};

    for(size_t i = 0; i < KYBER_N / 8; i++){
        avec = vld1q_s16(a + 8 * i);

        tvec = vqdmulhq_n_s16(avec, -20553);
        tvec = vhaddq_s16(tvec, zero);
        tvec = vmlaq_n_s16(tvec, avec, 20);
        tvec = vrshrq_n_s16(tvec, 6);
        tvec = vandq_s16(tvec, mask10);

        tvec = SHIFT_WITH_HOLES_H(tvec, mask_h_lo, mask_h_hi, 6);
        tvec = (int16x8_t)SHIFT_WITH_HOLES_S((int32x4_t)tvec, mask_w_lo, mask_w_hi, 12);

        vst1q_s16((int16_t*)t, tvec);

        r[0] = t[0];
        r[1] = t[0] >> 8;
        r[2] = t[1];
        r[3] = t[1] >> 8;
        r[4] = t[2];
        r[5] = t[4];
        r[6] = t[4] >> 8;
        r[7] = t[5];
        r[8] = t[5] >> 8;
        r[9] = t[6];

        r += 10;
    }

}
#endif

#if KYBER_K == 4
static
void poly_compress11_neon(uint8_t r[352], const int16_t a[KYBER_N]){

    uint64_t lo64, hi64;
    __attribute__((aligned(16))) uint16_t t[8];
    int16x8_t avec, tvec;
    int16x8_t mask11 = vdupq_n_s16(0x7ff);
    int16x8_t zero = vdupq_n_s16(0);

    int16x8_t mask_h_lo = {0x7ff, 0, 0x7ff, 0, 0x7ff, 0, 0x7ff, 0};
    int16x8_t mask_h_hi = {0, 0x7ff, 0, 0x7ff, 0, 0x7ff, 0, 0x7ff};
    int32x4_t mask_w_lo = {0x3fffff, 0, 0x3fffff, 0};
    int32x4_t mask_w_hi = {0, 0x3fffff, 0, 0x3fffff};

    for(size_t i = 0; i < KYBER_N / 8; i++) {

        avec = vld1q_s16(a + 8 * i);

        tvec = vqdmulhq_n_s16(avec, -20553);
        tvec = vhaddq_s16(tvec, zero);
        tvec = vmlaq_n_s16(tvec, avec, 20);
        tvec = vrshrq_n_s16(tvec, 5);
        tvec = vandq_s16(tvec, mask11);

        tvec = SHIFT_WITH_HOLES_H(tvec, mask_h_lo, mask_h_hi, 5);
        tvec = (int16x8_t)SHIFT_WITH_HOLES_S((int32x4_t)tvec, mask_w_lo, mask_w_hi, 10);

        vst1q_s16((int16_t*)t, tvec);

        r[0] = t[0];
        r[1] = t[0] >> 8;
        r[2] = t[1];
        r[3] = t[1] >> 8;
        r[4] = t[2];

        memcpy(&lo64, t + 0, 8);
        memcpy(&hi64, t + 4, 8);
        hi64 = (hi64 << 4) | (lo64 >> 40);

        r[5] = hi64;
        r[6] = hi64 >> 8;
        r[7] = hi64 >> 16;
        r[8] = hi64 >> 24;
        r[9] = hi64 >> 32;
        r[10] = hi64 >> 40;

        r += 11;
    }

}
#endif

/*************************************************
* Name:        polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECCOMPRESSEDBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void polyvec_compress(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES], const int16_t a[KYBER_K][KYBER_N]) {

    #if KYBER_K == 4
    // uint16_t t[8];
    for (size_t i = 0; i < KYBER_K; i++) {
        poly_compress11_neon(r + i * 352, a[i]);
        // for (j = 0; j < KYBER_N / 8; j++) {
        //     for (k = 0; k < 8; k++) {
        //         u  = a[i][8 * j + k];

        //         // 21-bit suffices for round(2048 x / q)
        //         // inputs are in [-q/2, ..., q/2]
        //         // 1290167 = round(2048 * 2^21 / q)
        //         u = ((int16_t)(((int32_t)u * 1290167 + (1 << 20)) >> 21));
        //         t[k] = u & 0x7ff;

        //     }

        //     r[ 0] = (t[0] >>  0);
        //     r[ 1] = (t[0] >>  8) | (t[1] << 3);
        //     r[ 2] = (t[1] >>  5) | (t[2] << 6);
        //     r[ 3] = (t[2] >>  2);
        //     r[ 4] = (t[2] >> 10) | (t[3] << 1);
        //     r[ 5] = (t[3] >>  7) | (t[4] << 4);
        //     r[ 6] = (t[4] >>  4) | (t[5] << 7);
        //     r[ 7] = (t[5] >>  1);
        //     r[ 8] = (t[5] >>  9) | (t[6] << 2);
        //     r[ 9] = (t[6] >>  6) | (t[7] << 5);
        //     r[10] = (t[7] >>  3);
        //     r += 11;
        // }
    }
    #elif (KYBER_K == 2 || KYBER_K == 3)
    // uint16_t t[4];
    for (size_t i = 0; i < KYBER_K; i++) {
        poly_compress10_neon(r + i * 320, a[i]);
        // for (j = 0; j < KYBER_N / 4; j++) {
        //     for (k = 0; k < 4; k++) {
        //         u  = a[i][4 * j + k];

        //         // 22-bit suffices for round(1024 x / q)
        //         // inputs are in [-q/2, ..., q/2]
        //         // 1290167 = round(1024 * 2^22 / q)
        //         u = ((int16_t)(((int32_t)u * 1290167 + (1 << 21)) >> 22));
        //         t[k] = u & 0x3ff;

        //     }

        //     r[0] = (t[0] >> 0);
        //     r[1] = (t[0] >> 8) | (t[1] << 2);
        //     r[2] = (t[1] >> 6) | (t[2] << 4);
        //     r[3] = (t[2] >> 4) | (t[3] << 6);
        //     r[4] = (t[3] >> 2);
        //     r += 5;
        // }
    }
    #else
#error "KYBER_POLYVECCOMPRESSEDBYTES needs to be in {320*KYBER_K, 352*KYBER_K}"
    #endif
}

/*************************************************
* Name:        polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of polyvec_compress
*
* Arguments:   - polyvec *r:       pointer to output vector of polynomials
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYVECCOMPRESSEDBYTES)
**************************************************/
void polyvec_decompress(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES]) {
    unsigned int i, j, k;

    #if KYBER_K == 4
    uint16_t t[8];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 8; j++) {
            t[0] = (a[0] >> 0) | ((uint16_t)a[ 1] << 8);
            t[1] = (a[1] >> 3) | ((uint16_t)a[ 2] << 5);
            t[2] = (a[2] >> 6) | ((uint16_t)a[ 3] << 2) | ((uint16_t)a[4] << 10);
            t[3] = (a[4] >> 1) | ((uint16_t)a[ 5] << 7);
            t[4] = (a[5] >> 4) | ((uint16_t)a[ 6] << 4);
            t[5] = (a[6] >> 7) | ((uint16_t)a[ 7] << 1) | ((uint16_t)a[8] << 9);
            t[6] = (a[8] >> 2) | ((uint16_t)a[ 9] << 6);
            t[7] = (a[9] >> 5) | ((uint16_t)a[10] << 3);
            a += 11;

            for (k = 0; k < 8; k++) {
                r[i][8 * j + k] = ((uint32_t)(t[k] & 0x7FF) * KYBER_Q + 1024) >> 11;
            }
        }
    }
    #elif (KYBER_K == 2 || KYBER_K == 3)
    uint16_t t[4];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 4; j++) {
            t[0] = (a[0] >> 0) | ((uint16_t)a[1] << 8);
            t[1] = (a[1] >> 2) | ((uint16_t)a[2] << 6);
            t[2] = (a[2] >> 4) | ((uint16_t)a[3] << 4);
            t[3] = (a[3] >> 6) | ((uint16_t)a[4] << 2);
            a += 5;

            for (k = 0; k < 4; k++) {
                r[i][4 * j + k] = ((uint32_t)(t[k] & 0x3FF) * KYBER_Q + 512) >> 10;
            }
        }
    }
    #else
#error "KYBER_POLYVECCOMPRESSEDBYTES needs to be in {320*KYBER_K, 352*KYBER_K}"
    #endif
}

/*************************************************
* Name:        polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void polyvec_tobytes(uint8_t r[KYBER_POLYVECBYTES], const int16_t a[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_tobytes(r + i * KYBER_POLYBYTES, a[i]);
    }
}

/*************************************************
* Name:        polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of polyvec_tobytes
*
* Arguments:   - uint8_t *r:       pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
*                                  (of length KYBER_POLYVECBYTES)
**************************************************/
void polyvec_frombytes(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECBYTES]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_frombytes(r[i], a + i * KYBER_POLYBYTES);
    }
}

#pragma GCC diagnostic pop


