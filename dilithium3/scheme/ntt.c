
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

#include "params.h"
#include "reduce.h"
#include <stdint.h>
#include <stdio.h>

#include "NTT_params.h"
#include "ntt.h"

const int32_t constants[16] = {
    Q1, -Q1prime, RmodQ1_prime_half, RmodQ1_doubleprime,
    invNQ1R2modQ1_prime_half,
    invNQ1R2modQ1_doubleprime,
    invNQ1_final_R2modQ1_prime_half,
    invNQ1_final_R2modQ1_doubleprime
};

const int32_t streamlined_CT_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1] = {
0, 0, -915382907, -3572223, 964937599, 3765607, 963888510, 3761513, -820383522, -3201494, -738955404, -2883726, -806080660, -3145678, -820367122, -3201430, -154181397, -601683, 907762539, 3542485, 687336873, 2682288, 545785280, 2129892, 964747974, 3764867, -257592709, -1005239, 142848732, 557458, -312926867, -1221177, 8380417, 0, -863652652, -3370349, 923069133, 3602218, 815613168, 3182878, 787459213, 327391679, -675340520, 987079667, 3073009, 1277625, -2635473, 3852015, 449207, -681503850, 681730119, -15156688, 1753, -2659525, 2660408, -59148, -495951789, -373072124, -456183549, 710479343, -1935420, -1455890, -1780227, 2772600, 8380417, 0, -1041158200, -4063053, 702264730, 2740543, -919027554, -3586446, 1071989969, -825844983, -799869667, -70227934, 4183372, -3222807, -3121440, -274060, 302950022, 163212680, -1013916752, -841760171, 1182243, 636927, -3956745, -3284915, 22347069, -1016110510, -588452222, -952468207, 87208, -3965306, -2296397, -3716946, 8380417, 0, 682491182, 2663378, -797147778, -3110818, 538486762, 2101410, 642926661, 519705671, 496502727, -977780347, 2508980, 2028118, 1937570, -3815725, -7126831, 258649997, -507246529, -1013967746, -27812, 1009365, -1979497, -3956944, 210776307, -628875181, 409185979, -963363710, 822541, -2454145, 1596822, -3759465, 8380417, 0, -429120452, -1674615, 949361686, 3704823, 297218217, 1159875, 720393920, -764594519, -284313712, 1065510939, 2811291, -2983781, -1109516, 4158088, -431820817, 686309310, -909946047, -64176841, -1685153, 2678278, -3551006, -250446, -873958779, -965793731, 162963861, -629190881, -3410568, -3768948, 635956, -2455377, 8380417, 0, -903139016, -3524442, 101000509, 394148, 237992130, 928749, 391567239, 123678909, 294395108, -759080783, 1528066, 482649, 1148858, -2962264, -1062481036, 561940831, 611800717, -68791907, -4146264, 2192938, 2387513, -268456, -454226054, -442566669, -925511710, -814992530, -1772588, -1727088, -3611750, -3180456, 8380417, 0, -111244624, -434125, 280713909, 1095468, -898510625, -3506380, -144935890, 43482586, 631001801, -854436357, -565603, 169688, 2462444, -3334383, 960233614, 317727459, 818892658, 321386456, 3747250, 1239911, 3195676, 1254190, 588375860, -983611064, 677264190, -3181859, 2296099, -3838479, 2642980, -12417, 8380417, 0, 173376332, 676590, 530906624, 2071829, -1029866791, -4018989, -1067647297, -893898890, 509377762, -819295484, -4166425, -3488383, 1987814, -3197248, 768294260, -22883400, -347191365, -335754661, 2998219, -89301, -1354892, -1310261, 36345249, 643961400, 157142369, -568482643, 141835, 2513018, 613238, -2218467, 8380417, 0, -342333886, -1335936, 830756018, 3241972, 552488273, 2156050, 444930577, 60323094, -832852657, 834980303, 1736313, 235407, -3250154, 3258457, -117552223, 1035301089, 522531086, -209807681, -458740, 4040196, 2039144, -818761, -492511373, -889718424, -481719139, -558360247, -1921994, -3472069, -1879878, -2178965, 8380417, 0, -827143915, -3227876, 875112161, 3415069, 450833045, 1759347, -660934133, 458160776, -612717067, -577774276, -2579253, 1787943, -2391089, -2254727, -415984810, -608441020, 150224382, 135295244, -1623354, -2374402, 586241, 527981, 539479988, -521163479, -302276083, -702999655, 2105286, -2033807, -1179613, -2743411, 8380417, 0, 439288460, 1714295, -209493775, -817536, -915957677, -3574466, 892316032, -1071872863, -333129378, -605279149, 3482206, -4182915, -1300016, -2362063, -378477722, 638402564, 130156402, -185731180, -1476985, 2491325, 507927, -724804, 510974714, -356997292, -304395785, -470097680, 1994046, -1393159, -1187885, -1834526, 8380417, 0, 628833668, 2453983, 962678241, 3756790, -496048908, -1935799, -337655269, 630730945, 777970524, 159173408, -1317678, 2461387, 3035980, 621164, -777397036, 678549029, -669544140, 192079267, -3033742, 2647994, -2612853, 749577, -86720197, 771248568, 1063046068, -1030830548, -338420, 3009748, 4148469, -4022750, 8380417, 0, 374309300, 1460718, -439978542, -1716988, -1012201926, -3950053, 999753034, -314332144, 749740976, 864652284, 3901472, -1226661, 2925816, 3374250, 1020029345, -413979908, 426738094, 298172236, 3980599, -1615530, 1665318, 1163598, 658309618, 441577800, 519685171, -863376927, 2569011, 1723229, 2028038, -3369273, 8380417, 0, -164673562, -642628, -742437332, -2897314, 818041395, 3192354, 347590090, -711287812, 687588511, -712065019, 1356448, -2775755, 2683270, -2778788, 1023635298, -351195274, 861908357, 139752717, 3994671, -1370517, 3363542, 545376, -3043996, 773976352, 55063046, -197425671, -11879, 3020393, 214880, -770441, 8380417, 0, -918682129, -3585098, 142694469, 556856, 991769559, 3870317, -888589898, 592665232, -167401858, -117660617, -3467665, 2312838, -653275, -459163, 795799901, 130212265, 220412084, 35937555, 3105558, 508145, 860144, 140244, -282732136, -141890356, 879049958, -388001774, -1103344, -553718, 3430436, -1514152, 8380417, 0, 721508096, 2815639, 747568486, 2917338, 475038184, 1853806, 89383150, -84011120, 259126110, -603268097, 348812, -327848, 1011223, -2354215, -559928242, 604333585, -772445769, 749801963, -2185084, 2358373, -3014420, 2926054, 800464680, -561979013, -439933955, -100631253, 3123762, -2193087, -1716814, -392707, 8380417, 0, 585207070, 2283733, 857403734, 3345963, 476219497, 1858416, -978523985, -492577742, -573161516, 447030292, -3818627, -1922253, -2236726, 1744507, -77645096, -1018462631, 486888731, 270210213, -303005, -3974485, 1900052, 1054478, 904878186, -967019376, -200355636, -187430119, 3531229, -3773731, -781875, -731434
};

const int32_t streamlined_GS_itable_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1] = {
0, 0, 915382907, 3572223, -963888510, -3761513, -964937599, -3765607, 820367122, 3201430, 806080660, 3145678, 738955404, 2883726, 820383522, 3201494, 312926867, 1221177, -142848732, -557458, 257592709, 1005239, -964747974, -3764867, -545785280, -2129892, -687336873, -2682288, -907762539, -3542485, 154181397, 601683, 8380417, 0, -585207070, -2283733, -476219497, -1858416, -857403734, -3345963, -447030292, 573161516, 492577742, 978523985, -1744507, 2236726, 1922253, 3818627, 187430119, 200355636, 967019376, -904878186, 731434, 781875, 3773731, -3531229, -270210213, -486888731, 1018462631, 77645096, -1054478, -1900052, 3974485, 303005, 8380417, 0, -721508096, -2815639, -475038184, -1853806, -747568486, -2917338, 603268097, -259126110, 84011120, -89383150, 2354215, -1011223, 327848, -348812, 100631253, 439933955, 561979013, -800464680, 392707, 1716814, 2193087, -3123762, -749801963, 772445769, -604333585, 559928242, -2926054, 3014420, -2358373, 2185084, 8380417, 0, 918682129, 3585098, -991769559, -3870317, -142694469, -556856, 117660617, 167401858, -592665232, 888589898, 459163, 653275, -2312838, 3467665, 388001774, -879049958, 141890356, 282732136, 1514152, -3430436, 553718, 1103344, -35937555, -220412084, -130212265, -795799901, -140244, -860144, -508145, -3105558, 8380417, 0, 164673562, 642628, -818041395, -3192354, 742437332, 2897314, 712065019, -687588511, 711287812, -347590090, 2778788, -2683270, 2775755, -1356448, 197425671, -55063046, -773976352, 3043996, 770441, -214880, -3020393, 11879, -139752717, -861908357, 351195274, -1023635298, -545376, -3363542, 1370517, -3994671, 8380417, 0, -374309300, -1460718, 1012201926, 3950053, 439978542, 1716988, -864652284, -749740976, 314332144, -999753034, -3374250, -2925816, 1226661, -3901472, 863376927, -519685171, -441577800, -658309618, 3369273, -2028038, -1723229, -2569011, -298172236, -426738094, 413979908, -1020029345, -1163598, -1665318, 1615530, -3980599, 8380417, 0, -628833668, -2453983, 496048908, 1935799, -962678241, -3756790, -159173408, -777970524, -630730945, 337655269, -621164, -3035980, -2461387, 1317678, 1030830548, -1063046068, -771248568, 86720197, 4022750, -4148469, -3009748, 338420, -192079267, 669544140, -678549029, 777397036, -749577, 2612853, -2647994, 3033742, 8380417, 0, -439288460, -1714295, 915957677, 3574466, 209493775, 817536, 605279149, 333129378, 1071872863, -892316032, 2362063, 1300016, 4182915, -3482206, 470097680, 304395785, 356997292, -510974714, 1834526, 1187885, 1393159, -1994046, 185731180, -130156402, -638402564, 378477722, 724804, -507927, -2491325, 1476985, 8380417, 0, 827143915, 3227876, -450833045, -1759347, -875112161, -3415069, 577774276, 612717067, -458160776, 660934133, 2254727, 2391089, -1787943, 2579253, 702999655, 302276083, 521163479, -539479988, 2743411, 1179613, 2033807, -2105286, -135295244, -150224382, 608441020, 415984810, -527981, -586241, 2374402, 1623354, 8380417, 0, 342333886, 1335936, -552488273, -2156050, -830756018, -3241972, -834980303, 832852657, -60323094, -444930577, -3258457, 3250154, -235407, -1736313, 558360247, 481719139, 889718424, 492511373, 2178965, 1879878, 3472069, 1921994, 209807681, -522531086, -1035301089, 117552223, 818761, -2039144, -4040196, 458740, 8380417, 0, -173376332, -676590, 1029866791, 4018989, -530906624, -2071829, 819295484, -509377762, 893898890, 1067647297, 3197248, -1987814, 3488383, 4166425, 568482643, -157142369, -643961400, -36345249, 2218467, -613238, -2513018, -141835, 335754661, 347191365, 22883400, -768294260, 1310261, 1354892, 89301, -2998219, 8380417, 0, 111244624, 434125, 898510625, 3506380, -280713909, -1095468, 854436357, -631001801, -43482586, 144935890, 3334383, -2462444, -169688, 565603, 3181859, -677264190, 983611064, -588375860, 12417, -2642980, 3838479, -2296099, -321386456, -818892658, -317727459, -960233614, -1254190, -3195676, -1239911, -3747250, 8380417, 0, 903139016, 3524442, -237992130, -928749, -101000509, -394148, 759080783, -294395108, -123678909, -391567239, 2962264, -1148858, -482649, -1528066, 814992530, 925511710, 442566669, 454226054, 3180456, 3611750, 1727088, 1772588, 68791907, -611800717, -561940831, 1062481036, 268456, -2387513, -2192938, 4146264, 8380417, 0, 429120452, 1674615, -297218217, -1159875, -949361686, -3704823, -1065510939, 284313712, 764594519, -720393920, -4158088, 1109516, 2983781, -2811291, 629190881, -162963861, 965793731, 873958779, 2455377, -635956, 3768948, 3410568, 64176841, 909946047, -686309310, 431820817, 250446, 3551006, -2678278, 1685153, 8380417, 0, -682491182, -2663378, -538486762, -2101410, 797147778, 3110818, 977780347, -496502727, -519705671, -642926661, 3815725, -1937570, -2028118, -2508980, 963363710, -409185979, 628875181, -210776307, 3759465, -1596822, 2454145, -822541, 1013967746, 507246529, -258649997, 7126831, 3956944, 1979497, -1009365, 27812, 8380417, 0, 1041158200, 4063053, 919027554, 3586446, -702264730, -2740543, 70227934, 799869667, 825844983, -1071989969, 274060, 3121440, 3222807, -4183372, 952468207, 588452222, 1016110510, -22347069, 3716946, 2296397, 3965306, -87208, 841760171, 1013916752, -163212680, -302950022, 3284915, 3956745, -636927, -1182243, 8380417, 0, 863652652, 3370349, -815613168, -3182878, -923069133, -3602218, -987079667, 675340520, -327391679, -787459213, -3852015, 2635473, -1277625, -3073009, -710479343, 456183549, 373072124, 495951789, -2772600, 1780227, 1455890, 1935420, 15156688, -681730119, 681503850, -449207, 59148, -2660408, 2659525, -1753
};

/*************************************************
* Name:        ntt
*
* Description: Forward NTT, in-place. No modular reduction is performed after
*              additions or subtractions. Output vector is in bitreversed order.
*
* Arguments:   - uint32_t p[N]: input/output coefficient array
**************************************************/
void ntt(int32_t a[N]) {
    NTT(a);
}

/*************************************************
* Name:        invntt_tomont
*
* Description: Inverse NTT and multiplication by Montgomery factor 2^32.
*              In-place. No modular reductions after additions or
*              subtractions; input coefficients need to be smaller than
*              Q in absolute value. Output coefficient are smaller than Q in
*              absolute value.
*
* Arguments:   - uint32_t p[N]: input/output coefficient array
**************************************************/
void invntt_tomont(int32_t a[N]) {
    iNTT(a);
}
