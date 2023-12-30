
/*
 * We offer
 * CC0 1.0 Universal or the following MIT License for this file.
 * You may freely choose one of them that applies.
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

#include "params.h"
#include "ntt.h"
#include "NTT_params.h"
#include <stdint.h>

const __attribute__ ((aligned (16)))int16_t asymmetric_const[8] = {
    Q1, Q1prime2, RmodQ1, RmodQ1Q1prime, R3modQ1_prime_half, R3modQ1_doubleprime
};

const __attribute__ ((aligned (16)))int16_t constants[16] = {
    Q1, Q1prime2, RmodQ1, RmodQ1Q1prime, roundRdivQ1,
    invNQ1_R3modQ1_prime_half,
    invNQ1_R3modQ1_doubleprime,
    invNQ1_final_R3modQ1_prime_half,
    invNQ1_final_R3modQ1_doubleprime
};

const __attribute__ ((aligned (16)))int16_t streamlined_CT_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4) + NTT_N) << 1] = {
0, 0, -15749, -1600, -7373, -749, -394, -40, -6762, -687, 6201, 630, -14095, -1432, 8347, 848, 10453, 1062, -13879, -1410, 1900, 193, 7845, 797, -5345, -543, -679, -69, 5601, 569, -15582, -1583, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2914, 2914, 14036, 14036, -8682, -8682, -12156, -12156, 296, 296, 1426, 1426, -882, -882, -1235, -1235, 2845, 2845, -9942, -9942, -748, -748, 7943, 7943, 289, 289, -1010, -1010, -76, -76, 807, 807, 3258, 3258, 14125, 14125, -15483, -15483, 4449, 4449, 331, 331, 1435, 1435, -1573, -1573, 452, 452, 167, 167, 15592, 15592, 16113, 16113, 3691, 3691, 17, 17, 1584, 1584, 1637, 1637, 375, 375, -5591, -5591, -10148, -10148, 7117, 7117, -7678, -7678, -568, -568, -1031, -1031, 723, 723, -780, -780, 5739, 5739, -12717, -12717, -10247, -10247, -12196, -12196, 583, 583, -1292, -1292, -1041, -1041, -1239, -1239, -6693, -6693, -1073, -1073, 10828, 10828, 16192, 16192, -680, -680, -109, -109, 1100, 1100, 1645, 1645, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13180, 13180, 5266, 5266, 14529, 14529, -4400, -4400, 1339, 1339, 535, 535, 1476, 1476, -447, -447, 11782, 11782, 14155, 14155, -10355, -10355, 15099, 15099, 1197, 1197, 1438, 1438, -1052, -1052, 1534, 1534, -10089, -10089, -4538, -4538, -12540, -12540, -9125, -9125, -1025, -1025, -461, -461, -1274, -1274, -927, -927, 13869, 13869, 10463, 10463, 7441, 7441, -12107, -12107, 1409, 1409, 1063, 1063, 756, 756, -1230, -1230, -6565, -6565, 3140, 3140, -11546, -11546, 5522, 5522, -667, -667, 319, 319, -1173, -1173, 561, 561, -472, -472, -5473, -5473, -3091, -3091, -8495, -8495, -48, -48, -556, -556, -314, -314, -863, -863, 2293, 2293, 7451, 7451, -2746, -2746, -7235, -7235, 233, 233, 757, 757, -279, -279, -735, -735, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2786, -2786, -9213, -9213, 551, 551, -4429, -4429, -283, -283, -936, -936, 56, 56, -450, -450, 6398, 6398, -6713, -6713, -8032, -8032, 14578, 14578, 650, 650, -682, -682, -816, -816, 1481, 1481, -13308, -13308, -7008, -7008, 6221, 6221, 6378, 6378, -1352, -1352, -712, -712, 632, 632, 648, 648, -16005, -16005, -5168, -5168, -14588, -14588, 11251, 11251, -1626, -1626, -525, -525, -1482, -1482, 1143, 1143, 16251, 16251, 10749, 10749, 9371, 9371, -11605, -11605, 1651, 1651, 1092, 1092, 952, 952, -1179, -1179, -5315, -5315, 3967, 3967, 14381, 14381, -5453, -5453, -540, -540, 403, 403, 1461, 1461, -554, -554, -15159, -15159, 10099, 10099, -6319, -6319, 8721, 8721, -1540, -1540, 1026, 1026, -642, -642, 886, 886, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10719, -10719, -13338, -13338, 13121, 13121, 8081, 8081, -1089, -1089, -1355, -1355, 1333, 1333, 821, 821, -4567, -4567, -8416, -8416, 12993, 12993, 12078, 12078, -464, -464, -855, -855, 1320, 1320, 1227, 1227, 325, 325, -2156, -2156, -13918, -13918, 8957, 8957, 33, 33, -219, -219, -1414, -1414, 910, 910, 9243, 9243, -15818, -15818, 7215, 7215, -11999, -11999, 939, 939, -1607, -1607, 733, 733, -1219, -1219, -10050, -10050, 11930, 11930, -9764, -9764, -3878, -3878, -1021, -1021, 1212, 1212, -992, -992, -394, -394, -8780, -8780, -14322, -14322, 2638, 2638, 8711, 8711, -892, -892, -1455, -1455, 268, 268, 885, 885, -9262, -9262, 10129, 10129, 6309, 6309, -11566, -11566, -941, -941, 1029, 1029, 641, 641, -1175, -1175
};

const __attribute__ ((aligned (16)))int16_t pre_asymmetric_table_Q1_extended[ARRAY_N] = {
167, -167, -5591, 5591, 5739, -5739, -6693, 6693, 17, -17, -568, 568, 583, -583, -680, 680, 16113, -16113, 7117, -7117, -10247, 10247, 10828, -10828, 1637, -1637, 723, -723, -1041, 1041, 1100, -1100, 13869, -13869, -6565, 6565, -472, 472, 2293, -2293, 1409, -1409, -667, 667, -48, 48, 233, -233, 7441, -7441, -11546, 11546, -3091, 3091, -2746, 2746, 756, -756, -1173, 1173, -314, 314, -279, 279, -16005, 16005, 16251, -16251, -5315, 5315, -15159, 15159, -1626, 1626, 1651, -1651, -540, 540, -1540, 1540, -14588, 14588, 9371, -9371, 14381, -14381, -6319, 6319, -1482, 1482, 952, -952, 1461, -1461, -642, 642, 9243, -9243, -10050, 10050, -8780, 8780, -9262, 9262, 939, -939, -1021, 1021, -892, 892, -941, 941, 7215, -7215, -9764, 9764, 2638, -2638, 6309, -6309, 733, -733, -992, 992, 268, -268, 641, -641, 15592, -15592, -10148, 10148, -12717, 12717, -1073, 1073, 1584, -1584, -1031, 1031, -1292, 1292, -109, 109, 3691, -3691, -7678, 7678, -12196, 12196, 16192, -16192, 375, -375, -780, 780, -1239, 1239, 1645, -1645, 10463, -10463, 3140, -3140, -5473, 5473, 7451, -7451, 1063, -1063, 319, -319, -556, 556, 757, -757, -12107, 12107, 5522, -5522, -8495, 8495, -7235, 7235, -1230, 1230, 561, -561, -863, 863, -735, 735, -5168, 5168, 10749, -10749, 3967, -3967, 10099, -10099, -525, 525, 1092, -1092, 403, -403, 1026, -1026, 11251, -11251, -11605, 11605, -5453, 5453, 8721, -8721, 1143, -1143, -1179, 1179, -554, 554, 886, -886, -15818, 15818, 11930, -11930, -14322, 14322, 10129, -10129, -1607, 1607, 1212, -1212, -1455, 1455, 1029, -1029, -11999, 11999, -3878, 3878, 8711, -8711, -11566, 11566, -1219, 1219, -394, 394, 885, -885, -1175, 1175
};

const __attribute__ ((aligned (16)))int16_t streamlined_inv_GS_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4) + NTT_N) << 1] = {
0, 0, 15749, 1600, 394, 40, 7373, 749, -8347, -848, 14095, 1432, -6201, -630, 6762, 687, 15582, 1583, -5601, -569, 679, 69, 5345, 543, -7845, -797, -1900, -193, 13879, 1410, -10453, -1062, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -8081, -8081, -13121, -13121, 13338, 13338, 10719, 10719, -821, -821, -1333, -1333, 1355, 1355, 1089, 1089, -8957, -8957, 13918, 13918, 2156, 2156, -325, -325, -910, -910, 1414, 1414, 219, 219, -33, -33, -12078, -12078, -12993, -12993, 8416, 8416, 4567, 4567, -1227, -1227, -1320, -1320, 855, 855, 464, 464, 11566, 11566, -6309, -6309, -10129, -10129, 9262, 9262, 1175, 1175, -641, -641, -1029, -1029, 941, 941, -8711, -8711, -2638, -2638, 14322, 14322, 8780, 8780, -885, -885, -268, -268, 1455, 1455, 892, 892, 3878, 3878, 9764, 9764, -11930, -11930, 10050, 10050, 394, 394, 992, 992, -1212, -1212, 1021, 1021, 11999, 11999, -7215, -7215, 15818, 15818, -9243, -9243, 1219, 1219, -733, -733, 1607, 1607, -939, -939, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4429, 4429, -551, -551, 9213, 9213, 2786, 2786, 450, 450, -56, -56, 936, 936, 283, 283, -6378, -6378, -6221, -6221, 7008, 7008, 13308, 13308, -648, -648, -632, -632, 712, 712, 1352, 1352, -14578, -14578, 8032, 8032, 6713, 6713, -6398, -6398, -1481, -1481, 816, 816, 682, 682, -650, -650, -8721, -8721, 6319, 6319, -10099, -10099, 15159, 15159, -886, -886, 642, 642, -1026, -1026, 1540, 1540, 5453, 5453, -14381, -14381, -3967, -3967, 5315, 5315, 554, 554, -1461, -1461, -403, -403, 540, 540, 11605, 11605, -9371, -9371, -10749, -10749, -16251, -16251, 1179, 1179, -952, -952, -1092, -1092, -1651, -1651, -11251, -11251, 14588, 14588, 5168, 5168, 16005, 16005, -1143, -1143, 1482, 1482, 525, 525, 1626, 1626, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4400, 4400, -14529, -14529, -5266, -5266, -13180, -13180, 447, 447, -1476, -1476, -535, -535, -1339, -1339, 9125, 9125, 12540, 12540, 4538, 4538, 10089, 10089, 927, 927, 1274, 1274, 461, 461, 1025, 1025, -15099, -15099, 10355, 10355, -14155, -14155, -11782, -11782, -1534, -1534, 1052, 1052, -1438, -1438, -1197, -1197, 7235, 7235, 2746, 2746, -7451, -7451, -2293, -2293, 735, 735, 279, 279, -757, -757, -233, -233, 8495, 8495, 3091, 3091, 5473, 5473, 472, 472, 863, 863, 314, 314, 556, 556, 48, 48, -5522, -5522, 11546, 11546, -3140, -3140, 6565, 6565, -561, -561, 1173, 1173, -319, -319, 667, 667, 12107, 12107, -7441, -7441, -10463, -10463, -13869, -13869, 1230, 1230, -756, -756, -1063, -1063, -1409, -1409, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12156, 12156, 8682, 8682, -14036, -14036, -2914, -2914, 1235, 1235, 882, 882, -1426, -1426, -296, -296, -4449, -4449, 15483, 15483, -14125, -14125, -3258, -3258, -452, -452, 1573, 1573, -1435, -1435, -331, -331, -7943, -7943, 748, 748, 9942, 9942, -2845, -2845, -807, -807, 76, 76, 1010, 1010, -289, -289, -16192, -16192, -10828, -10828, 1073, 1073, 6693, 6693, -1645, -1645, -1100, -1100, 109, 109, 680, 680, 12196, 12196, 10247, 10247, 12717, 12717, -5739, -5739, 1239, 1239, 1041, 1041, 1292, 1292, -583, -583, 7678, 7678, -7117, -7117, 10148, 10148, 5591, 5591, 780, 780, -723, -723, 1031, 1031, 568, 568, -3691, -3691, -16113, -16113, -15592, -15592, -167, -167, -375, -375, -1637, -1637, -1584, -1584, -17, -17
};

/*************************************************
* Name:        ntt
*
* Description: Inplace number-theoretic transform (NTT) in Rq.
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
// Merged NTT layer
void ntt(int16_t r[256]) {
    NTT(r);
}

/*************************************************
* Name:        invntt_tomont
*
* Description: Inplace inverse number-theoretic transform in Rq and
*              multiplication by Montgomery factor 2^16.
*              Input is in bitreversed order, output is in standard order
*
* Arguments:   - int16_t r[256] in {-(q-1)/2,...,(q-1)/2}
*              pointer to input/output vector of elements of Zq
**************************************************/
void invntt(int16_t r[256]) {
    iNTT(r);

}
