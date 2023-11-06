
/*
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

// table of constants generated by https://github.com/neon-ntt/neon-ntt/blob/master/gen_table/Dilithium/call_gen.c

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "NTT_params.h"
#include "gen_table.h"
#include "tools.h"

#define BUFF_MAX (NTT_N << 4)

struct compress_profile profile;

static const int32_t streamlined_CT_negacyclic_table_Q1_extended[((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1] = {
0, 0, -915382907, -3572223, 964937599, 3765607, 963888510, 3761513, -820383522, -3201494, -738955404, -2883726, -806080660, -3145678, -820367122, -3201430, -154181397, -601683, 907762539, 3542485, 687336873, 2682288, 545785280, 2129892, 964747974, 3764867, -257592709, -1005239, 142848732, 557458, -312926867, -1221177, 0, 0, -863652652, -3370349, 923069133, 3602218, 815613168, 3182878, 787459213, 3073009, 327391679, 1277625, -675340520, -2635473, 987079667, 3852015, 449207, 1753, -495951789, -1935420, -681503850, -2659525, -373072124, -1455890, 681730119, 2660408, -456183549, -1780227, -15156688, -59148, 710479343, 2772600, 0, 0, -1041158200, -4063053, 702264730, 2740543, -919027554, -3586446, 1071989969, 4183372, -825844983, -3222807, -799869667, -3121440, -70227934, -274060, 302950022, 1182243, 22347069, 87208, 163212680, 636927, -1016110510, -3965306, -1013916752, -3956745, -588452222, -2296397, -841760171, -3284915, -952468207, -3716946, 0, 0, 682491182, 2663378, -797147778, -3110818, 538486762, 2101410, 642926661, 2508980, 519705671, 2028118, 496502727, 1937570, -977780347, -3815725, -7126831, -27812, 210776307, 822541, 258649997, 1009365, -628875181, -2454145, -507246529, -1979497, 409185979, 1596822, -1013967746, -3956944, -963363710, -3759465, 0, 0, -429120452, -1674615, 949361686, 3704823, 297218217, 1159875, 720393920, 2811291, -764594519, -2983781, -284313712, -1109516, 1065510939, 4158088, -431820817, -1685153, -873958779, -3410568, 686309310, 2678278, -965793731, -3768948, -909946047, -3551006, 162963861, 635956, -64176841, -250446, -629190881, -2455377, 0, 0, -903139016, -3524442, 101000509, 394148, 237992130, 928749, 391567239, 1528066, 123678909, 482649, 294395108, 1148858, -759080783, -2962264, -1062481036, -4146264, -454226054, -1772588, 561940831, 2192938, -442566669, -1727088, 611800717, 2387513, -925511710, -3611750, -68791907, -268456, -814992530, -3180456, 0, 0, -111244624, -434125, 280713909, 1095468, -898510625, -3506380, -144935890, -565603, 43482586, 169688, 631001801, 2462444, -854436357, -3334383, 960233614, 3747250, 588375860, 2296099, 317727459, 1239911, -983611064, -3838479, 818892658, 3195676, 677264190, 2642980, 321386456, 1254190, -3181859, -12417, 0, 0, 173376332, 676590, 530906624, 2071829, -1029866791, -4018989, -1067647297, -4166425, -893898890, -3488383, 509377762, 1987814, -819295484, -3197248, 768294260, 2998219, 36345249, 141835, -22883400, -89301, 643961400, 2513018, -347191365, -1354892, 157142369, 613238, -335754661, -1310261, -568482643, -2218467, 0, 0, -342333886, -1335936, 830756018, 3241972, 552488273, 2156050, 444930577, 1736313, 60323094, 235407, -832852657, -3250154, 834980303, 3258457, -117552223, -458740, -492511373, -1921994, 1035301089, 4040196, -889718424, -3472069, 522531086, 2039144, -481719139, -1879878, -209807681, -818761, -558360247, -2178965, 0, 0, -827143915, -3227876, 875112161, 3415069, 450833045, 1759347, -660934133, -2579253, 458160776, 1787943, -612717067, -2391089, -577774276, -2254727, -415984810, -1623354, 539479988, 2105286, -608441020, -2374402, -521163479, -2033807, 150224382, 586241, -302276083, -1179613, 135295244, 527981, -702999655, -2743411, 0, 0, 439288460, 1714295, -209493775, -817536, -915957677, -3574466, 892316032, 3482206, -1071872863, -4182915, -333129378, -1300016, -605279149, -2362063, -378477722, -1476985, 510974714, 1994046, 638402564, 2491325, -356997292, -1393159, 130156402, 507927, -304395785, -1187885, -185731180, -724804, -470097680, -1834526, 0, 0, 628833668, 2453983, 962678241, 3756790, -496048908, -1935799, -337655269, -1317678, 630730945, 2461387, 777970524, 3035980, 159173408, 621164, -777397036, -3033742, -86720197, -338420, 678549029, 2647994, 771248568, 3009748, -669544140, -2612853, 1063046068, 4148469, 192079267, 749577, -1030830548, -4022750, 0, 0, 374309300, 1460718, -439978542, -1716988, -1012201926, -3950053, 999753034, 3901472, -314332144, -1226661, 749740976, 2925816, 864652284, 3374250, 1020029345, 3980599, 658309618, 2569011, -413979908, -1615530, 441577800, 1723229, 426738094, 1665318, 519685171, 2028038, 298172236, 1163598, -863376927, -3369273, 0, 0, -164673562, -642628, -742437332, -2897314, 818041395, 3192354, 347590090, 1356448, -711287812, -2775755, 687588511, 2683270, -712065019, -2778788, 1023635298, 3994671, -3043996, -11879, -351195274, -1370517, 773976352, 3020393, 861908357, 3363542, 55063046, 214880, 139752717, 545376, -197425671, -770441, 0, 0, -918682129, -3585098, 142694469, 556856, 991769559, 3870317, -888589898, -3467665, 592665232, 2312838, -167401858, -653275, -117660617, -459163, 795799901, 3105558, -282732136, -1103344, 130212265, 508145, -141890356, -553718, 220412084, 860144, 879049958, 3430436, 35937555, 140244, -388001774, -1514152, 0, 0, 721508096, 2815639, 747568486, 2917338, 475038184, 1853806, 89383150, 348812, -84011120, -327848, 259126110, 1011223, -603268097, -2354215, -559928242, -2185084, 800464680, 3123762, 604333585, 2358373, -561979013, -2193087, -772445769, -3014420, -439933955, -1716814, 749801963, 2926054, -100631253, -392707, 0, 0, 585207070, 2283733, 857403734, 3345963, 476219497, 1858416, -978523985, -3818627, -492577742, -1922253, -573161516, -2236726, 447030292, 1744507, -77645096, -303005, 904878186, 3531229, -1018462631, -3974485, -967019376, -3773731, 486888731, 1900052, -200355636, -781875, 270210213, 1054478, -187430119, -731434
};

static const int32_t streamlined_CT_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1] = {
0, 0, -915382907, -3572223, 964937599, 3765607, 963888510, 3761513, -820383522, -3201494, -738955404, -2883726, -806080660, -3145678, -820367122, -3201430, -154181397, -601683, 907762539, 3542485, 687336873, 2682288, 545785280, 2129892, 964747974, 3764867, -257592709, -1005239, 142848732, 557458, -312926867, -1221177, 8380417, 0, -863652652, -3370349, 923069133, 3602218, 815613168, 3182878, 787459213, 327391679, -675340520, 987079667, 3073009, 1277625, -2635473, 3852015, 449207, -681503850, 681730119, -15156688, 1753, -2659525, 2660408, -59148, -495951789, -373072124, -456183549, 710479343, -1935420, -1455890, -1780227, 2772600, 8380417, 0, -1041158200, -4063053, 702264730, 2740543, -919027554, -3586446, 1071989969, -825844983, -799869667, -70227934, 4183372, -3222807, -3121440, -274060, 302950022, 163212680, -1013916752, -841760171, 1182243, 636927, -3956745, -3284915, 22347069, -1016110510, -588452222, -952468207, 87208, -3965306, -2296397, -3716946, 8380417, 0, 682491182, 2663378, -797147778, -3110818, 538486762, 2101410, 642926661, 519705671, 496502727, -977780347, 2508980, 2028118, 1937570, -3815725, -7126831, 258649997, -507246529, -1013967746, -27812, 1009365, -1979497, -3956944, 210776307, -628875181, 409185979, -963363710, 822541, -2454145, 1596822, -3759465, 8380417, 0, -429120452, -1674615, 949361686, 3704823, 297218217, 1159875, 720393920, -764594519, -284313712, 1065510939, 2811291, -2983781, -1109516, 4158088, -431820817, 686309310, -909946047, -64176841, -1685153, 2678278, -3551006, -250446, -873958779, -965793731, 162963861, -629190881, -3410568, -3768948, 635956, -2455377, 8380417, 0, -903139016, -3524442, 101000509, 394148, 237992130, 928749, 391567239, 123678909, 294395108, -759080783, 1528066, 482649, 1148858, -2962264, -1062481036, 561940831, 611800717, -68791907, -4146264, 2192938, 2387513, -268456, -454226054, -442566669, -925511710, -814992530, -1772588, -1727088, -3611750, -3180456, 8380417, 0, -111244624, -434125, 280713909, 1095468, -898510625, -3506380, -144935890, 43482586, 631001801, -854436357, -565603, 169688, 2462444, -3334383, 960233614, 317727459, 818892658, 321386456, 3747250, 1239911, 3195676, 1254190, 588375860, -983611064, 677264190, -3181859, 2296099, -3838479, 2642980, -12417, 8380417, 0, 173376332, 676590, 530906624, 2071829, -1029866791, -4018989, -1067647297, -893898890, 509377762, -819295484, -4166425, -3488383, 1987814, -3197248, 768294260, -22883400, -347191365, -335754661, 2998219, -89301, -1354892, -1310261, 36345249, 643961400, 157142369, -568482643, 141835, 2513018, 613238, -2218467, 8380417, 0, -342333886, -1335936, 830756018, 3241972, 552488273, 2156050, 444930577, 60323094, -832852657, 834980303, 1736313, 235407, -3250154, 3258457, -117552223, 1035301089, 522531086, -209807681, -458740, 4040196, 2039144, -818761, -492511373, -889718424, -481719139, -558360247, -1921994, -3472069, -1879878, -2178965, 8380417, 0, -827143915, -3227876, 875112161, 3415069, 450833045, 1759347, -660934133, 458160776, -612717067, -577774276, -2579253, 1787943, -2391089, -2254727, -415984810, -608441020, 150224382, 135295244, -1623354, -2374402, 586241, 527981, 539479988, -521163479, -302276083, -702999655, 2105286, -2033807, -1179613, -2743411, 8380417, 0, 439288460, 1714295, -209493775, -817536, -915957677, -3574466, 892316032, -1071872863, -333129378, -605279149, 3482206, -4182915, -1300016, -2362063, -378477722, 638402564, 130156402, -185731180, -1476985, 2491325, 507927, -724804, 510974714, -356997292, -304395785, -470097680, 1994046, -1393159, -1187885, -1834526, 8380417, 0, 628833668, 2453983, 962678241, 3756790, -496048908, -1935799, -337655269, 630730945, 777970524, 159173408, -1317678, 2461387, 3035980, 621164, -777397036, 678549029, -669544140, 192079267, -3033742, 2647994, -2612853, 749577, -86720197, 771248568, 1063046068, -1030830548, -338420, 3009748, 4148469, -4022750, 8380417, 0, 374309300, 1460718, -439978542, -1716988, -1012201926, -3950053, 999753034, -314332144, 749740976, 864652284, 3901472, -1226661, 2925816, 3374250, 1020029345, -413979908, 426738094, 298172236, 3980599, -1615530, 1665318, 1163598, 658309618, 441577800, 519685171, -863376927, 2569011, 1723229, 2028038, -3369273, 8380417, 0, -164673562, -642628, -742437332, -2897314, 818041395, 3192354, 347590090, -711287812, 687588511, -712065019, 1356448, -2775755, 2683270, -2778788, 1023635298, -351195274, 861908357, 139752717, 3994671, -1370517, 3363542, 545376, -3043996, 773976352, 55063046, -197425671, -11879, 3020393, 214880, -770441, 8380417, 0, -918682129, -3585098, 142694469, 556856, 991769559, 3870317, -888589898, 592665232, -167401858, -117660617, -3467665, 2312838, -653275, -459163, 795799901, 130212265, 220412084, 35937555, 3105558, 508145, 860144, 140244, -282732136, -141890356, 879049958, -388001774, -1103344, -553718, 3430436, -1514152, 8380417, 0, 721508096, 2815639, 747568486, 2917338, 475038184, 1853806, 89383150, -84011120, 259126110, -603268097, 348812, -327848, 1011223, -2354215, -559928242, 604333585, -772445769, 749801963, -2185084, 2358373, -3014420, 2926054, 800464680, -561979013, -439933955, -100631253, 3123762, -2193087, -1716814, -392707, 8380417, 0, 585207070, 2283733, 857403734, 3345963, 476219497, 1858416, -978523985, -492577742, -573161516, 447030292, -3818627, -1922253, -2236726, 1744507, -77645096, -1018462631, 486888731, 270210213, -303005, -3974485, 1900052, 1054478, 904878186, -967019376, -200355636, -187430119, 3531229, -3773731, -781875, -731434
};

static const int32_t streamlined_GS_itable_Q1_extended[((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1] = {
0, 0, 915382907, 3572223, -963888510, -3761513, -964937599, -3765607, 820367122, 3201430, 806080660, 3145678, 738955404, 2883726, 820383522, 3201494, 312926867, 1221177, -142848732, -557458, 257592709, 1005239, -964747974, -3764867, -545785280, -2129892, -687336873, -2682288, -907762539, -3542485, 154181397, 601683, 0, 0, -585207070, -2283733, -476219497, -1858416, -857403734, -3345963, -447030292, -1744507, 573161516, 2236726, 492577742, 1922253, 978523985, 3818627, 187430119, 731434, -270210213, -1054478, 200355636, 781875, -486888731, -1900052, 967019376, 3773731, 1018462631, 3974485, -904878186, -3531229, 77645096, 303005, 0, 0, -721508096, -2815639, -475038184, -1853806, -747568486, -2917338, 603268097, 2354215, -259126110, -1011223, 84011120, 327848, -89383150, -348812, 100631253, 392707, -749801963, -2926054, 439933955, 1716814, 772445769, 3014420, 561979013, 2193087, -604333585, -2358373, -800464680, -3123762, 559928242, 2185084, 0, 0, 918682129, 3585098, -991769559, -3870317, -142694469, -556856, 117660617, 459163, 167401858, 653275, -592665232, -2312838, 888589898, 3467665, 388001774, 1514152, -35937555, -140244, -879049958, -3430436, -220412084, -860144, 141890356, 553718, -130212265, -508145, 282732136, 1103344, -795799901, -3105558, 0, 0, 164673562, 642628, -818041395, -3192354, 742437332, 2897314, 712065019, 2778788, -687588511, -2683270, 711287812, 2775755, -347590090, -1356448, 197425671, 770441, -139752717, -545376, -55063046, -214880, -861908357, -3363542, -773976352, -3020393, 351195274, 1370517, 3043996, 11879, -1023635298, -3994671, 0, 0, -374309300, -1460718, 1012201926, 3950053, 439978542, 1716988, -864652284, -3374250, -749740976, -2925816, 314332144, 1226661, -999753034, -3901472, 863376927, 3369273, -298172236, -1163598, -519685171, -2028038, -426738094, -1665318, -441577800, -1723229, 413979908, 1615530, -658309618, -2569011, -1020029345, -3980599, 0, 0, -628833668, -2453983, 496048908, 1935799, -962678241, -3756790, -159173408, -621164, -777970524, -3035980, -630730945, -2461387, 337655269, 1317678, 1030830548, 4022750, -192079267, -749577, -1063046068, -4148469, 669544140, 2612853, -771248568, -3009748, -678549029, -2647994, 86720197, 338420, 777397036, 3033742, 0, 0, -439288460, -1714295, 915957677, 3574466, 209493775, 817536, 605279149, 2362063, 333129378, 1300016, 1071872863, 4182915, -892316032, -3482206, 470097680, 1834526, 185731180, 724804, 304395785, 1187885, -130156402, -507927, 356997292, 1393159, -638402564, -2491325, -510974714, -1994046, 378477722, 1476985, 0, 0, 827143915, 3227876, -450833045, -1759347, -875112161, -3415069, 577774276, 2254727, 612717067, 2391089, -458160776, -1787943, 660934133, 2579253, 702999655, 2743411, -135295244, -527981, 302276083, 1179613, -150224382, -586241, 521163479, 2033807, 608441020, 2374402, -539479988, -2105286, 415984810, 1623354, 0, 0, 342333886, 1335936, -552488273, -2156050, -830756018, -3241972, -834980303, -3258457, 832852657, 3250154, -60323094, -235407, -444930577, -1736313, 558360247, 2178965, 209807681, 818761, 481719139, 1879878, -522531086, -2039144, 889718424, 3472069, -1035301089, -4040196, 492511373, 1921994, 117552223, 458740, 0, 0, -173376332, -676590, 1029866791, 4018989, -530906624, -2071829, 819295484, 3197248, -509377762, -1987814, 893898890, 3488383, 1067647297, 4166425, 568482643, 2218467, 335754661, 1310261, -157142369, -613238, 347191365, 1354892, -643961400, -2513018, 22883400, 89301, -36345249, -141835, -768294260, -2998219, 0, 0, 111244624, 434125, 898510625, 3506380, -280713909, -1095468, 854436357, 3334383, -631001801, -2462444, -43482586, -169688, 144935890, 565603, 3181859, 12417, -321386456, -1254190, -677264190, -2642980, -818892658, -3195676, 983611064, 3838479, -317727459, -1239911, -588375860, -2296099, -960233614, -3747250, 0, 0, 903139016, 3524442, -237992130, -928749, -101000509, -394148, 759080783, 2962264, -294395108, -1148858, -123678909, -482649, -391567239, -1528066, 814992530, 3180456, 68791907, 268456, 925511710, 3611750, -611800717, -2387513, 442566669, 1727088, -561940831, -2192938, 454226054, 1772588, 1062481036, 4146264, 0, 0, 429120452, 1674615, -297218217, -1159875, -949361686, -3704823, -1065510939, -4158088, 284313712, 1109516, 764594519, 2983781, -720393920, -2811291, 629190881, 2455377, 64176841, 250446, -162963861, -635956, 909946047, 3551006, 965793731, 3768948, -686309310, -2678278, 873958779, 3410568, 431820817, 1685153, 0, 0, -682491182, -2663378, -538486762, -2101410, 797147778, 3110818, 977780347, 3815725, -496502727, -1937570, -519705671, -2028118, -642926661, -2508980, 963363710, 3759465, 1013967746, 3956944, -409185979, -1596822, 507246529, 1979497, 628875181, 2454145, -258649997, -1009365, -210776307, -822541, 7126831, 27812, 0, 0, 1041158200, 4063053, 919027554, 3586446, -702264730, -2740543, 70227934, 274060, 799869667, 3121440, 825844983, 3222807, -1071989969, -4183372, 952468207, 3716946, 841760171, 3284915, 588452222, 2296397, 1013916752, 3956745, 1016110510, 3965306, -163212680, -636927, -22347069, -87208, -302950022, -1182243, 0, 0, 863652652, 3370349, -815613168, -3182878, -923069133, -3602218, -987079667, -3852015, 675340520, 2635473, -327391679, -1277625, -787459213, -3073009, -710479343, -2772600, 15156688, 59148, 456183549, 1780227, -681730119, -2660408, 373072124, 1455890, 681503850, 2659525, 495951789, 1935420, -449207, -1753
};

static const int32_t streamlined_GS_itable_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1] = {
0, 0, 915382907, 3572223, -963888510, -3761513, -964937599, -3765607, 820367122, 3201430, 806080660, 3145678, 738955404, 2883726, 820383522, 3201494, 312926867, 1221177, -142848732, -557458, 257592709, 1005239, -964747974, -3764867, -545785280, -2129892, -687336873, -2682288, -907762539, -3542485, 154181397, 601683, 8380417, 0, -585207070, -2283733, -476219497, -1858416, -857403734, -3345963, -447030292, 573161516, 492577742, 978523985, -1744507, 2236726, 1922253, 3818627, 187430119, 200355636, 967019376, -904878186, 731434, 781875, 3773731, -3531229, -270210213, -486888731, 1018462631, 77645096, -1054478, -1900052, 3974485, 303005, 8380417, 0, -721508096, -2815639, -475038184, -1853806, -747568486, -2917338, 603268097, -259126110, 84011120, -89383150, 2354215, -1011223, 327848, -348812, 100631253, 439933955, 561979013, -800464680, 392707, 1716814, 2193087, -3123762, -749801963, 772445769, -604333585, 559928242, -2926054, 3014420, -2358373, 2185084, 8380417, 0, 918682129, 3585098, -991769559, -3870317, -142694469, -556856, 117660617, 167401858, -592665232, 888589898, 459163, 653275, -2312838, 3467665, 388001774, -879049958, 141890356, 282732136, 1514152, -3430436, 553718, 1103344, -35937555, -220412084, -130212265, -795799901, -140244, -860144, -508145, -3105558, 8380417, 0, 164673562, 642628, -818041395, -3192354, 742437332, 2897314, 712065019, -687588511, 711287812, -347590090, 2778788, -2683270, 2775755, -1356448, 197425671, -55063046, -773976352, 3043996, 770441, -214880, -3020393, 11879, -139752717, -861908357, 351195274, -1023635298, -545376, -3363542, 1370517, -3994671, 8380417, 0, -374309300, -1460718, 1012201926, 3950053, 439978542, 1716988, -864652284, -749740976, 314332144, -999753034, -3374250, -2925816, 1226661, -3901472, 863376927, -519685171, -441577800, -658309618, 3369273, -2028038, -1723229, -2569011, -298172236, -426738094, 413979908, -1020029345, -1163598, -1665318, 1615530, -3980599, 8380417, 0, -628833668, -2453983, 496048908, 1935799, -962678241, -3756790, -159173408, -777970524, -630730945, 337655269, -621164, -3035980, -2461387, 1317678, 1030830548, -1063046068, -771248568, 86720197, 4022750, -4148469, -3009748, 338420, -192079267, 669544140, -678549029, 777397036, -749577, 2612853, -2647994, 3033742, 8380417, 0, -439288460, -1714295, 915957677, 3574466, 209493775, 817536, 605279149, 333129378, 1071872863, -892316032, 2362063, 1300016, 4182915, -3482206, 470097680, 304395785, 356997292, -510974714, 1834526, 1187885, 1393159, -1994046, 185731180, -130156402, -638402564, 378477722, 724804, -507927, -2491325, 1476985, 8380417, 0, 827143915, 3227876, -450833045, -1759347, -875112161, -3415069, 577774276, 612717067, -458160776, 660934133, 2254727, 2391089, -1787943, 2579253, 702999655, 302276083, 521163479, -539479988, 2743411, 1179613, 2033807, -2105286, -135295244, -150224382, 608441020, 415984810, -527981, -586241, 2374402, 1623354, 8380417, 0, 342333886, 1335936, -552488273, -2156050, -830756018, -3241972, -834980303, 832852657, -60323094, -444930577, -3258457, 3250154, -235407, -1736313, 558360247, 481719139, 889718424, 492511373, 2178965, 1879878, 3472069, 1921994, 209807681, -522531086, -1035301089, 117552223, 818761, -2039144, -4040196, 458740, 8380417, 0, -173376332, -676590, 1029866791, 4018989, -530906624, -2071829, 819295484, -509377762, 893898890, 1067647297, 3197248, -1987814, 3488383, 4166425, 568482643, -157142369, -643961400, -36345249, 2218467, -613238, -2513018, -141835, 335754661, 347191365, 22883400, -768294260, 1310261, 1354892, 89301, -2998219, 8380417, 0, 111244624, 434125, 898510625, 3506380, -280713909, -1095468, 854436357, -631001801, -43482586, 144935890, 3334383, -2462444, -169688, 565603, 3181859, -677264190, 983611064, -588375860, 12417, -2642980, 3838479, -2296099, -321386456, -818892658, -317727459, -960233614, -1254190, -3195676, -1239911, -3747250, 8380417, 0, 903139016, 3524442, -237992130, -928749, -101000509, -394148, 759080783, -294395108, -123678909, -391567239, 2962264, -1148858, -482649, -1528066, 814992530, 925511710, 442566669, 454226054, 3180456, 3611750, 1727088, 1772588, 68791907, -611800717, -561940831, 1062481036, 268456, -2387513, -2192938, 4146264, 8380417, 0, 429120452, 1674615, -297218217, -1159875, -949361686, -3704823, -1065510939, 284313712, 764594519, -720393920, -4158088, 1109516, 2983781, -2811291, 629190881, -162963861, 965793731, 873958779, 2455377, -635956, 3768948, 3410568, 64176841, 909946047, -686309310, 431820817, 250446, 3551006, -2678278, 1685153, 8380417, 0, -682491182, -2663378, -538486762, -2101410, 797147778, 3110818, 977780347, -496502727, -519705671, -642926661, 3815725, -1937570, -2028118, -2508980, 963363710, -409185979, 628875181, -210776307, 3759465, -1596822, 2454145, -822541, 1013967746, 507246529, -258649997, 7126831, 3956944, 1979497, -1009365, 27812, 8380417, 0, 1041158200, 4063053, 919027554, 3586446, -702264730, -2740543, 70227934, 799869667, 825844983, -1071989969, 274060, 3121440, 3222807, -4183372, 952468207, 588452222, 1016110510, -22347069, 3716946, 2296397, 3965306, -87208, 841760171, 1013916752, -163212680, -302950022, 3284915, 3956745, -636927, -1182243, 8380417, 0, 863652652, 3370349, -815613168, -3182878, -923069133, -3602218, -987079667, 675340520, -327391679, -787459213, -3852015, 2635473, -1277625, -3073009, -710479343, 456183549, 373072124, 495951789, -2772600, 1780227, 1455890, 1935420, 15156688, -681730119, 681503850, -449207, 59148, -2660408, 2659525, -1753
};

void extend_table_int32(int32_t *table_extended, int32_t *table, size_t len, int32_t *Qprime, int32_t *Q);

void permute32(int32_t table32[32]);

int main(){

    int32_t buff[BUFF_MAX];
    int32_t buff_extended[BUFF_MAX];

    int32_t scale_int32;
    int32_t omega_int32;
    int32_t mod_int32;
    int32_t aux_int32;


    profile.compressed_layers = 2;
    profile.merged_layers[0] = 4;
    profile.merged_layers[1] = 4;

// ================

    scale_int32 = RmodQ1;
    omega_int32 = omegaQ1;
    mod_int32 = Q1;
    gen_streamlined_CT_negacyclic_table_generic(
        buff,
        &scale_int32, &omega_int32,
        &mod_int32,
        sizeof(int32_t),
        mulmod_int32,
        &profile, 1
    );

    aux_int32 = Q1prime;
    mod_int32 = Q1;
    extend_table_int32(
        buff_extended,
        buff,
        (NTT_N - 1) + (1 << 0) + (1 << 4),
        &aux_int32, &mod_int32
    );

    for(size_t i = 0; i < (((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1); i++){
        assert(streamlined_CT_negacyclic_table_Q1_extended[i] == buff_extended[i]);
    }

    for(size_t i = 32; i < (((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1); i += 32){
        permute32(buff_extended + i);
    }

    for(size_t i = 0; i < (((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1); i++){
        assert(streamlined_CT_negacyclic_table_Q1_jump_extended[i] == buff_extended[i]);
    }

// ================

// ================

    scale_int32 = RmodQ1;
    omega_int32 = invomegaQ1;
    mod_int32 = Q1;
    gen_streamlined_CT_negacyclic_table_generic(
        buff,
        &scale_int32, &omega_int32,
        &mod_int32,
        sizeof(int32_t),
        mulmod_int32,
        &profile, 1
    );

    aux_int32 = Q1prime;
    mod_int32 = Q1;
    extend_table_int32(
        buff_extended,
        buff,
        (NTT_N - 1) + (1 << 0) + (1 << 4),
        &aux_int32, &mod_int32
    );

    for(size_t i = 0; i < (((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1); i++){
        assert(streamlined_GS_itable_Q1_extended[i] == buff_extended[i]);
    }

    for(size_t i = 32; i < (((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1); i += 32){
        permute32(buff_extended + i);
    }

    for(size_t i = 0; i < (((NTT_N - 1) + (1 << 0) + (1 << 4)) << 1); i++){
        assert(streamlined_GS_itable_Q1_jump_extended[i] == buff_extended[i]);
    }

// ================

    printf("Well done!\n");

}

void extend_table_int32(int32_t *table_extended, int32_t *table, size_t len, int32_t *Qprime, int32_t *Q){

    int32_t table_v;
    int32_t B;
    int32_t Bprime;
    int32_t Bprime_half;
    int32_t Bdouble_prime;
    int32_t mod;
    int32_t aux;

    mod = *Q;
    aux = *Qprime;

    for(size_t i = 0; i < len; i++){
        table_v = -table[i];
        if( (table_v & 1) && (table_v > 0)){
            table_v -= mod;
    }
        if( (table_v & 1) && (table_v < 0)){
            table_v += mod;
    }
    B = table_v;
    // note that we reduce mod^{\pm} 2^32 by typing
    Bprime = B * aux;
    Bprime_half = Bprime >> 1;
    Bdouble_prime =
        ( ((int64_t)Bprime) * ((int64_t)mod) - B ) >> 32;
        table_extended[2 * i + 0] = Bprime_half;
        table_extended[2 * i + 1] = Bdouble_prime;
    }

}

void permute32(int32_t table32[32]){

    int32_t tmp[32];

    tmp[ 8] = table32[ 8];
    tmp[12] = table32[ 9];
    tmp[ 9] = table32[10];
    tmp[13] = table32[11];
    tmp[10] = table32[12];
    tmp[14] = table32[13];
    tmp[11] = table32[14];
    tmp[15] = table32[15];

    tmp[16] = table32[16];
    tmp[20] = table32[17];
    tmp[24] = table32[18];
    tmp[28] = table32[19];

    tmp[17] = table32[20];
    tmp[21] = table32[21];
    tmp[25] = table32[22];
    tmp[29] = table32[23];

    tmp[18] = table32[24];
    tmp[22] = table32[25];
    tmp[26] = table32[26];
    tmp[30] = table32[27];

    tmp[19] = table32[28];
    tmp[23] = table32[29];
    tmp[27] = table32[30];
    tmp[31] = table32[31];

    for(size_t i = 8; i < 32; i++){
        table32[i] = tmp[i];
    }

    table32[0] = Q1;

}
