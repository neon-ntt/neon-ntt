
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "hal.h"

#include "params.h"
#include "NTT_params.h"
#include "ntt.h"


#define ITERATIONS 10000
uint64_t t0, t1;
uint64_t times[ITERATIONS];

static int cmp_uint64_t(const void *a, const void *b)
{
    return (int)((*((const uint64_t *)a)) - (*((const uint64_t *)b)));
}

int main()
{
    int16_t A[KYBER_K][KYBER_K][256];
    int16_t s[KYBER_K][256];
    int16_t s_asymmetric[KYBER_K][256];
    int16_t acc[KYBER_K][256];
    int16_t t;


    int poly1[KYBER_K][256];
    int poly2[KYBER_K][256];
    int poly2_asymmetric[KYBER_K][128];
    int polyout[KYBER_K][256];

    for(size_t i = 0; i < 256; i++){
        t = rand() % Q1;
        if(t > Q1 / 2){
            t -= Q1;
        }
        if(t < -Q1 / 2){
            t += Q1;
        }
        poly2[0][i] = poly1[0][i] = t;
    }


    __asm_ntt_SIMD_top((int16_t*)(&poly1[0][0]), streamlined_CT_negacyclic_table_Q1_extended, constants);
    __asm_ntt_SIMD_top_schedule((int16_t*)(&poly2[0][0]), streamlined_CT_negacyclic_table_Q1_extended, constants);

    assert(memcmp(poly1, poly2, 256 * sizeof(int16_t)) == 0);

    printf("test finished!\n");



}


