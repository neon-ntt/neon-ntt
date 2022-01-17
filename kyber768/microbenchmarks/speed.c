
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "hal.h"

#include "ntt.h"
#include "params.h"
#include "NTT_params.h"

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

    int poly1[KYBER_K][256];
    int poly2[KYBER_K][256];
    int poly2_asymmetric[KYBER_K][128];
    int polyout[KYBER_K][256];

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        NTT((int16_t *)(&poly1[0][0]));

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("NTT cycles (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();
        NTT((int16_t *)(&poly2[0][0]));
        __asm_point_mul_extended((int16_t *)(&poly2_asymmetric[0][0]), (int16_t *)(&poly2[0][0]), pre_asymmetric_table_Q1_extended, asymmetric_const);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("NTT_heavy (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        __asm_point_mul_extended((int16_t *)(&poly2_asymmetric[0][0]), (int16_t *)(&poly2[0][0]), pre_asymmetric_table_Q1_extended, asymmetric_const);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("point_mul cycles (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);


    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        __asm_asymmetric_mul((int16_t *)(&poly1[0][0]), (int16_t *)(&poly2[0][0]), (int16_t *)(&poly2_asymmetric[0][0]), asymmetric_const, (int16_t *)polyout);

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("asymmetric_mul  (dim x base_mul) cycles (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        iNTT((int16_t *)(&poly1[0][0]));

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("iNTT cycles (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        for (int j = 0; j < KYBER_K; j++)
        {
            NTT(&(s[0][0]));
            __asm_point_mul_extended(&(s_asymmetric[j][0]), &(s[j][0]), pre_asymmetric_table_Q1_extended, asymmetric_const);
        }

        for (int j = 0; j < KYBER_K; j++)
        {
            __asm_asymmetric_mul(&(A[j][0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const, &(acc[j][0]));
        }

        for (int j = 0; j < KYBER_K; j++)
        {
            iNTT(&(acc[j][0]));
        }

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("MatrixVectorMul cycles (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        __asm_asymmetric_mul(&(A[0][0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const, &(acc[0][0]));

        iNTT(&(acc[0][0]));

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("InnerProd (Enc) cycles (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);

    for (int i = 0; i < ITERATIONS; i++)
    {

        t0 = hal_get_time();

        for (int j = 0; j < KYBER_K; j++)
        {
            NTT(&(s[0][0]));
            __asm_point_mul_extended(&(s_asymmetric[j][0]), &(s[j][0]), pre_asymmetric_table_Q1_extended, asymmetric_const);
        }

        __asm_asymmetric_mul(&(A[0][0][0]), &(s[0][0]), &(s_asymmetric[0][0]), asymmetric_const, &(acc[0][0]));

        iNTT(&(acc[0][0]));

        t1 = hal_get_time();

        times[i] = t1 - t0;
    }

    qsort(times, ITERATIONS, sizeof(uint64_t), cmp_uint64_t);
    printf("InnerProd (Dec) cycles (median of %d measurements): %ld\n", ITERATIONS, times[ITERATIONS >> 1]);
}
