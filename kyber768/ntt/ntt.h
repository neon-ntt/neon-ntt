#ifndef NTT_H
#define NTT_H

/*
 * CC0 1.0 Universal or the following MIT License
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
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
#include <stdint.h>

#include "NTT_params.h"

extern const int16_t zetas[128];

void ntt(int16_t r[256]);

void invntt(int16_t r[256]);

extern void __asm_ntt_SIMD_top(int16_t *, const int16_t *, const int16_t *);
extern void __asm_ntt_SIMD_bot(int16_t *, const int16_t *, const int16_t *);

extern void __asm_ntt_SIMD_top_schedule(int16_t *, const int16_t *, const int16_t *);
extern void __asm_ntt_SIMD_bot_schedule(int16_t *, const int16_t *, const int16_t *);

extern void __asm_intt_SIMD_bot(int16_t *, const int16_t *, const int16_t *);
extern void __asm_intt_SIMD_top(int16_t *, const int16_t *, const int16_t *);

extern void __asm_intt_SIMD_bot_schedule(int16_t *, const int16_t *, const int16_t *);
extern void __asm_intt_SIMD_top_schedule(int16_t *, const int16_t *, const int16_t *);


extern void __asm_point_mul_extended(int16_t *, const int16_t *, const int16_t *, const int16_t *);
extern void __asm_asymmetric_mul(const int16_t *, const int16_t *, const int16_t *, const int16_t *, int16_t *);
extern void __asm_asymmetric_mul_montgomery(const int16_t *, const int16_t *, const int16_t *, const int16_t *, int16_t *);

extern
const int16_t asymmetric_const[16];
extern
const int16_t constants[16];
extern
const int16_t streamlined_CT_negacyclic_table_Q1_extended[(NTT_N + (1 << 0) + (1 << 4) + NTT_N) << 1];
extern
const int16_t streamlined_CT_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4) + NTT_N) << 1];
extern
const int16_t pre_asymmetric_table_Q1_extended[ARRAY_N];
extern
const int16_t streamlined_inv_GS_negacyclic_table_Q1_extended[(NTT_N + (1 << 0) + (1 << 4) + NTT_N) << 1];
extern
const int16_t streamlined_inv_GS_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4) + NTT_N) << 1];

#define NTT(in) { \
        __asm_ntt_SIMD_top(in, streamlined_CT_negacyclic_table_Q1_extended, constants); \
        __asm_ntt_SIMD_bot(in, streamlined_CT_negacyclic_table_Q1_extended, constants); \
    }

#define NTT_schedule(in) { \
        __asm_ntt_SIMD_top_schedule(in, streamlined_CT_negacyclic_table_Q1_jump_extended, constants); \
        __asm_ntt_SIMD_bot_schedule(in, streamlined_CT_negacyclic_table_Q1_jump_extended, constants); \
    }

#define iNTT(in) { \
        __asm_intt_SIMD_bot(in, streamlined_inv_GS_negacyclic_table_Q1_extended, constants); \
        __asm_intt_SIMD_top(in, streamlined_inv_GS_negacyclic_table_Q1_extended, constants); \
    }

#define iNTT_schedule(in) { \
        __asm_intt_SIMD_bot_schedule(in, streamlined_inv_GS_negacyclic_table_Q1_jump_extended, constants); \
        __asm_intt_SIMD_top_schedule(in, streamlined_inv_GS_negacyclic_table_Q1_jump_extended, constants); \
    }



#endif
