#ifndef NTT_H
#define NTT_H

/*
 * This file was originally licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/dilithium/tree/master/ref
 *
 * We choose
 * CC0 1.0 Universal or the following MIT License
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 * Copyright (c) 2023: Vincent Hwang
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

#include <stdint.h>
#include "NTT_params.h"

extern void __asm_ntt_SIMD_top(int32_t *des, const int32_t *table, const int32_t *_constants);
extern void __asm_ntt_SIMD_bot(int32_t *des, const int32_t *table, const int32_t *_constants);

extern void __asm_intt_SIMD_top(int32_t *des, const int32_t *table, const int32_t *_constants);
extern void __asm_intt_SIMD_bot(int32_t *des, const int32_t *table, const int32_t *_constants);

extern
const int32_t constants[16];

extern
const int32_t streamlined_CT_negacyclic_table_Q1_extended[(NTT_N + (1 << 0) + (1 << 4)) << 1];

extern
const int32_t streamlined_CT_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1];

extern
const int32_t streamlined_GS_itable_Q1_extended[(NTT_N + (1 << 0) + (1 << 4)) << 1];

extern
const int32_t streamlined_GS_itable_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1];

#define NTT(in) do { \
        __asm_ntt_SIMD_top(in, streamlined_CT_negacyclic_table_Q1_jump_extended, constants); \
        __asm_ntt_SIMD_bot(in, streamlined_CT_negacyclic_table_Q1_jump_extended, constants); \
    } while(0)

#define iNTT(in) do { \
        __asm_intt_SIMD_bot(in, streamlined_GS_itable_Q1_jump_extended, constants); \
        __asm_intt_SIMD_top(in, streamlined_GS_itable_Q1_jump_extended, constants); \
    } while(0)

void ntt(int32_t a[ARRAY_N]);
void invntt_tomont(int32_t a[ARRAY_N]);


#endif
