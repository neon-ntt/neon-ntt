
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arm_neon.h>
#include <stdint.h>

#include "hal.h"
#include "NTT_params.h"
#include "NTT.h"
#include "SABER_params.h"

#define ITERATIONS 10000

uint64_t t0, t1;
uint64_t times[ITERATIONS];

static int cmp_uint64_t(const void *a, const void *b)
{
    return (int)((*((const uint64_t *)a)) - (*((const uint64_t *)b)));
}

int main()
{

    uint32_t A[SABER_L * SABER_L * ARRAY_N];

    uint32_t poly1[SABER_L * ARRAY_N];
    uint32_t poly2[SABER_L * ARRAY_N];
    uint32_t poly2_asymmetric[SABER_L * ARRAY_N];

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

        NTT_heavy(poly2_asymmetric, poly2);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("NTT_heavy (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        __asm_asymmetric_mul(poly1, poly2, poly2_asymmetric, constants);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("asymmetric_muls (dim x base_mul) (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        iNTT(poly1);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("iNTT (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        for (int j = 0; j < SABER_L; j++)
        {
            NTT_heavy(poly2_asymmetric, poly2);
        }

        for (int j = 0; j < SABER_L; j++)
        {
            for (int k = 0; k < SABER_L; k++)
            {
                NTT(A + j * SABER_L * ARRAY_N + k * ARRAY_N);
            }
        }

        for (int j = 0; j < SABER_L; j++)
        {
            __asm_asymmetric_mul(A + j * SABER_L * ARRAY_N, poly2, poly2_asymmetric, constants);
        }

        for (int j = 0; j < SABER_L; j++)
        {
            iNTT(A + j * SABER_L * ARRAY_N);
        }

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("MatrixVectorMul (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        for (int j = 0; j < SABER_L; j++)
        {
            NTT(poly1 + j * ARRAY_N);
        }

        __asm_asymmetric_mul(poly1, poly2, poly2_asymmetric, constants);

        iNTT(poly1);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("InnerProd (Enc) (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        for (int j = 0; j < SABER_L; j++)
        {
            NTT_heavy(poly2, poly2_asymmetric);
        }

        for (int j = 0; j < SABER_L; j++)
        {
            NTT(poly1 + j * ARRAY_N);
        }

        __asm_asymmetric_mul(poly1, poly2, poly2_asymmetric, constants);

        iNTT(poly1);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("InnerProd (Dec) (median of %d measurements): %ld cycles\n", ITERATIONS, times[ITERATIONS >> 1]);
}
