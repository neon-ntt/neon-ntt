#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

/*
 * We offer
 * CC0 1.0 Universal or the following MIT License for this file.
 * You may freely choose one of them that applies.
 *
 * MIT License
 *
 * Copyright (c) 2024: Vincent Hwang
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

#define ARRAY_N 256

#define NTT_N 128
#define LOGNTT_N 7

#define Q (3329)
#define omegaQ (17)
// omegaQ^(-1) mod^+- Q
#define invomegaQ (1175)

// R = 2^16 below
// 2^16 mod^+- Q
#define RmodQ (-1044)
// round( RmodQ * R / (2 * Q) )
#define RmodQ_hi (-10276)
// -Q^(-1) mod^+- 2^16
#define Qprime (3327)

// NTT^(-1) * 2^16 mod^+- Q
#define FINAL_SCALE 512
// round( FINAL_SCALE * R / (2 * Q) )
#define FINAL_SCALE_hi 5040
// invomegaQ^64 * FINAL_SCALE mod^+- Q
#define FINAL_SCALE_F 266
// round( FINAL_SCALE * R / (2 * Q) )
#define FINAL_SCALE_F_hi 2618

// Barrett26 = round (2^26 / Q1)
#define Barrett26 20159

#endif





