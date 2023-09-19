
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

#include <arm_neon.h>
#include "params.h"
#include "ntt.h"
#include "NTT_params.h"

const int16_t asymmetric_const[16] = {
    Q1, Q1prime2, RmodQ1, RmodQ1Q1prime, R3modQ1_prime_half, R3modQ1_doubleprime
};

const int16_t constants[16] = {
    Q1, Q1prime2, RmodQ1, RmodQ1Q1prime, roundRdivQ1,
    invNQ1_R3modQ1_prime_half,
    invNQ1_R3modQ1_doubleprime,
    invNQ1_final_R3modQ1_prime_half,
    invNQ1_final_R3modQ1_doubleprime
};

const int16_t streamlined_CT_negacyclic_table_Q1_extended[(NTT_N + (1 << 0) + (1 << 4) + NTT_N) << 1] = {
    0, 0, -15749, -1600, -7373, -749, -394, -40, -6762, -687, 6201, 630, -14095, -1432, 8347, 848, 10453, 1062, -13879, -1410, 1900, 193, 7845, 797, -5345, -543, -679, -69, 5601, 569, -15582, -1583, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2914, 296, 2914, 296, 14036, 1426, 14036, 1426, -8682, -882, -8682, -882, -12156, -1235, -12156, -1235, 2845, 289, 2845, 289, -9942, -1010, -9942, -1010, -748, -76, -748, -76, 7943, 807, 7943, 807, 3258, 331, 3258, 331, 14125, 1435, 14125, 1435, -15483, -1573, -15483, -1573, 4449, 452, 4449, 452, 167, 17, 167, 17, 15592, 1584, 15592, 1584, 16113, 1637, 16113, 1637, 3691, 375, 3691, 375, -5591, -568, -5591, -568, -10148, -1031, -10148, -1031, 7117, 723, 7117, 723, -7678, -780, -7678, -780, 5739, 583, 5739, 583, -12717, -1292, -12717, -1292, -10247, -1041, -10247, -1041, -12196, -1239, -12196, -1239, -6693, -680, -6693, -680, -1073, -109, -1073, -109, 10828, 1100, 10828, 1100, 16192, 1645, 16192, 1645, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13180, 1339, 13180, 1339, 5266, 535, 5266, 535, 14529, 1476, 14529, 1476, -4400, -447, -4400, -447, 11782, 1197, 11782, 1197, 14155, 1438, 14155, 1438, -10355, -1052, -10355, -1052, 15099, 1534, 15099, 1534, -10089, -1025, -10089, -1025, -4538, -461, -4538, -461, -12540, -1274, -12540, -1274, -9125, -927, -9125, -927, 13869, 1409, 13869, 1409, 10463, 1063, 10463, 1063, 7441, 756, 7441, 756, -12107, -1230, -12107, -1230, -6565, -667, -6565, -667, 3140, 319, 3140, 319, -11546, -1173, -11546, -1173, 5522, 561, 5522, 561, -472, -48, -472, -48, -5473, -556, -5473, -556, -3091, -314, -3091, -314, -8495, -863, -8495, -863, 2293, 233, 2293, 233, 7451, 757, 7451, 757, -2746, -279, -2746, -279, -7235, -735, -7235, -735, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2786, -283, -2786, -283, -9213, -936, -9213, -936, 551, 56, 551, 56, -4429, -450, -4429, -450, 6398, 650, 6398, 650, -6713, -682, -6713, -682, -8032, -816, -8032, -816, 14578, 1481, 14578, 1481, -13308, -1352, -13308, -1352, -7008, -712, -7008, -712, 6221, 632, 6221, 632, 6378, 648, 6378, 648, -16005, -1626, -16005, -1626, -5168, -525, -5168, -525, -14588, -1482, -14588, -1482, 11251, 1143, 11251, 1143, 16251, 1651, 16251, 1651, 10749, 1092, 10749, 1092, 9371, 952, 9371, 952, -11605, -1179, -11605, -1179, -5315, -540, -5315, -540, 3967, 403, 3967, 403, 14381, 1461, 14381, 1461, -5453, -554, -5453, -554, -15159, -1540, -15159, -1540, 10099, 1026, 10099, 1026, -6319, -642, -6319, -642, 8721, 886, 8721, 886, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10719, -1089, -10719, -1089, -13338, -1355, -13338, -1355, 13121, 1333, 13121, 1333, 8081, 821, 8081, 821, -4567, -464, -4567, -464, -8416, -855, -8416, -855, 12993, 1320, 12993, 1320, 12078, 1227, 12078, 1227, 325, 33, 325, 33, -2156, -219, -2156, -219, -13918, -1414, -13918, -1414, 8957, 910, 8957, 910, 9243, 939, 9243, 939, -15818, -1607, -15818, -1607, 7215, 733, 7215, 733, -11999, -1219, -11999, -1219, -10050, -1021, -10050, -1021, 11930, 1212, 11930, 1212, -9764, -992, -9764, -992, -3878, -394, -3878, -394, -8780, -892, -8780, -892, -14322, -1455, -14322, -1455, 2638, 268, 2638, 268, 8711, 885, 8711, 885, -9262, -941, -9262, -941, 10129, 1029, 10129, 1029, 6309, 641, 6309, 641, -11566, -1175, -11566, -1175, 0, 0
};

const int16_t streamlined_CT_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4) + NTT_N) << 1] = {
    0, 0, -15749, -1600, -7373, -749, -394, -40, -6762, -687, 6201, 630, -14095, -1432, 8347, 848, 10453, 1062, -13879, -1410, 1900, 193, 7845, 797, -5345, -543, -679, -69, 5601, 569, -15582, -1583, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2914, 2914, 14036, 14036, -8682, -8682, -12156, -12156, 296, 296, 1426, 1426, -882, -882, -1235, -1235, 2845, 2845, -9942, -9942, -748, -748, 7943, 7943, 289, 289, -1010, -1010, -76, -76, 807, 807, 3258, 3258, 14125, 14125, -15483, -15483, 4449, 4449, 331, 331, 1435, 1435, -1573, -1573, 452, 452, 167, 167, 15592, 15592, 16113, 16113, 3691, 3691, 17, 17, 1584, 1584, 1637, 1637, 375, 375, -5591, -5591, -10148, -10148, 7117, 7117, -7678, -7678, -568, -568, -1031, -1031, 723, 723, -780, -780, 5739, 5739, -12717, -12717, -10247, -10247, -12196, -12196, 583, 583, -1292, -1292, -1041, -1041, -1239, -1239, -6693, -6693, -1073, -1073, 10828, 10828, 16192, 16192, -680, -680, -109, -109, 1100, 1100, 1645, 1645, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13180, 13180, 5266, 5266, 14529, 14529, -4400, -4400, 1339, 1339, 535, 535, 1476, 1476, -447, -447, 11782, 11782, 14155, 14155, -10355, -10355, 15099, 15099, 1197, 1197, 1438, 1438, -1052, -1052, 1534, 1534, -10089, -10089, -4538, -4538, -12540, -12540, -9125, -9125, -1025, -1025, -461, -461, -1274, -1274, -927, -927, 13869, 13869, 10463, 10463, 7441, 7441, -12107, -12107, 1409, 1409, 1063, 1063, 756, 756, -1230, -1230, -6565, -6565, 3140, 3140, -11546, -11546, 5522, 5522, -667, -667, 319, 319, -1173, -1173, 561, 561, -472, -472, -5473, -5473, -3091, -3091, -8495, -8495, -48, -48, -556, -556, -314, -314, -863, -863, 2293, 2293, 7451, 7451, -2746, -2746, -7235, -7235, 233, 233, 757, 757, -279, -279, -735, -735, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2786, -2786, -9213, -9213, 551, 551, -4429, -4429, -283, -283, -936, -936, 56, 56, -450, -450, 6398, 6398, -6713, -6713, -8032, -8032, 14578, 14578, 650, 650, -682, -682, -816, -816, 1481, 1481, -13308, -13308, -7008, -7008, 6221, 6221, 6378, 6378, -1352, -1352, -712, -712, 632, 632, 648, 648, -16005, -16005, -5168, -5168, -14588, -14588, 11251, 11251, -1626, -1626, -525, -525, -1482, -1482, 1143, 1143, 16251, 16251, 10749, 10749, 9371, 9371, -11605, -11605, 1651, 1651, 1092, 1092, 952, 952, -1179, -1179, -5315, -5315, 3967, 3967, 14381, 14381, -5453, -5453, -540, -540, 403, 403, 1461, 1461, -554, -554, -15159, -15159, 10099, 10099, -6319, -6319, 8721, 8721, -1540, -1540, 1026, 1026, -642, -642, 886, 886, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10719, -10719, -13338, -13338, 13121, 13121, 8081, 8081, -1089, -1089, -1355, -1355, 1333, 1333, 821, 821, -4567, -4567, -8416, -8416, 12993, 12993, 12078, 12078, -464, -464, -855, -855, 1320, 1320, 1227, 1227, 325, 325, -2156, -2156, -13918, -13918, 8957, 8957, 33, 33, -219, -219, -1414, -1414, 910, 910, 9243, 9243, -15818, -15818, 7215, 7215, -11999, -11999, 939, 939, -1607, -1607, 733, 733, -1219, -1219, -10050, -10050, 11930, 11930, -9764, -9764, -3878, -3878, -1021, -1021, 1212, 1212, -992, -992, -394, -394, -8780, -8780, -14322, -14322, 2638, 2638, 8711, 8711, -892, -892, -1455, -1455, 268, 268, 885, 885, -9262, -9262, 10129, 10129, 6309, 6309, -11566, -11566, -941, -941, 1029, 1029, 641, 641, -1175, -1175
};

const int16_t pre_asymmetric_table_Q1_extended[ARRAY_N] = {
    167, 17, -167, -17, -5591, -568, 5591, 568, 5739, 583, -5739, -583, -6693, -680, 6693, 680, 16113, 1637, -16113, -1637, 7117, 723, -7117, -723, -10247, -1041, 10247, 1041, 10828, 1100, -10828, -1100, 13869, 1409, -13869, -1409, -6565, -667, 6565, 667, -472, -48, 472, 48, 2293, 233, -2293, -233, 7441, 756, -7441, -756, -11546, -1173, 11546, 1173, -3091, -314, 3091, 314, -2746, -279, 2746, 279, -16005, -1626, 16005, 1626, 16251, 1651, -16251, -1651, -5315, -540, 5315, 540, -15159, -1540, 15159, 1540, -14588, -1482, 14588, 1482, 9371, 952, -9371, -952, 14381, 1461, -14381, -1461, -6319, -642, 6319, 642, 9243, 939, -9243, -939, -10050, -1021, 10050, 1021, -8780, -892, 8780, 892, -9262, -941, 9262, 941, 7215, 733, -7215, -733, -9764, -992, 9764, 992, 2638, 268, -2638, -268, 6309, 641, -6309, -641, 15592, 1584, -15592, -1584, -10148, -1031, 10148, 1031, -12717, -1292, 12717, 1292, -1073, -109, 1073, 109, 3691, 375, -3691, -375, -7678, -780, 7678, 780, -12196, -1239, 12196, 1239, 16192, 1645, -16192, -1645, 10463, 1063, -10463, -1063, 3140, 319, -3140, -319, -5473, -556, 5473, 556, 7451, 757, -7451, -757, -12107, -1230, 12107, 1230, 5522, 561, -5522, -561, -8495, -863, 8495, 863, -7235, -735, 7235, 735, -5168, -525, 5168, 525, 10749, 1092, -10749, -1092, 3967, 403, -3967, -403, 10099, 1026, -10099, -1026, 11251, 1143, -11251, -1143, -11605, -1179, 11605, 1179, -5453, -554, 5453, 554, 8721, 886, -8721, -886, -15818, -1607, 15818, 1607, 11930, 1212, -11930, -1212, -14322, -1455, 14322, 1455, 10129, 1029, -10129, -1029, -11999, -1219, 11999, 1219, -3878, -394, 3878, 394, 8711, 885, -8711, -885, -11566, -1175, 11566, 1175
};

const int16_t streamlined_inv_CT_table_Q1_extended[(NTT_N + (1 << 0) + (1 << 4) + NTT_N) << 1] = {
    0, 0, 15749, 1600, 394, 40, 7373, 749, -8347, -848, 14095, 1432, -6201, -630, 6762, 687, 15582, 1583, -5601, -569, 679, 69, 5345, 543, -7845, -797, -1900, -193, 13879, 1410, -10453, -1062, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -8081, -821, -8081, -821, -13121, -1333, -13121, -1333, 13338, 1355, 13338, 1355, 10719, 1089, 10719, 1089, -8957, -910, -8957, -910, 13918, 1414, 13918, 1414, 2156, 219, 2156, 219, -325, -33, -325, -33, -12078, -1227, -12078, -1227, -12993, -1320, -12993, -1320, 8416, 855, 8416, 855, 4567, 464, 4567, 464, 11566, 1175, 11566, 1175, -6309, -641, -6309, -641, -10129, -1029, -10129, -1029, 9262, 941, 9262, 941, -8711, -885, -8711, -885, -2638, -268, -2638, -268, 14322, 1455, 14322, 1455, 8780, 892, 8780, 892, 3878, 394, 3878, 394, 9764, 992, 9764, 992, -11930, -1212, -11930, -1212, 10050, 1021, 10050, 1021, 11999, 1219, 11999, 1219, -7215, -733, -7215, -733, 15818, 1607, 15818, 1607, -9243, -939, -9243, -939, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4429, 450, 4429, 450, -551, -56, -551, -56, 9213, 936, 9213, 936, 2786, 283, 2786, 283, -6378, -648, -6378, -648, -6221, -632, -6221, -632, 7008, 712, 7008, 712, 13308, 1352, 13308, 1352, -14578, -1481, -14578, -1481, 8032, 816, 8032, 816, 6713, 682, 6713, 682, -6398, -650, -6398, -650, -8721, -886, -8721, -886, 6319, 642, 6319, 642, -10099, -1026, -10099, -1026, 15159, 1540, 15159, 1540, 5453, 554, 5453, 554, -14381, -1461, -14381, -1461, -3967, -403, -3967, -403, 5315, 540, 5315, 540, 11605, 1179, 11605, 1179, -9371, -952, -9371, -952, -10749, -1092, -10749, -1092, -16251, -1651, -16251, -1651, -11251, -1143, -11251, -1143, 14588, 1482, 14588, 1482, 5168, 525, 5168, 525, 16005, 1626, 16005, 1626, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4400, 447, 4400, 447, -14529, -1476, -14529, -1476, -5266, -535, -5266, -535, -13180, -1339, -13180, -1339, 9125, 927, 9125, 927, 12540, 1274, 12540, 1274, 4538, 461, 4538, 461, 10089, 1025, 10089, 1025, -15099, -1534, -15099, -1534, 10355, 1052, 10355, 1052, -14155, -1438, -14155, -1438, -11782, -1197, -11782, -1197, 7235, 735, 7235, 735, 2746, 279, 2746, 279, -7451, -757, -7451, -757, -2293, -233, -2293, -233, 8495, 863, 8495, 863, 3091, 314, 3091, 314, 5473, 556, 5473, 556, 472, 48, 472, 48, -5522, -561, -5522, -561, 11546, 1173, 11546, 1173, -3140, -319, -3140, -319, 6565, 667, 6565, 667, 12107, 1230, 12107, 1230, -7441, -756, -7441, -756, -10463, -1063, -10463, -1063, -13869, -1409, -13869, -1409, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12156, 1235, 12156, 1235, 8682, 882, 8682, 882, -14036, -1426, -14036, -1426, -2914, -296, -2914, -296, -4449, -452, -4449, -452, 15483, 1573, 15483, 1573, -14125, -1435, -14125, -1435, -3258, -331, -3258, -331, -7943, -807, -7943, -807, 748, 76, 748, 76, 9942, 1010, 9942, 1010, -2845, -289, -2845, -289, -16192, -1645, -16192, -1645, -10828, -1100, -10828, -1100, 1073, 109, 1073, 109, 6693, 680, 6693, 680, 12196, 1239, 12196, 1239, 10247, 1041, 10247, 1041, 12717, 1292, 12717, 1292, -5739, -583, -5739, -583, 7678, 780, 7678, 780, -7117, -723, -7117, -723, 10148, 1031, 10148, 1031, 5591, 568, 5591, 568, -3691, -375, -3691, -375, -16113, -1637, -16113, -1637, -15592, -1584, -15592, -1584, -167, -17, -167, -17, 0, 0
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
