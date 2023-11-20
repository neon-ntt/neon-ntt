
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

#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define ARRAY_N 256

#define NTT_N 64
#define LOGNTT_N 6

// Q1
#define Q1 25570817
// omegaQ1 = 3^( (Q1 - 1) / (NTT_N << 1) ) mod Q1
#define omegaQ1 21614269
// invomegaQ1 = omegaQ^{-1} mod Q1
#define invomegaQ1 8215090
// R = 2^32 below
// RmodQ1 = 2^32 mod^{+-} Q1
#define RmodQ1 -929960
// Q1prime = Q1^{-1} mod^{+-} 2^32
#define Q1prime -155332095
// invNQ1 = NTT_N^{-1} mod Q1
#define invNQ1 25171273
// R2modQ1 = 2^32 mod^{+-} Q1
#define R2modQ1 -929960
// Q1prime2 = -Q1^{-1} mod^{+-} 2^32
#define Q1prime2 155332095

#endif





