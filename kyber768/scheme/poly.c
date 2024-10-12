
/*
 * This file was originally licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/blob/master/ref
 *
 * We offer
 * CC0 1.0 Universal or the following MIT License for this file.
 * You may freely choose one of them that applies.
 *
 * MIT License
 *
 * Copyright (c) 2024: Vincent Hwang
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 *
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

#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "reduce.h"
#include "cbd.h"
#include "symmetric.h"

#include <arm_neon.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-statement-expression"

/*************************************************
* Name:        poly_getnoise_eta1
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA1
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void poly_getnoise_eta1(int16_t r[KYBER_N], const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce)
{
  uint8_t buf[KYBER_ETA1 * KYBER_N / 4];
  prf(buf, sizeof(buf), seed, nonce);
  poly_cbd_eta1(r, buf);
}

/*************************************************
* Name:        poly_getnoise_eta1_x2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA1
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void poly_getnoise_eta1_x2(int16_t vec1[KYBER_N], int16_t vec2[KYBER_N],
                                const uint8_t seed[KYBER_SYMBYTES],
                                uint8_t nonce1, uint8_t nonce2) {
    uint8_t buf1[KYBER_ETA1 * KYBER_N / 4],
            buf2[KYBER_ETA1 * KYBER_N / 4];
    prfx2(buf1, buf2, sizeof(buf1), seed, nonce1, nonce2);
    poly_cbd_eta1(vec1, buf1);
    poly_cbd_eta1(vec2, buf2);
}

/*************************************************
* Name:        poly_getnoise_eta2_x2
*              poly_getnoise_eta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA2
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void poly_getnoise_eta2_x2(int16_t vec1[KYBER_N], int16_t vec2[KYBER_N],
                                const uint8_t seed[KYBER_SYMBYTES],
                                uint8_t nonce1, uint8_t nonce2) {
    uint8_t buf1[KYBER_ETA2 * KYBER_N / 4],
            buf2[KYBER_ETA2 * KYBER_N / 4];
    prfx2(buf1, buf2, sizeof(buf1), seed, nonce1, nonce2);
    poly_cbd_eta2(vec1, buf1);
    poly_cbd_eta2(vec2, buf2);
}

void poly_getnoise_eta2(int16_t r[KYBER_N],
                             const uint8_t seed[KYBER_SYMBYTES],
                             uint8_t nonce) {
    uint8_t buf[KYBER_ETA2 * KYBER_N / 4];
    prf(buf, sizeof(buf), seed, nonce);
    poly_cbd_eta2(r, buf);
}

/*************************************************
* Name:        poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *r: pointer to in/output polynomial
**************************************************/
void poly_ntt(int16_t r[KYBER_N]) {
    ntt(r);
}

/*************************************************
* Name:        poly_invntt_tomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void poly_invntt_tomont(int16_t r[KYBER_N]) {
    invntt(r);
}

/*************************************************
* Name:        poly_add_reduce
*
* Description: Add two polynomials; no modular reduction is performed
*              Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
extern void KYBER_AARCH64__asm_add_reduce(int16_t *, const int16_t *);
void poly_add_reduce(int16_t c[KYBER_N], const int16_t a[KYBER_N]) {
    KYBER_AARCH64__asm_add_reduce(c, a);
}

extern void KYBER_AARCH64__asm_add_add_reduce(int16_t *, const int16_t *, const int16_t *);
void poly_add_add_reduce(int16_t c[KYBER_N], const int16_t a[KYBER_N], const int16_t b[KYBER_N]) {
    KYBER_AARCH64__asm_add_add_reduce(c, a, b);
}

/*************************************************
* Name:        poly_sub_reduce
*
* Description: Subtract two polynomials; no modular reduction is performed
*              Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
extern void KYBER_AARCH64__asm_sub_reduce(int16_t *, const int16_t *);
void poly_sub_reduce(int16_t c[KYBER_N], const int16_t a[KYBER_N]) {
    KYBER_AARCH64__asm_sub_reduce(c, a);
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

#if (KYBER_POLYCOMPRESSEDBYTES == 128)
static
void poly_compress4_neon(uint8_t r[128], const int16_t a[KYBER_N]){

    int16x8_t tvec[4];
    int16x8_t mask4 = vdupq_n_s16(0xf);
    int16x8_t one = vdupq_n_s16(1);
#if __ARM_FEATURE_DOTPROD
    int8x16_t dot_v = {1, 0, 16, 0, 1, 0, 16, 0, 1, 0, 16, 0, 1, 0, 16, 0};
    int32x4_t zero_int32x4 = {0, 0, 0, 0};
#endif

    for(size_t i = 0; i < KYBER_N / 32; i++) {

        tvec[0] = vld1q_s16(a + i * 32 + 8 * 0);
        tvec[1] = vld1q_s16(a + i * 32 + 8 * 1);
        tvec[2] = vld1q_s16(a + i * 32 + 8 * 2);
        tvec[3] = vld1q_s16(a + i * 32 + 8 * 3);

        tvec[0] = vqdmulhq_n_s16(tvec[0], 315);
        tvec[1] = vqdmulhq_n_s16(tvec[1], 315);
        tvec[2] = vqdmulhq_n_s16(tvec[2], 315);
        tvec[3] = vqdmulhq_n_s16(tvec[3], 315);

        tvec[0] = vhaddq_s16(tvec[0], one);
        tvec[1] = vhaddq_s16(tvec[1], one);
        tvec[2] = vhaddq_s16(tvec[2], one);
        tvec[3] = vhaddq_s16(tvec[3], one);

        tvec[0] = vandq_s16(tvec[0], mask4);
        tvec[1] = vandq_s16(tvec[1], mask4);
        tvec[2] = vandq_s16(tvec[2], mask4);
        tvec[3] = vandq_s16(tvec[3], mask4);

#if __ARM_FEATURE_DOTPROD

        tvec[0] = (int16x8_t)vdotq_s32(zero_int32x4, (int8x16_t)tvec[0], dot_v);
        tvec[1] = (int16x8_t)vdotq_s32(zero_int32x4, (int8x16_t)tvec[1], dot_v);
        tvec[2] = (int16x8_t)vdotq_s32(zero_int32x4, (int8x16_t)tvec[2], dot_v);
        tvec[3] = (int16x8_t)vdotq_s32(zero_int32x4, (int8x16_t)tvec[3], dot_v);

        tvec[0] = vmovn_high_s32(vmovn_s32((int32x4_t)tvec[0]), (int32x4_t)tvec[1]);
        tvec[2] = vmovn_high_s32(vmovn_s32((int32x4_t)tvec[2]), (int32x4_t)tvec[3]);

        tvec[0] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[0]), tvec[2]);

#else

        tvec[0] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[0]), tvec[1]);
        tvec[2] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[2]), tvec[3]);

        tvec[0] = (int16x8_t)SHIFT_WITH_HOLES_B_VERY_LAZY((int8x16_t)tvec[0], 4);
        tvec[2] = (int16x8_t)SHIFT_WITH_HOLES_B_VERY_LAZY((int8x16_t)tvec[2], 4);

        tvec[0] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[0]), tvec[2]);

#endif

        vst1q_s16((int16_t*)r, tvec[0]);

        r += 16;
    }

}
#endif

#if (KYBER_POLYCOMPRESSEDBYTES == 160)
static
void poly_compress5_neon(uint8_t r[160], const int16_t a[KYBER_N]){

    int16x8_t tvec[2];
    int16x8_t mask5 = vdupq_n_s16(0x1f);
    int16x8_t mask_h_lo = {0x3ff, 0, 0x3ff, 0, 0x3ff, 0, 0x3ff, 0};
    int16x8_t mask_h_hi = {0, 0x3ff, 0, 0x3ff, 0, 0x3ff, 0, 0x3ff};
    int32x4_t mask_w_lo = {0xfffff, 0, 0xfffff, 0};
    int32x4_t mask_w_hi = {0, 0xfffff, 0, 0xfffff};
#if __ARM_FEATURE_DOTPROD
    int8x16_t dot_v = {1, 0, 32, 0, 1, 0, 32, 0, 1, 0, 32, 0, 1, 0, 32, 0};
    int32x4_t zero_int32x4 = {0, 0, 0, 0};
#else
    int8x16_t mask_b_lo = {0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0};
    int8x16_t mask_b_hi = {0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f, 0, 0x1f};
#endif

    uint16_t t[2][8];
    for(size_t i = 0; i < KYBER_N / 16; i++) {

        tvec[0] = vld1q_s16(a + 16 * i + 8 * 0);
        tvec[1] = vld1q_s16(a + 16 * i + 8 * 1);
        tvec[0] = vqrdmulhq_n_s16(tvec[0], 315);
        tvec[1] = vqrdmulhq_n_s16(tvec[1], 315);
        tvec[0] = vandq_s16(tvec[0], mask5);
        tvec[1] = vandq_s16(tvec[1], mask5);

#if __ARM_FEATURE_DOTPROD

        tvec[0] = (int16x8_t)vdotq_s32(zero_int32x4, (int8x16_t)tvec[0], dot_v);
        tvec[1] = (int16x8_t)vdotq_s32(zero_int32x4, (int8x16_t)tvec[1], dot_v);

        tvec[0] = vmovn_high_s32(vmovn_s32((int32x4_t)tvec[0]), (int32x4_t)tvec[1]);

        tvec[0] = SHIFT_WITH_HOLES_H(tvec[0], mask_h_lo, mask_h_hi, 6);
        tvec[0] = (int16x8_t)SHIFT_WITH_HOLES_S((int32x4_t)tvec[0], mask_w_lo, mask_w_hi, 12);

#else

        tvec[0] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[0]), tvec[1]);

        tvec[0] = (int16x8_t)SHIFT_WITH_HOLES_B((int8x16_t)tvec[0], mask_b_lo, mask_b_hi, 3);
        tvec[0] = SHIFT_WITH_HOLES_H(tvec[0], mask_h_lo, mask_h_hi, 6);
        tvec[0] = (int16x8_t)SHIFT_WITH_HOLES_S((int32x4_t)tvec[0], mask_w_lo, mask_w_hi, 12);

#endif

        vst1q_s16((int16_t*)t[0], tvec[0]);

        r[0] = t[0][0];
        r[1] = t[0][0] >> 8;
        r[2] = t[0][1];
        r[3] = t[0][1] >> 8;
        r[4] = t[0][2];
        r[5] = t[0][4];
        r[6] = t[0][4] >> 8;
        r[7] = t[0][5];
        r[8] = t[0][5] >> 8;
        r[9] = t[0][6];

        r += 10;
    }

}
#endif

/*************************************************
* Name:        poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (of length KYBER_POLYCOMPRESSEDBYTES)
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_compress(uint8_t r[KYBER_POLYCOMPRESSEDBYTES], const int16_t a[KYBER_N]) {
#if (KYBER_POLYCOMPRESSEDBYTES == 128)
    poly_compress4_neon(r, a);
#elif (KYBER_POLYCOMPRESSEDBYTES == 160)
    poly_compress5_neon(r, a);
#else
#error "KYBER_POLYCOMPRESSEDBYTES needs to be in {128, 160}"
#endif
}

/*************************************************
* Name:        poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of poly_compress
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYCOMPRESSEDBYTES bytes)
**************************************************/
void poly_decompress(int16_t r[KYBER_N], const uint8_t a[KYBER_POLYCOMPRESSEDBYTES]) {
    unsigned int i;

    #if (KYBER_POLYCOMPRESSEDBYTES == 128)
    for (i = 0; i < KYBER_N / 2; i++) {
        r[2 * i + 0] = (((uint16_t)(a[0] & 15) * KYBER_Q) + 8) >> 4;
        r[2 * i + 1] = (((uint16_t)(a[0] >> 4) * KYBER_Q) + 8) >> 4;
        a += 1;
    }
    #elif (KYBER_POLYCOMPRESSEDBYTES == 160)
    unsigned int j;
    uint8_t t[8];
    for (i = 0; i < KYBER_N / 8; i++) {
        t[0] = (a[0] >> 0);
        t[1] = (a[0] >> 5) | (a[1] << 3);
        t[2] = (a[1] >> 2);
        t[3] = (a[1] >> 7) | (a[2] << 1);
        t[4] = (a[2] >> 4) | (a[3] << 4);
        t[5] = (a[3] >> 1);
        t[6] = (a[3] >> 6) | (a[4] << 2);
        t[7] = (a[4] >> 3);
        a += 5;

        for (j = 0; j < 8; j++) {
            r[8 * i + j] = ((uint32_t)(t[j] & 31) * KYBER_Q + 16) >> 5;
        }
    }
    #else
#error "KYBER_POLYCOMPRESSEDBYTES needs to be in {128, 160}"
    #endif
}

/*************************************************
* Name:        poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYBYTES bytes)
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tobytes(uint8_t r[KYBER_POLYBYTES], const int16_t a[KYBER_N]) {
    unsigned int i;
    uint16_t t0, t1;

    for (i = 0; i < KYBER_N / 2; i++) {
        // map to positive standard representatives
        t0  = a[2 * i];
        t0 += ((int16_t)t0 >> 15) & KYBER_Q;
        t1 = a[2 * i + 1];
        t1 += ((int16_t)t1 >> 15) & KYBER_Q;
        r[3 * i + 0] = (t0 >> 0);
        r[3 * i + 1] = (t0 >> 8) | (t1 << 4);
        r[3 * i + 2] = (t1 >> 4);
    }
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of KYBER_POLYBYTES bytes)
**************************************************/
void poly_frombytes(int16_t r[KYBER_N], const uint8_t a[KYBER_POLYBYTES]){
    for(size_t i = 0; i < KYBER_N / 2; i++) {
        r[2 * i    ] = ((a[3 * i + 0] >> 0) | ((uint16_t)a[3 * i + 1] << 8)) & 0xFFF;
        r[2 * i + 1] = ((a[3 * i + 1] >> 4) | ((uint16_t)a[3 * i + 2] << 4)) & 0xFFF;
    }
}

/*************************************************
* Name:        poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
**************************************************/
void poly_frommsg(int16_t r[KYBER_N], const uint8_t msg[KYBER_INDCPA_MSGBYTES]) {
    unsigned int i, j;
    int16_t mask;

    for (i = 0; i < KYBER_N / 8; i++) {
        for (j = 0; j < 8; j++) {
            mask = -(int16_t)((msg[i] >> j) & 1);
            r[8 * i + j] = mask & ((KYBER_Q + 1) / 2);
        }
    }
}

static
void poly_compress1_neon(uint8_t r[32], const int16_t a[KYBER_N]){

    int16x8_t tvec[16];
    int16x8_t mask1 = vdupq_n_s16(0x1);
#if __ARM_FEATURE_DOTPROD
    int8x16_t dot_v1 = {1, 2, 4, 8, 1, 2, 4, 8, 1, 2, 4, 8, 1, 2, 4, 8};
    int8x16_t dot_v4 = {1, 0, 16, 0, 1, 0, 16, 0, 1, 0, 16, 0, 1, 0, 16, 0};
    int32x4_t zero_int32x4 = {0, 0, 0, 0};
#endif

    for(size_t i = 0; i < KYBER_N / 128; i++){

        for(size_t j = 0; j < 16; j++){
            tvec[j] = vld1q_s16(a + i * 128 + 8 * j);
            tvec[j] = vqdmulhq_n_s16(tvec[j], 315);
            tvec[j] = vrshrq_n_s16(tvec[j], 4);
            tvec[j] = vandq_s16(tvec[j], mask1);
        }

#if __ARM_FEATURE_DOTPROD

        for(size_t j = 0; j < 16; j += 2){
            tvec[j] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[j]), tvec[j + 1]);
            tvec[j] = (int16x8_t)vdotq_s32(zero_int32x4, (int8x16_t)tvec[j], dot_v1);
        }

        for(size_t j = 0; j < 16; j += 4){
            tvec[j] = vmovn_high_s32(vmovn_s32((int32x4_t)tvec[j]), (int32x4_t)tvec[j + 2]);
            tvec[j] = (int16x8_t)vdotq_s32(zero_int32x4, (int8x16_t)tvec[j], dot_v4);
        }

        tvec[0] = (int16x8_t)vmovn_high_s32(vmovn_s32((int32x4_t)tvec[0]), (int32x4_t)tvec[4]);
        tvec[8] = (int16x8_t)vmovn_high_s32(vmovn_s32((int32x4_t)tvec[8]), (int32x4_t)tvec[12]);

        tvec[0] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[0]), tvec[8]);

#else

        for(size_t j = 0; j < 16; j += 2){
            tvec[j] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[j]), tvec[j + 1]);
            tvec[j] = (int16x8_t)SHIFT_WITH_HOLES_B_VERY_LAZY((int8x16_t)tvec[j], 7);
        }

        for(size_t j = 0; j < 16; j += 4){
            tvec[j] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[j]), tvec[j + 2]);
            tvec[j] = (int16x8_t)SHIFT_WITH_HOLES_B_VERY_LAZY((int8x16_t)tvec[j], 6);
        }

        tvec[0] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[0]), tvec[4]);
        tvec[8] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[8]), tvec[12]);
        tvec[0] = (int16x8_t)SHIFT_WITH_HOLES_B_VERY_LAZY((int8x16_t)tvec[0], 4);
        tvec[8] = (int16x8_t)SHIFT_WITH_HOLES_B_VERY_LAZY((int8x16_t)tvec[8], 4);

        tvec[0] = (int16x8_t)vmovn_high_s16(vmovn_s16(tvec[0]), tvec[8]);

#endif

        vst1q_u8(r, (uint8x16_t)tvec[0]);
        r += 16;

    }

}

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], const int16_t a[KYBER_N]) {
    poly_compress1_neon(msg, a);
}

#pragma GCC diagnostic pop
