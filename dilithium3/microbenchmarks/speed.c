
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arm_neon.h>

#include "hal.h"
#include "NTT_params.h"
#include "ntt.h"

#include "params.h"

#define DILITHIUM_L L
#define DILITHIUM_K K
#define ITERATIONS 10000

#define QINV 58728449

extern void __asm_polyvecl_pointwise_acc_montgomery(int *, const int *, const int *, const int *);

static const int32_t l_montgomery_const[4] = {
    Q1, QINV};

uint64_t t0, t1;
uint64_t times[ITERATIONS];

static int cmp_uint64_t(const void *a, const void *b)
{
    return (int)((*((const uint64_t *)a)) - (*((const uint64_t *)b)));
}

int main()
{

    int A[DILITHIUM_K][DILITHIUM_L][ARRAY_N];
    int s[DILITHIUM_L][ARRAY_N];
    int t[DILITHIUM_K][ARRAY_N];

    int poly1[DILITHIUM_L * ARRAY_N];
    int poly2[DILITHIUM_L * ARRAY_N];
    int polyNTTout[ARRAY_N];

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        NTT(poly1);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("NTTs (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        __asm_polyvecl_pointwise_acc_montgomery(polyNTTout, poly1, poly2, l_montgomery_const);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("polyvecl_pointwise_acc_montgomery (dim x base_mul) (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        iNTT(poly1);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("iNTTs (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        for (int j = 0; j < DILITHIUM_L; j++)
        {
            NTT(&(s[j][0]));
        }

        for (int j = 0; j < DILITHIUM_K; j++)
        {
            __asm_polyvecl_pointwise_acc_montgomery(&(t[j][0]), &(A[j][0][0]), &(s[0][0]), l_montgomery_const);
        }

        for (int j = 0; j < DILITHIUM_K; j++)
        {
            iNTT(&(t[j][0]));
        }

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("MatrixVectorMul (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);
}