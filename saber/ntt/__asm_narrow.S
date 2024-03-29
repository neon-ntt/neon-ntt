
/*
 * We offer
 * CC0 1.0 Universal or the following MIT License for this file.
 * You may freely choose one of them that applies.
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

#include "SABER_params.h"

.align 2
.global __asm_round
.global ___asm_round
__asm_round:
___asm_round:


    .equ srv, (SABER_EQ-SABER_EP)

    ld2 { v0.8H,  v1.8H}, [x1], #32
    ld2 { v2.8H,  v3.8H}, [x1], #32
    ld2 { v4.8H,  v5.8H}, [x1], #32
    ld2 { v6.8H,  v7.8H}, [x1], #32

    srshr  v0.8H,   v0.8H, #srv
    srshr  v2.8H,   v2.8H, #srv
    srshr  v4.8H,   v4.8H, #srv
    srshr  v6.8H,   v6.8H, #srv

    mov x7, #7
    _round_loop:

    st1 { v0.8H}, [x0], #16
    ld2 { v0.8H,  v1.8H}, [x1], #32
    st1 { v2.8H}, [x0], #16
    ld2 { v2.8H,  v3.8H}, [x1], #32
    st1 { v4.8H}, [x0], #16
    ld2 { v4.8H,  v5.8H}, [x1], #32
    st1 { v6.8H}, [x0], #16
    ld2 { v6.8H,  v7.8H}, [x1], #32

    srshr  v0.8H,   v0.8H, #srv
    srshr  v2.8H,   v2.8H, #srv
    srshr  v4.8H,   v4.8H, #srv
    srshr  v6.8H,   v6.8H, #srv

    sub x7, x7, #1
    cbnz x7, _round_loop

    st1 { v0.8H}, [x0], #16
    st1 { v2.8H}, [x0], #16
    st1 { v4.8H}, [x0], #16
    st1 { v6.8H}, [x0], #16

    br lr

.align 2
.global __asm_enc_add_msg
.global ___asm_enc_add_msg
__asm_enc_add_msg:
___asm_enc_add_msg:

    .equ srv, (SABER_EP-SABER_ET)
    .equ slv, (SABER_EP-1)

    dup v30.8H, w3

    ld2 { v0.8H,  v1.8H}, [x1], #32
    ld2 { v2.8H,  v3.8H}, [x1], #32
    ld2 { v4.8H,  v5.8H}, [x1], #32
    ld2 { v6.8H,  v7.8H}, [x1], #32
    ld1 { v1.8H}, [x2], #16
    ld1 { v3.8H}, [x2], #16
    ld1 { v5.8H}, [x2], #16
    ld1 { v7.8H}, [x2], #16

    add   v0.8H,   v0.8H,  v30.8H
    add   v2.8H,   v2.8H,  v30.8H
    add   v4.8H,   v4.8H,  v30.8H
    add   v6.8H,   v6.8H,  v30.8H

    shl   v1.8H,   v1.8H, #slv
    shl   v3.8H,   v3.8H, #slv
    shl   v5.8H,   v5.8H, #slv
    shl   v7.8H,   v7.8H, #slv

    sub   v0.8H,   v0.8H,   v1.8H
    sub   v2.8H,   v2.8H,   v3.8H
    sub   v4.8H,   v4.8H,   v5.8H
    sub   v6.8H,   v6.8H,   v7.8H

    sshr  v0.8H,   v0.8H, #srv
    sshr  v2.8H,   v2.8H, #srv
    sshr  v4.8H,   v4.8H, #srv
    sshr  v6.8H,   v6.8H, #srv

    mov x7, #7
    _enc_add_msg_loop:

    st1 { v0.8H}, [x0], #16
    ld2 { v0.8H,  v1.8H}, [x1], #32
    st1 { v2.8H}, [x0], #16
    ld2 { v2.8H,  v3.8H}, [x1], #32
    st1 { v4.8H}, [x0], #16
    ld2 { v4.8H,  v5.8H}, [x1], #32
    st1 { v6.8H}, [x0], #16
    ld2 { v6.8H,  v7.8H}, [x1], #32
    ld1 { v1.8H}, [x2], #16
    ld1 { v3.8H}, [x2], #16
    ld1 { v5.8H}, [x2], #16
    ld1 { v7.8H}, [x2], #16

    add   v0.8H,   v0.8H,  v30.8H
    add   v2.8H,   v2.8H,  v30.8H
    add   v4.8H,   v4.8H,  v30.8H
    add   v6.8H,   v6.8H,  v30.8H

    shl   v1.8H,   v1.8H, #slv
    shl   v3.8H,   v3.8H, #slv
    shl   v5.8H,   v5.8H, #slv
    shl   v7.8H,   v7.8H, #slv

    sub   v0.8H,   v0.8H,   v1.8H
    sub   v2.8H,   v2.8H,   v3.8H
    sub   v4.8H,   v4.8H,   v5.8H
    sub   v6.8H,   v6.8H,   v7.8H

    sshr  v0.8H,   v0.8H, #srv
    sshr  v2.8H,   v2.8H, #srv
    sshr  v4.8H,   v4.8H, #srv
    sshr  v6.8H,   v6.8H, #srv

    sub x7, x7, #1
    cbnz x7, _enc_add_msg_loop

    st1 { v0.8H}, [x0], #16
    st1 { v2.8H}, [x0], #16
    st1 { v4.8H}, [x0], #16
    st1 { v6.8H}, [x0], #16

    br lr


.align 2
.global __asm_dec_get_msg
.global ___asm_dec_get_msg
__asm_dec_get_msg:
___asm_dec_get_msg:

    .equ srv, (SABER_EP-1)
    .equ slv, (SABER_EP-SABER_ET)

    dup v30.8H, w3

    ld2 { v0.8H,  v1.8H}, [x1], #32
    ld2 { v2.8H,  v3.8H}, [x1], #32
    ld2 { v4.8H,  v5.8H}, [x1], #32
    ld2 { v6.8H,  v7.8H}, [x1], #32
    ld1 { v1.8H}, [x2], #16
    ld1 { v3.8H}, [x2], #16
    ld1 { v5.8H}, [x2], #16
    ld1 { v7.8H}, [x2], #16

    add   v0.8H,   v0.8H,  v30.8H
    add   v2.8H,   v2.8H,  v30.8H
    add   v4.8H,   v4.8H,  v30.8H
    add   v6.8H,   v6.8H,  v30.8H

    shl   v1.8H,   v1.8H, #slv
    shl   v3.8H,   v3.8H, #slv
    shl   v5.8H,   v5.8H, #slv
    shl   v7.8H,   v7.8H, #slv

    sub   v0.8H,   v0.8H,   v1.8H
    sub   v2.8H,   v2.8H,   v3.8H
    sub   v4.8H,   v4.8H,   v5.8H
    sub   v6.8H,   v6.8H,   v7.8H

    sshr  v0.8H,   v0.8H, #srv
    sshr  v2.8H,   v2.8H, #srv
    sshr  v4.8H,   v4.8H, #srv
    sshr  v6.8H,   v6.8H, #srv

    mov x7, #7
    _dec_get_msg_loop:

    st1 { v0.8H}, [x0], #16
    ld2 { v0.8H,  v1.8H}, [x1], #32
    st1 { v2.8H}, [x0], #16
    ld2 { v2.8H,  v3.8H}, [x1], #32
    st1 { v4.8H}, [x0], #16
    ld2 { v4.8H,  v5.8H}, [x1], #32
    st1 { v6.8H}, [x0], #16
    ld2 { v6.8H,  v7.8H}, [x1], #32
    ld1 { v1.8H}, [x2], #16
    ld1 { v3.8H}, [x2], #16
    ld1 { v5.8H}, [x2], #16
    ld1 { v7.8H}, [x2], #16

    add   v0.8H,   v0.8H,  v30.8H
    add   v2.8H,   v2.8H,  v30.8H
    add   v4.8H,   v4.8H,  v30.8H
    add   v6.8H,   v6.8H,  v30.8H

    shl   v1.8H,   v1.8H, #slv
    shl   v3.8H,   v3.8H, #slv
    shl   v5.8H,   v5.8H, #slv
    shl   v7.8H,   v7.8H, #slv

    sub   v0.8H,   v0.8H,   v1.8H
    sub   v2.8H,   v2.8H,   v3.8H
    sub   v4.8H,   v4.8H,   v5.8H
    sub   v6.8H,   v6.8H,   v7.8H

    sshr  v0.8H,   v0.8H, #srv
    sshr  v2.8H,   v2.8H, #srv
    sshr  v4.8H,   v4.8H, #srv
    sshr  v6.8H,   v6.8H, #srv

    sub x7, x7, #1
    cbnz x7, _dec_get_msg_loop

    st1 { v0.8H}, [x0], #16
    st1 { v2.8H}, [x0], #16
    st1 { v4.8H}, [x0], #16
    st1 { v6.8H}, [x0], #16

    br lr

















