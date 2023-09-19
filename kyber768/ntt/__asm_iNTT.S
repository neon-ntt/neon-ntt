
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

#include "macros.i"

.align 2
.global __asm_intt_SIMD_bot
.global ___asm_intt_SIMD_bot
__asm_intt_SIMD_bot:
___asm_intt_SIMD_bot:

    push_all
    Q         .req w20
    BarrettM  .req w21
    src0      .req x0
    src1      .req x1
    table     .req x28
    counter   .req x19

    ldrsh Q, [x2, #0]
    ldrsh BarrettM, [x2, #8]

    add table, x1, #64

    add src0, x0, #256*0
    add src1, x0, #256*1

    mov counter, #4
    _intt_bot_loop:

    ld4 {v16.4S, v17.4S, v18.4S, v19.4S}, [src0]
    ld4 {v20.4S, v21.4S, v22.4S, v23.4S}, [src1]

    trn1 v24.4S, v16.4S, v20.4S
    ld2 { v0.8H,  v1.8H}, [table], #32
    trn2 v28.4S, v16.4S, v20.4S
    ld2 { v2.8H,  v3.8H}, [table], #32
    trn1 v25.4S, v17.4S, v21.4S
    ld2 { v4.8H,  v5.8H}, [table], #32
    trn2 v29.4S, v17.4S, v21.4S
    ld2 { v6.8H,  v7.8H}, [table], #32
    trn1 v26.4S, v18.4S, v22.4S
    ld2 { v8.8H,  v9.8H}, [table], #32
    trn2 v30.4S, v18.4S, v22.4S
    ld2 {v10.8H, v11.8H}, [table], #32
    trn1 v27.4S, v19.4S, v23.4S
    ld2 {v12.8H, v13.8H}, [table], #32
    trn2 v31.4S, v19.4S, v23.4S
    ld2 {v14.8H, v15.8H}, [table], #32

    dup v0.8H, Q
    mov v1.H[1], BarrettM

    do_butterfly_vec_bot v28, v30, v18, v19, v29, v31,  v0, v12, v13, v14, v15
    do_butterfly_vec_mix_rev v28, v30, v18, v19, v29, v31, v24, v26, v16, v17, v25, v27,  v0, v12, v13, v14, v15,  v8,  v9, v10, v11
    do_butterfly_vec_mix_rev v24, v26, v16, v17, v25, v27, v28, v29, v18, v19, v30, v31,  v0,  v8,  v9, v10, v11,  v6,  v7,  v6,  v7
    do_butterfly_vec_mix_rev v28, v29, v18, v19, v30, v31, v24, v25, v16, v17, v26, v27,  v0,  v6,  v7,  v6,  v7,  v4,  v5,  v4,  v5
    do_butterfly_vec_mix_rev v24, v25, v16, v17, v26, v27, v24, v25, v18, v19, v28, v29,  v0,  v4,  v5,  v4,  v5,  v2,  v3,  v2,  v3
    do_butterfly_vec_mix_rev v24, v25, v18, v19, v28, v29, v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,  v2,  v3,  v2,  v3,  v2,  v3
    do_butterfly_vec_top v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,   v2, v3

    qo_barrett v24, v25, v26, v27, v16, v17, v18, v19,  v1,  #11,  v0

    trn1 v16.4S, v24.4S, v28.4S
    trn2 v20.4S, v24.4S, v28.4S
    trn1 v17.4S, v25.4S, v29.4S
    trn2 v21.4S, v25.4S, v29.4S
    trn1 v18.4S, v26.4S, v30.4S
    trn2 v22.4S, v26.4S, v30.4S
    trn1 v19.4S, v27.4S, v31.4S
    trn2 v23.4S, v27.4S, v31.4S

    st4 {v16.4S, v17.4S, v18.4S, v19.4S}, [src0], #64
    st4 {v20.4S, v21.4S, v22.4S, v23.4S}, [src1], #64

    sub counter, counter, #1
    cbnz counter, _intt_bot_loop

    .unreq    Q
    .unreq    BarrettM
    .unreq    src0
    .unreq    src1
    .unreq    table
    .unreq    counter
    pop_all

    br lr

.align 2
.global __asm_intt_SIMD_top
.global ___asm_intt_SIMD_top
__asm_intt_SIMD_top:
___asm_intt_SIMD_top:

    push_all
    Q         .req w20
    BarrettM  .req w21
    invN      .req w22
    invN_f    .req w23
    src0      .req x0
    src1      .req x1
    src2      .req x2
    src3      .req x3
    src4      .req x4
    src5      .req x5
    src6      .req x6
    src7      .req x7
    src8      .req x8
    src9      .req x9
    src10     .req x10
    src11     .req x11
    src12     .req x12
    src13     .req x13
    src14     .req x14
    src15     .req x15
    table     .req x28
    counter   .req x19

    ldrsh Q, [x2, #0]
    ldrsh BarrettM, [x2, #8]
    ldr   invN, [x2, #10]
    ldr   invN_f, [x2, #14]

    mov table, x1

    add  src0, x0,  #32*0
    add  src1, x0,  #32*1
    add  src2, x0,  #32*2
    add  src3, x0,  #32*3
    add  src4, x0,  #32*4
    add  src5, x0,  #32*5
    add  src6, x0,  #32*6
    add  src7, x0,  #32*7
    add  src8, x0,  #32*8
    add  src9, x0,  #32*9
    add src10, x0, #32*10
    add src11, x0, #32*11
    add src12, x0, #32*12
    add src13, x0, #32*13
    add src14, x0, #32*14
    add src15, x0, #32*15

    ld1 { v0.8H,  v1.8H,  v2.8H,  v3.8H}, [table], #64

    mov  v0.H[0], Q

    dup v24.8H, Q
    dup v25.8H, BarrettM

    ld1 { v4.8H}, [ src0]
    ld1 { v5.8H}, [ src1]
    ld1 { v6.8H}, [ src2]
    ld1 { v7.8H}, [ src3]
    ld1 { v8.8H}, [ src4]
    ld1 { v9.8H}, [ src5]
    ld1 {v10.8H}, [ src6]
    ld1 {v11.8H}, [ src7]

    ld1 {v12.8H}, [ src8]
    ld1 {v13.8H}, [ src9]
    ld1 {v14.8H}, [src10]
    ld1 {v15.8H}, [src11]
    ld1 {v16.8H}, [src12]
    ld1 {v17.8H}, [src13]
    ld1 {v18.8H}, [src14]
    ld1 {v19.8H}, [src15]

    qo_butterfly_bot v12, v14, v16, v18, v28, v29, v30, v31, v13, v15, v17, v19,  v0,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7
    qo_butterfly_mix_rev v12, v14, v16, v18, v28, v29, v30, v31, v13, v15, v17, v19,  v4,  v6,  v8, v10, v20, v21, v22, v23,  v5,  v7,  v9, v11,  v0,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7
    qo_butterfly_mix_rev  v4,  v6,  v8, v10, v20, v21, v22, v23,  v5,  v7,  v9, v11, v12, v13, v16, v17, v28, v29, v30, v31, v14, v15, v18, v19,  v0,  v2, 0, 1,  v2, 2, 3,  v2, 4, 5,  v2, 6, 7,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7
    qo_butterfly_mix_rev v12, v13, v16, v17, v28, v29, v30, v31, v14, v15, v18, v19,  v4,  v5,  v8,  v9, v20, v21, v22, v23,  v6,  v7, v10, v11,  v0,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3
    qo_butterfly_mix_rev  v4,  v5,  v8,  v9, v20, v21, v22, v23,  v6,  v7, v10, v11, v12, v13, v14, v15, v28, v29, v30, v31, v16, v17, v18, v19,  v0,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7
    qo_butterfly_mix_rev v12, v13, v14, v15, v28, v29, v30, v31, v16, v17, v18, v19,  v4,  v5,  v6,  v7, v20, v21, v22, v23,  v8,  v9, v10, v11,  v0,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5
    qo_butterfly_top  v4,  v5,  v6,  v7, v20, v21, v22, v23,  v8,  v9, v10, v11,  v0,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5

    qo_barrett_vec  v4,  v5,  v12,  v13, v20, v21, v22, v23, v25, #11, v24

    mov v0.S[1], invN_f

    qo_butterfly_bot  v4,  v5,  v6,  v7, v28, v29, v30, v31, v12, v13, v14, v15,  v0,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3
    qo_butterfly_mix_rev  v4,  v5,  v6,  v7, v28, v29, v30, v31, v12, v13, v14, v15,  v8,  v9, v10, v11, v20, v21, v22, v23, v16, v17, v18, v19,  v0,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3
    qo_butterfly_top  v8,  v9, v10, v11, v20, v21, v22, v23, v16, v17, v18, v19,  v0,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3

    mov v0.S[1], invN

    sqrdmulh v28.8H,  v4.8H,  v0.H[2]
    sqrdmulh v29.8H,  v5.8H,  v0.H[2]
    sqrdmulh v30.8H,  v6.8H,  v0.H[2]
    sqrdmulh v31.8H,  v7.8H,  v0.H[2]
    sqrdmulh v20.8H,  v8.8H,  v0.H[2]
    sqrdmulh v21.8H,  v9.8H,  v0.H[2]
    sqrdmulh v22.8H, v10.8H,  v0.H[2]
    sqrdmulh v23.8H, v11.8H,  v0.H[2]

    mul       v4.8H,  v4.8H,  v0.H[3]
    mul       v5.8H,  v5.8H,  v0.H[3]
    mul       v6.8H,  v6.8H,  v0.H[3]
    mul       v7.8H,  v7.8H,  v0.H[3]
    mul       v8.8H,  v8.8H,  v0.H[3]
    mul       v9.8H,  v9.8H,  v0.H[3]
    mul      v10.8H, v10.8H,  v0.H[3]
    mul      v11.8H, v11.8H,  v0.H[3]

    mls       v4.8H, v28.8H,  v0.H[0]
    mls       v5.8H, v29.8H,  v0.H[0]
    mls       v6.8H, v30.8H,  v0.H[0]
    mls       v7.8H, v31.8H,  v0.H[0]
    mls       v8.8H, v20.8H,  v0.H[0]
    mls       v9.8H, v21.8H,  v0.H[0]
    mls      v10.8H, v22.8H,  v0.H[0]
    mls      v11.8H, v23.8H,  v0.H[0]

    st1 { v4.8H}, [ src0], #16
    ld1 { v4.8H}, [ src0]
    st1 { v5.8H}, [ src1], #16
    ld1 { v5.8H}, [ src1]
    st1 { v6.8H}, [ src2], #16
    ld1 { v6.8H}, [ src2]
    st1 { v7.8H}, [ src3], #16
    ld1 { v7.8H}, [ src3]
    st1 { v8.8H}, [ src4], #16
    ld1 { v8.8H}, [ src4]
    st1 { v9.8H}, [ src5], #16
    ld1 { v9.8H}, [ src5]
    st1 {v10.8H}, [ src6], #16
    ld1 {v10.8H}, [ src6]
    st1 {v11.8H}, [ src7], #16
    ld1 {v11.8H}, [ src7]

    st1 {v12.8H}, [ src8], #16
    ld1 {v12.8H}, [ src8]
    st1 {v13.8H}, [ src9], #16
    ld1 {v13.8H}, [ src9]
    st1 {v14.8H}, [src10], #16
    ld1 {v14.8H}, [src10]
    st1 {v15.8H}, [src11], #16
    ld1 {v15.8H}, [src11]
    st1 {v16.8H}, [src12], #16
    ld1 {v16.8H}, [src12]
    st1 {v17.8H}, [src13], #16
    ld1 {v17.8H}, [src13]
    st1 {v18.8H}, [src14], #16
    ld1 {v18.8H}, [src14]
    st1 {v19.8H}, [src15], #16
    ld1 {v19.8H}, [src15]

    qo_butterfly_bot v12, v14, v16, v18, v28, v29, v30, v31, v13, v15, v17, v19,  v0,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7
    qo_butterfly_mix_rev v12, v14, v16, v18, v28, v29, v30, v31, v13, v15, v17, v19,  v4,  v6,  v8, v10, v20, v21, v22, v23,  v5,  v7,  v9, v11,  v0,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7
    qo_butterfly_mix_rev  v4,  v6,  v8, v10, v20, v21, v22, v23,  v5,  v7,  v9, v11, v12, v13, v16, v17, v28, v29, v30, v31, v14, v15, v18, v19,  v0,  v2, 0, 1,  v2, 2, 3,  v2, 4, 5,  v2, 6, 7,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7
    qo_butterfly_mix_rev v12, v13, v16, v17, v28, v29, v30, v31, v14, v15, v18, v19,  v4,  v5,  v8,  v9, v20, v21, v22, v23,  v6,  v7, v10, v11,  v0,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3
    qo_butterfly_mix_rev  v4,  v5,  v8,  v9, v20, v21, v22, v23,  v6,  v7, v10, v11, v12, v13, v14, v15, v28, v29, v30, v31, v16, v17, v18, v19,  v0,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7
    qo_butterfly_mix_rev v12, v13, v14, v15, v28, v29, v30, v31, v16, v17, v18, v19,  v4,  v5,  v6,  v7, v20, v21, v22, v23,  v8,  v9, v10, v11,  v0,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5
    qo_butterfly_top  v4,  v5,  v6,  v7, v20, v21, v22, v23,  v8,  v9, v10, v11,  v0,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5

    qo_barrett_vec  v4,  v5,  v12,  v13, v20, v21, v22, v23, v25, #11, v24

    mov v0.S[1], invN_f

    qo_butterfly_bot  v4,  v5,  v6,  v7, v28, v29, v30, v31, v12, v13, v14, v15,  v0,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3
    qo_butterfly_mix_rev  v4,  v5,  v6,  v7, v28, v29, v30, v31, v12, v13, v14, v15,  v8,  v9, v10, v11, v20, v21, v22, v23, v16, v17, v18, v19,  v0,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3
    qo_butterfly_top  v8,  v9, v10, v11, v20, v21, v22, v23, v16, v17, v18, v19,  v0,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3

    mov v0.S[1], invN

    sqrdmulh v28.8H,  v4.8H,  v0.H[2]
    sqrdmulh v29.8H,  v5.8H,  v0.H[2]
    sqrdmulh v30.8H,  v6.8H,  v0.H[2]
    sqrdmulh v31.8H,  v7.8H,  v0.H[2]
    sqrdmulh v20.8H,  v8.8H,  v0.H[2]
    sqrdmulh v21.8H,  v9.8H,  v0.H[2]
    sqrdmulh v22.8H, v10.8H,  v0.H[2]
    sqrdmulh v23.8H, v11.8H,  v0.H[2]

    mul       v4.8H,  v4.8H,  v0.H[3]
    mul       v5.8H,  v5.8H,  v0.H[3]
    mul       v6.8H,  v6.8H,  v0.H[3]
    mul       v7.8H,  v7.8H,  v0.H[3]
    mul       v8.8H,  v8.8H,  v0.H[3]
    mul       v9.8H,  v9.8H,  v0.H[3]
    mul      v10.8H, v10.8H,  v0.H[3]
    mul      v11.8H, v11.8H,  v0.H[3]

    mls       v4.8H, v28.8H,  v0.H[0]
    mls       v5.8H, v29.8H,  v0.H[0]
    mls       v6.8H, v30.8H,  v0.H[0]
    mls       v7.8H, v31.8H,  v0.H[0]
    mls       v8.8H, v20.8H,  v0.H[0]
    mls       v9.8H, v21.8H,  v0.H[0]
    mls      v10.8H, v22.8H,  v0.H[0]
    mls      v11.8H, v23.8H,  v0.H[0]

    st1 { v4.8H}, [ src0], #16
    st1 { v5.8H}, [ src1], #16
    st1 { v6.8H}, [ src2], #16
    st1 { v7.8H}, [ src3], #16
    st1 { v8.8H}, [ src4], #16
    st1 { v9.8H}, [ src5], #16
    st1 {v10.8H}, [ src6], #16
    st1 {v11.8H}, [ src7], #16

    st1 {v12.8H}, [ src8], #16
    st1 {v13.8H}, [ src9], #16
    st1 {v14.8H}, [src10], #16
    st1 {v15.8H}, [src11], #16
    st1 {v16.8H}, [src12], #16
    st1 {v17.8H}, [src13], #16
    st1 {v18.8H}, [src14], #16
    st1 {v19.8H}, [src15], #16

    .unreq    Q
    .unreq    BarrettM
    .unreq    invN
    .unreq    invN_f
    .unreq    src0
    .unreq    src1
    .unreq    src2
    .unreq    src3
    .unreq    src4
    .unreq    src5
    .unreq    src6
    .unreq    src7
    .unreq    src8
    .unreq    src9
    .unreq    src10
    .unreq    src11
    .unreq    src12
    .unreq    src13
    .unreq    src14
    .unreq    src15
    .unreq    table
    .unreq    counter
    pop_all

    br lr

.align 2
.global __asm_intt_SIMD_bot_schedule
.global ___asm_intt_SIMD_bot_schedule
__asm_intt_SIMD_bot_schedule:
___asm_intt_SIMD_bot_schedule:

    push_all
    Q         .req w20
    BarrettM  .req w21
    src0      .req x0
    src1      .req x1
    table     .req x28
    counter   .req x19

    ldrsh Q, [x2, #0]
    ldrsh BarrettM, [x2, #8]

    add table, x1, #64

    add src0, x0, #256*0
    add src1, x0, #256*1

    mov v0.H[0], Q
    mov v0.H[1], BarrettM

    ldr q28, [src0, #1*16]
    ldr q29, [src1, #1*16]
    ldr q30, [src0, #3*16]
    ldr q31, [src1, #3*16]

    trn_4x4_2l4 v28, v29, v30, v31, v20, v21, v22, v23, src0, src1, q24, q25, q26, q27, #0*16, #0*16, #2*16, #2*16

    trn_4x4_2l4 v24, v25, v26, v27, v20, v21, v22, v23, table, table, q12, q13, q14, q15, #12*16, #13*16, #14*16, #15*16

    do_butterfly_vec_bot v28, v30, v18, v19, v29, v31,  v0, v12, v13, v14, v15

    do_butterfly_vec_mix_rev_l4 \
            v18, v19, v29, v31, \
            v24, v26, v16, v17, v25, v27,  v0, v12, v13, v14, v15, \
            table, \
             q8,  q9, q10, q11, \
             #8*16, #9*16, #10*16, #11*16

    do_butterfly_vec_mix_rev_l4 \
            v16, v17, v25, v27, \
            v28, v29, v18, v19, v30, v31,  v0,  v8,  v9, v10, v11, \
            table, \
             q4,  q5,  q6,  q7, \
             #4*16, #5*16, #6*16, #7*16

    do_butterfly_vec_mix_rev_l3 \
            v18, v19, v30, v31, \
            v24, v25, v16, v17, v26, v27,  v0,  v6,  v7,  v6,  v7, \
            table, \
             q1,  q2,  q3, \
             #1*16, #2*16, #3*16

    do_butterfly_vec_mix_rev v24, v25, v16, v17, v26, v27, v24, v25, v18, v19, v28, v29,  v0,  v4,  v5,  v4,  v5,  v2,  v3,  v2,  v3
    do_butterfly_vec_mix_rev v24, v25, v18, v19, v28, v29, v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,  v2,  v3,  v2,  v3,  v2,  v3
    do_butterfly_vec_top v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,   v2, v3

    oo_barrett v24, v25, v26, v27, v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23, v0, #11, v0

    add table, table, #256

    trn_4x4 v28, v29, v30, v31, v16, v17, v18, v19

    trn_4x4_2s4 v24, v25, v26, v27, v16, v17, v18, v19, src0, src1, q28, q29, q30, q31, #1*16, #1*16, #3*16, #3*16

    mov counter, #3
    _intt_bot_schedule_loop:

    str q24, [src0, #0*16]
    ldr q28, [src0, #(64+1*16)]
    str q25, [src1, #0*16]
    ldr q29, [src1, #(64+1*16)]
    str q26, [src0, #2*16]
    ldr q30, [src0, #(64+3*16)]
    str q27, [src1, #2*16]
    ldr q31, [src1, #(64+3*16)]

    add src0, src0, #64
    add src1, src1, #64

    trn_4x4_2l4 v28, v29, v30, v31, v20, v21, v22, v23, src0, src1, q24, q25, q26, q27, #0*16, #0*16, #2*16, #2*16

    trn_4x4_2l4 v24, v25, v26, v27, v20, v21, v22, v23, table, table, q12, q13, q14, q15, #12*16, #13*16, #14*16, #15*16

    do_butterfly_vec_bot v28, v30, v18, v19, v29, v31,  v0, v12, v13, v14, v15

    do_butterfly_vec_mix_rev_l4 \
            v18, v19, v29, v31, \
            v24, v26, v16, v17, v25, v27,  v0, v12, v13, v14, v15, \
            table, \
             q8,  q9, q10, q11, \
             #8*16, #9*16, #10*16, #11*16

    do_butterfly_vec_mix_rev_l4 \
            v16, v17, v25, v27, \
            v28, v29, v18, v19, v30, v31,  v0,  v8,  v9, v10, v11, \
            table, \
             q4,  q5,  q6,  q7, \
             #4*16, #5*16, #6*16, #7*16

    do_butterfly_vec_mix_rev_l3 \
            v18, v19, v30, v31, \
            v24, v25, v16, v17, v26, v27,  v0,  v6,  v7,  v6,  v7, \
            table, \
             q1,  q2,  q3, \
             #1*16, #2*16, #3*16

    do_butterfly_vec_mix_rev v24, v25, v16, v17, v26, v27, v24, v25, v18, v19, v28, v29,  v0,  v4,  v5,  v4,  v5,  v2,  v3,  v2,  v3
    do_butterfly_vec_mix_rev v24, v25, v18, v19, v28, v29, v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,  v2,  v3,  v2,  v3,  v2,  v3
    do_butterfly_vec_top v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,   v2, v3

    oo_barrett v24, v25, v26, v27, v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23, v0, #11, v0

    add table, table, #256

    trn_4x4 v28, v29, v30, v31, v16, v17, v18, v19

    trn_4x4_2s4 v24, v25, v26, v27, v16, v17, v18, v19, src0, src1, q28, q29, q30, q31, #1*16, #1*16, #3*16, #3*16

    sub counter, counter, #1
    cbnz counter, _intt_bot_schedule_loop

    str q24, [src0, #0*16]
    str q25, [src1, #0*16]
    str q26, [src0, #2*16]
    str q27, [src1, #2*16]


    .unreq    Q
    .unreq    BarrettM
    .unreq    src0
    .unreq    src1
    .unreq    table
    .unreq    counter
    pop_all

    br lr

.align 2
.global __asm_intt_SIMD_top_schedule
.global ___asm_intt_SIMD_top_schedule
__asm_intt_SIMD_top_schedule:
___asm_intt_SIMD_top_schedule:

    push_all
    Q         .req w20
    BarrettM  .req w21
    invN      .req w22
    invN_f    .req w23
    src       .req x0
    table     .req x1
    counter   .req x19

    ldrsh        Q, [x2, #0]
    ldrsh BarrettM, [x2, #8]
    ldr       invN, [x2, #10]
    ldr     invN_f, [x2, #14]

    mov v4.S[0], invN
    mov v4.S[1], invN_f

    ldr q0, [table, #0*16]
    mov v0.H[0], Q

    ldr q1, [table, #1*16]
    ldr q2, [table, #2*16]
    ldr q3, [table, #3*16]

    ldr q16, [src, # 8*32]
    ldr q17, [src, # 9*32]
    ldr q18, [src, #10*32]
    ldr q19, [src, #11*32]
    ldr q20, [src, #12*32]
    ldr q21, [src, #13*32]
    ldr q22, [src, #14*32]
    ldr q23, [src, #15*32]

    qo_butterfly_botll \
            v16, v18, v20, v22, v28, v29, v30, v31, v17, v19, v21, v23, \
            src, \
             q8,  q9, q10, q11, \
             #0*32, #1*32, #2*32, #3*32, \
            src, \
            q12, q13, q14, q15, \
             #4*32, #5*32, #6*32, #7*32


    qo_butterfly_mix_rev v16, v18, v20, v22, v28, v29, v30, v31, v17, v19, v21, v23,  v8, v10, v12, v14, v24, v25, v26, v27,  v9, v11, v13, v15,  v0,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7
    qo_butterfly_mix_rev  v8, v10, v12, v14, v24, v25, v26, v27,  v9, v11, v13, v15, v16, v17, v20, v21, v28, v29, v30, v31, v18, v19, v22, v23,  v0,  v2, 0, 1,  v2, 2, 3,  v2, 4, 5,  v2, 6, 7,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7
    qo_butterfly_mix_rev v16, v17, v20, v21, v28, v29, v30, v31, v18, v19, v22, v23,  v8,  v9, v12, v13, v24, v25, v26, v27, v10, v11, v14, v15,  v0,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3
    qo_butterfly_mix_rev  v8,  v9, v12, v13, v24, v25, v26, v27, v10, v11, v14, v15, v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23,  v0,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7
    qo_butterfly_mix_rev v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23,  v8,  v9, v10, v11, v24, v25, v26, v27, v12, v13, v14, v15,  v0,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5
    qo_butterfly_mix_rev  v8,  v9, v10, v11, v24, v25, v26, v27, v12, v13, v14, v15,  v8,  v9, v10, v11, v28, v29, v30, v31, v16, v17, v18, v19,  v0,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3
    qo_butterfly_mix_rev  v8,  v9, v10, v11, v28, v29, v30, v31, v16, v17, v18, v19, v12, v13, v14, v15, v24, v25, v26, v27, v20, v21, v22, v23,  v0,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3

    qo_butterfly_topsl \
            v24, v25, v26, v27, v20, v21, v22, v23,  v0,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3, \
            src, \
            q16, q17, q18, q19, \
            #8*32, #9*32, #10*32, #11*32, \
            src, \
            q16, q17, q18, q19, \
            #(16+8*32), #(16+9*32), #(16+10*32), #(16+11*32)

    qo_montgomery_mul_insl \
             v8,  v9, v10, v11, v28, v29, v30, v31,  v0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0, \
             src, \
             q20, q21, q22, q23, \
             #12*32, #13*32, #14*32, #15*32, \
             src, \
             q20, q21, q22, q23, \
             #(16+12*32), #(16+13*32), #(16+14*32), #(16+15*32)

    qo_butterfly_botsl_mul \
            v16, v18, v20, v22, v28, v29, v30, v31, v17, v19, v21, v23, \
            src, \
             q8,  q9, q10, q11, \
             #0*32, #1*32, #2*32, #3*32, \
            src, \
             q8,  q9, q10, q11, \
             #(16+0*32), #(16+1*32), #(16+2*32), #(16+3*32), \
             v12, v13, v14, v15, v24, v25, v26, v27, \
             v0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0

    str q12, [src, # 4*32]
    ldr q12, [src, #(16+ 4*32)]
    str q13, [src, # 5*32]
    ldr q13, [src, #(16+ 5*32)]
    str q14, [src, # 6*32]
    ldr q14, [src, #(16+ 6*32)]
    str q15, [src, # 7*32]
    ldr q15, [src, #(16+ 7*32)]

    qo_butterfly_mix_rev v16, v18, v20, v22, v28, v29, v30, v31, v17, v19, v21, v23,  v8, v10, v12, v14, v24, v25, v26, v27,  v9, v11, v13, v15,  v0,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7
    qo_butterfly_mix_rev  v8, v10, v12, v14, v24, v25, v26, v27,  v9, v11, v13, v15, v16, v17, v20, v21, v28, v29, v30, v31, v18, v19, v22, v23,  v0,  v2, 0, 1,  v2, 2, 3,  v2, 4, 5,  v2, 6, 7,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7
    qo_butterfly_mix_rev v16, v17, v20, v21, v28, v29, v30, v31, v18, v19, v22, v23,  v8,  v9, v12, v13, v24, v25, v26, v27, v10, v11, v14, v15,  v0,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3
    qo_butterfly_mix_rev  v8,  v9, v12, v13, v24, v25, v26, v27, v10, v11, v14, v15, v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23,  v0,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7
    qo_butterfly_mix_rev v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23,  v8,  v9, v10, v11, v24, v25, v26, v27, v12, v13, v14, v15,  v0,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5
    qo_butterfly_mix_rev  v8,  v9, v10, v11, v24, v25, v26, v27, v12, v13, v14, v15,  v8,  v9, v10, v11, v28, v29, v30, v31, v16, v17, v18, v19,  v0,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3
    qo_butterfly_mix_rev  v8,  v9, v10, v11, v28, v29, v30, v31, v16, v17, v18, v19, v12, v13, v14, v15, v24, v25, v26, v27, v20, v21, v22, v23,  v0,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3

    qo_butterfly_tops \
            v24, v25, v26, v27, v20, v21, v22, v23,  v0,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3, \
            src, \
            q16, q17, q18, q19, \
            #(16+8*32), #(16+9*32), #(16+10*32), #(16+11*32)


    qo_montgomery_mul_ins \
             v8,  v9, v10, v11, v28, v29, v30, v31,  v0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0, \
             src, \
             q20, q21, q22, q23, \
             #(16+12*32), #(16+13*32), #(16+14*32), #(16+15*32)

    qo_montgomery_mul_ins \
            v12, v13, v14, v15, v24, v25, v26, v27,  v0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0, \
             src, \
              q8,  q9, q10, q11, \
             #(16+0*32), #(16+1*32), #(16+2*32), #(16+3*32)

    str q12, [src, #(16+ 4*32)]
    str q13, [src, #(16+ 5*32)]
    str q14, [src, #(16+ 6*32)]
    str q15, [src, #(16+ 7*32)]

    .unreq    Q
    .unreq    BarrettM
    .unreq    invN
    .unreq    invN_f
    .unreq    src
    .unreq    counter
    pop_all

    br lr





















