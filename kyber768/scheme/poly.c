
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

#include <stdio.h>

#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "reduce.h"
#include "cbd.h"
#include "symmetric.h"

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
    unsigned int i, j;
    int16_t u;
    uint8_t t[8];

    #if (KYBER_POLYCOMPRESSEDBYTES == 128)
    for (i = 0; i < KYBER_N / 8; i++) {
        for (j = 0; j < 8; j++) {
            u  = a[8 * i + j];

            // 16-bit precision suffices for round(2^4 x / q)
            // inputs are in [-q/2, ..., q/2]
            // 315 = round(16 * 2^16 / q)
            u = (int16_t)(((int32_t)u * 315 + (1 << 15)) >> 16);
            t[j] = u & 0xf;

        }

        r[0] = t[0] | (t[1] << 4);
        r[1] = t[2] | (t[3] << 4);
        r[2] = t[4] | (t[5] << 4);
        r[3] = t[6] | (t[7] << 4);
        r += 4;
    }
    #elif (KYBER_POLYCOMPRESSEDBYTES == 160)
    for (i = 0; i < KYBER_N / 8; i++) {
        for (j = 0; j < 8; j++) {
            u  = a[8 * i + j];

            // 15-bit precision suffices for round(2^5 x / q)
            // inputs are in [-q/2, ..., q/2]
            // 315 = round(32 * 2^15 / q)
            u = (int16_t)(((int32_t)u * 315 + (1 << 14)) >> 15);
            t[j] = u & 0x1f;

        }

        r[0] = (t[0] >> 0) | (t[1] << 5);
        r[1] = (t[1] >> 3) | (t[2] << 2) | (t[3] << 7);
        r[2] = (t[3] >> 1) | (t[4] << 4);
        r[3] = (t[4] >> 4) | (t[5] << 1) | (t[6] << 6);
        r[4] = (t[6] >> 2) | (t[7] << 3);
        r += 5;
    }
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
void poly_frombytes(int16_t r[KYBER_N], const uint8_t a[KYBER_POLYBYTES]) {
    uint8x16x3_t neon_buf;
    uint16x8x4_t tmp;
    int16x8x4_t value;
    uint16x8_t const_0xfff;
    const_0xfff = vdupq_n_u16(0xfff);

    unsigned int i, j = 0;
    for (i = 0; i < KYBER_POLYBYTES; i += 48) {
        neon_buf = vld3q_u8(&a[i]);

        // Val0: 0-1 | 3-4 | 6-7| 9-10
        tmp.val[0] = (uint16x8_t)vzip1q_u8(neon_buf.val[0], neon_buf.val[1]);
        tmp.val[1] = (uint16x8_t)vzip2q_u8(neon_buf.val[0], neon_buf.val[1]);

        tmp.val[0] = vandq_u16(tmp.val[0], const_0xfff);
        tmp.val[1] = vandq_u16(tmp.val[1], const_0xfff);

        // Val1: 1-2 | 4-5 | 7-8 | 10-11
        tmp.val[2] = (uint16x8_t)vzip1q_u8(neon_buf.val[1], neon_buf.val[2]);
        tmp.val[3] = (uint16x8_t)vzip2q_u8(neon_buf.val[1], neon_buf.val[2]);

        tmp.val[2] = vshrq_n_u16(tmp.val[2], 4);
        tmp.val[3] = vshrq_n_u16(tmp.val[3], 4);

        // Final value
        value.val[0] = (int16x8_t)vzip1q_u16(tmp.val[0], tmp.val[2]);
        value.val[1] = (int16x8_t)vzip2q_u16(tmp.val[0], tmp.val[2]);
        value.val[2] = (int16x8_t)vzip1q_u16(tmp.val[1], tmp.val[3]);
        value.val[3] = (int16x8_t)vzip2q_u16(tmp.val[1], tmp.val[3]);

        vst1q_s16_x4(r + j, value);
        j += 32;
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

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], const int16_t a[KYBER_N]) {
    unsigned int i, j;
    int16_t u;
    uint16_t t;

    for (i = 0; i < KYBER_N / 8; i++) {
        msg[i] = 0;
        for (j = 0; j < 8; j++) {
            u = a[8 * i + j];

            // 19-bit precision suffices for round(2 x / q)
            // inputs are in [-q/2, ..., q/2]
            // 315 = round(2 * 2^19 / q)
            u = (int16_t)(((int32_t)u * 315 + (1 << 18)) >> 19);
            t = u & 1;

            msg[i] |= t << j;
        }
    }
}
