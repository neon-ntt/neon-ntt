/* Based on the public domain implemntation in
 * crypto_stream/chacha20/e/ref from http://bench.cr.yp.to/supercop.html
 * by Daniel J. Bernstein */

#include <stdint.h>
#include "chacha20.h"

#include <stddef.h>
#include <arm_neon.h>

#define ROUNDS 20

uint8x16_t __attribute__((aligned (16)))rotl8_tbl = {
    0x3, 0x0, 0x1, 0x2,
    0x7, 0x4, 0x5, 0x6,
    0xb, 0x8, 0x9, 0xa,
    0xf, 0xc, 0xd, 0xe
};

typedef uint32_t uint32;

static inline uint32 load_littleendian(const unsigned char *x)
{
  return *((uint32_t*)x);
}

static inline void store_littleendian(unsigned char *x,uint32 u)
{
  *((uint32_t*)x) = u;
}

static inline uint32 rotate(uint32 a, int d)
{
  return (a << d) | (a >> (32 - d));
}

// static inline uint32x4_t rotatex4(uint32x4_t a, int d){
//     return vsraq_n_u32(vshlq_n_u32(a, d), a, 32 - d);
// }

static void quarterround(uint32_t *ptr_a, uint32_t *ptr_b, uint32_t *ptr_c, uint32_t *ptr_d)
{
  uint32_t a, b, c, d;
  a = *ptr_a;
  b = *ptr_b;
  c = *ptr_c;
  d = *ptr_d;

  a = a + b;
  d = d ^ a;
  d = rotate(d, 16);

  c = c + d;
  b = b ^ c;
  b = rotate(b, 12);

  a = a + b;
  d = d ^ a;
  d = rotate(d, 8);

  c = c + d;
  b = b ^ c;
  b = rotate(b, 7);

  *ptr_a = a;
  *ptr_b = b;
  *ptr_c = c;
  *ptr_d = d;

}

static void quarterroundx4(uint32x4_t *ptr_a, uint32x4_t *ptr_b, uint32x4_t *ptr_c, uint32x4_t *ptr_d){

  uint32x4_t a, b, c, d;
  a = *ptr_a;
  b = *ptr_b;
  c = *ptr_c;
  d = *ptr_d;

  a = a + b;
  d = d ^ a;
  d = (uint32x4_t)vrev32q_u16((uint16x8_t)d);

  c = c + d;
  b = b ^ c;
  b = vsraq_n_u32(vshlq_n_u32(b, 12), b, 20);

  a = a + b;
  d = d ^ a;
  d = (uint32x4_t)vqtbl1q_u8((uint8x16_t)d, rotl8_tbl);

  c = c + d;
  b = b ^ c;
  b = vsraq_n_u32(vshlq_n_u32(b, 7), b, 25);

  *ptr_a = a;
  *ptr_b = b;
  *ptr_c = c;
  *ptr_d = d;
}

static int crypto_core_chacha20(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
)
{
  uint32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
  uint32 j0, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14, j15;
  int i;

    j0  = x0  = load_littleendian( c +  0);
    j1  = x1  = load_littleendian( c +  4);
    j2  = x2  = load_littleendian( c +  8);
    j3  = x3  = load_littleendian( c + 12);
    j4  = x4  = load_littleendian( k +  0);
    j5  = x5  = load_littleendian( k +  4);
    j6  = x6  = load_littleendian( k +  8);
    j7  = x7  = load_littleendian( k + 12);
    j8  = x8  = load_littleendian( k + 16);
    j9  = x9  = load_littleendian( k + 20);
    j10 = x10 = load_littleendian( k + 24);
    j11 = x11 = load_littleendian( k + 28);
    j12 = x12 = load_littleendian(in +  8);
    j13 = x13 = load_littleendian(in + 12);
    j14 = x14 = load_littleendian(in +  0);
    j15 = x15 = load_littleendian(in +  4);

  for (i = ROUNDS;i > 0;i -= 2) {
    quarterround(&x0, &x4, &x8,&x12);
    quarterround(&x1, &x5, &x9,&x13);
    quarterround(&x2, &x6,&x10,&x14);
    quarterround(&x3, &x7,&x11,&x15);
    quarterround(&x0, &x5,&x10,&x15);
    quarterround(&x1, &x6,&x11,&x12);
    quarterround(&x2, &x7, &x8,&x13);
    quarterround(&x3, &x4, &x9,&x14);
  }

  x0 += j0;
  x1 += j1;
  x2 += j2;
  x3 += j3;
  x4 += j4;
  x5 += j5;
  x6 += j6;
  x7 += j7;
  x8 += j8;
  x9 += j9;
  x10 += j10;
  x11 += j11;
  x12 += j12;
  x13 += j13;
  x14 += j14;
  x15 += j15;

  store_littleendian(out + 0,x0);
  store_littleendian(out + 4,x1);
  store_littleendian(out + 8,x2);
  store_littleendian(out + 12,x3);
  store_littleendian(out + 16,x4);
  store_littleendian(out + 20,x5);
  store_littleendian(out + 24,x6);
  store_littleendian(out + 28,x7);
  store_littleendian(out + 32,x8);
  store_littleendian(out + 36,x9);
  store_littleendian(out + 40,x10);
  store_littleendian(out + 44,x11);
  store_littleendian(out + 48,x12);
  store_littleendian(out + 52,x13);
  store_littleendian(out + 56,x14);
  store_littleendian(out + 60,x15);

  return 0;
}

static int crypto_core_chacha20x4(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
){

    uint32x4_t  x0x4,  x1x4,  x2x4,  x3x4,  x4x4,  x5x4,  x6x4,  x7x4,  x8x4,  x9x4, x10x4, x11x4, x12x4, x13x4, x14x4, x15x4;
    uint32x4_t j12x4, j13x4, j14x4, j15x4;
    uint32x4x4_t last;

     x0x4 = vdupq_n_u32(*(uint32_t*)(c +  0));
     x1x4 = vdupq_n_u32(*(uint32_t*)(c +  4));
     x2x4 = vdupq_n_u32(*(uint32_t*)(c +  8));
     x3x4 = vdupq_n_u32(*(uint32_t*)(c + 12));
     x4x4 = vdupq_n_u32(*(uint32_t*)(k +  0));
     x5x4 = vdupq_n_u32(*(uint32_t*)(k +  4));
     x6x4 = vdupq_n_u32(*(uint32_t*)(k +  8));
     x7x4 = vdupq_n_u32(*(uint32_t*)(k + 12));
     x8x4 = vdupq_n_u32(*(uint32_t*)(k + 16));
     x9x4 = vdupq_n_u32(*(uint32_t*)(k + 20));
    x10x4 = vdupq_n_u32(*(uint32_t*)(k + 24));
    x11x4 = vdupq_n_u32(*(uint32_t*)(k + 28));
    last = vld4q_u32((uint32_t*)in);
    j14x4 = x14x4 = last.val[0];
    j15x4 = x15x4 = last.val[1];
    j12x4 = x12x4 = last.val[2];
    j13x4 = x13x4 = last.val[3];

    for (size_t i = ROUNDS; i > 0; i -= 2) {
        quarterroundx4(&x0x4, &x4x4,  &x8x4, &x12x4);
        quarterroundx4(&x1x4, &x5x4,  &x9x4, &x13x4);
        quarterroundx4(&x2x4, &x6x4, &x10x4, &x14x4);
        quarterroundx4(&x3x4, &x7x4, &x11x4, &x15x4);
        quarterroundx4(&x0x4, &x5x4, &x10x4, &x15x4);
        quarterroundx4(&x1x4, &x6x4, &x11x4, &x12x4);
        quarterroundx4(&x2x4, &x7x4,  &x8x4, &x13x4);
        quarterroundx4(&x3x4, &x4x4,  &x9x4, &x14x4);
    }

     x0x4 += vdupq_n_u32(*(uint32_t*)(c +  0));
     x1x4 += vdupq_n_u32(*(uint32_t*)(c +  4));
     x2x4 += vdupq_n_u32(*(uint32_t*)(c +  8));
     x3x4 += vdupq_n_u32(*(uint32_t*)(c + 12));
     x4x4 += vdupq_n_u32(*(uint32_t*)(k +  0));
     x5x4 += vdupq_n_u32(*(uint32_t*)(k +  4));
     x6x4 += vdupq_n_u32(*(uint32_t*)(k +  8));
     x7x4 += vdupq_n_u32(*(uint32_t*)(k + 12));
     x8x4 += vdupq_n_u32(*(uint32_t*)(k + 16));
     x9x4 += vdupq_n_u32(*(uint32_t*)(k + 20));
    x10x4 += vdupq_n_u32(*(uint32_t*)(k + 24));
    x11x4 += vdupq_n_u32(*(uint32_t*)(k + 28));

    x12x4 += j12x4;
    x13x4 += j13x4;
    x14x4 += j14x4;
    x15x4 += j15x4;

    for(size_t i = 0; i < 4; i++){

        store_littleendian(out +  0 + i * 64, x0x4[i]);
        store_littleendian(out +  4 + i * 64, x1x4[i]);
        store_littleendian(out +  8 + i * 64, x2x4[i]);
        store_littleendian(out + 12 + i * 64, x3x4[i]);
        store_littleendian(out + 16 + i * 64, x4x4[i]);
        store_littleendian(out + 20 + i * 64, x5x4[i]);
        store_littleendian(out + 24 + i * 64, x6x4[i]);
        store_littleendian(out + 28 + i * 64, x7x4[i]);
        store_littleendian(out + 32 + i * 64, x8x4[i]);
        store_littleendian(out + 36 + i * 64, x9x4[i]);
        store_littleendian(out + 40 + i * 64,x10x4[i]);
        store_littleendian(out + 44 + i * 64,x11x4[i]);
        store_littleendian(out + 48 + i * 64,x12x4[i]);
        store_littleendian(out + 52 + i * 64,x13x4[i]);
        store_littleendian(out + 56 + i * 64,x14x4[i]);
        store_littleendian(out + 60 + i * 64,x15x4[i]);

    }

  return 0;

}

static int crypto_core_chacha20x5(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
){

    uint32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
    uint32 j0, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14, j15;
    uint32x4_t  x0x4,  x1x4,  x2x4,  x3x4,  x4x4,  x5x4,  x6x4,  x7x4,  x8x4,  x9x4, x10x4, x11x4, x12x4, x13x4, x14x4, x15x4;
    uint32x4_t j12x4, j13x4, j14x4, j15x4;
    uint32x4x4_t last;

    j0  = x0  = load_littleendian( c +  0);
    j1  = x1  = load_littleendian( c +  4);
    j2  = x2  = load_littleendian( c +  8);
    j3  = x3  = load_littleendian( c + 12);
    j4  = x4  = load_littleendian( k +  0);
    j5  = x5  = load_littleendian( k +  4);
    j6  = x6  = load_littleendian( k +  8);
    j7  = x7  = load_littleendian( k + 12);
    j8  = x8  = load_littleendian( k + 16);
    j9  = x9  = load_littleendian( k + 20);
    j10 = x10 = load_littleendian( k + 24);
    j11 = x11 = load_littleendian( k + 28);
    j12 = x12 = load_littleendian(in +  8 + 4 * 16);
    j13 = x13 = load_littleendian(in + 12 + 4 * 16);
    j14 = x14 = load_littleendian(in +  0);
    j15 = x15 = load_littleendian(in +  4);


     x0x4 = vdupq_n_u32(x0);
     x1x4 = vdupq_n_u32(x1);
     x2x4 = vdupq_n_u32(x2);
     x3x4 = vdupq_n_u32(x3);
     x4x4 = vdupq_n_u32(x4);
     x5x4 = vdupq_n_u32(x5);
     x6x4 = vdupq_n_u32(x6);
     x7x4 = vdupq_n_u32(x7);
     x8x4 = vdupq_n_u32(x8);
     x9x4 = vdupq_n_u32(x9);
    x10x4 = vdupq_n_u32(x10);
    x11x4 = vdupq_n_u32(x11);
    last = vld4q_u32((uint32_t*)in);
    j14x4 = x14x4 = last.val[0];
    j15x4 = x15x4 = last.val[1];
    j12x4 = x12x4 = last.val[2];
    j13x4 = x13x4 = last.val[3];

    for (size_t i = ROUNDS; i > 0; i -= 2) {
        quarterround(&x0, &x4, &x8,&x12);
        quarterroundx4(&x0x4, &x4x4,  &x8x4, &x12x4);
        quarterround(&x1, &x5, &x9,&x13);
        quarterroundx4(&x1x4, &x5x4,  &x9x4, &x13x4);
        quarterround(&x2, &x6,&x10,&x14);
        quarterroundx4(&x2x4, &x6x4, &x10x4, &x14x4);
        quarterround(&x3, &x7,&x11,&x15);
        quarterroundx4(&x3x4, &x7x4, &x11x4, &x15x4);
        quarterround(&x0, &x5,&x10,&x15);
        quarterroundx4(&x0x4, &x5x4, &x10x4, &x15x4);
        quarterround(&x1, &x6,&x11,&x12);
        quarterroundx4(&x1x4, &x6x4, &x11x4, &x12x4);
        quarterround(&x2, &x7, &x8,&x13);
        quarterroundx4(&x2x4, &x7x4,  &x8x4, &x13x4);
        quarterround(&x3, &x4, &x9,&x14);
        quarterroundx4(&x3x4, &x4x4,  &x9x4, &x14x4);
    }

  x0 += j0;
  x1 += j1;
  x2 += j2;
  x3 += j3;
  x4 += j4;
  x5 += j5;
  x6 += j6;
  x7 += j7;
  x8 += j8;
  x9 += j9;
  x10 += j10;
  x11 += j11;
  x12 += j12;
  x13 += j13;
  x14 += j14;
  x15 += j15;

     x0x4 += vdupq_n_u32(j0);
     x1x4 += vdupq_n_u32(j1);
     x2x4 += vdupq_n_u32(j2);
     x3x4 += vdupq_n_u32(j3);
     x4x4 += vdupq_n_u32(j4);
     x5x4 += vdupq_n_u32(j5);
     x6x4 += vdupq_n_u32(j6);
     x7x4 += vdupq_n_u32(j7);
     x8x4 += vdupq_n_u32(j8);
     x9x4 += vdupq_n_u32(j9);
    x10x4 += vdupq_n_u32(j10);
    x11x4 += vdupq_n_u32(j11);

    x12x4 += j12x4;
    x13x4 += j13x4;
    x14x4 += j14x4;
    x15x4 += j15x4;

    for(size_t i = 0; i < 4; i++){

        store_littleendian(out +  0 + i * 64, x0x4[i]);
        store_littleendian(out +  4 + i * 64, x1x4[i]);
        store_littleendian(out +  8 + i * 64, x2x4[i]);
        store_littleendian(out + 12 + i * 64, x3x4[i]);
        store_littleendian(out + 16 + i * 64, x4x4[i]);
        store_littleendian(out + 20 + i * 64, x5x4[i]);
        store_littleendian(out + 24 + i * 64, x6x4[i]);
        store_littleendian(out + 28 + i * 64, x7x4[i]);
        store_littleendian(out + 32 + i * 64, x8x4[i]);
        store_littleendian(out + 36 + i * 64, x9x4[i]);
        store_littleendian(out + 40 + i * 64,x10x4[i]);
        store_littleendian(out + 44 + i * 64,x11x4[i]);
        store_littleendian(out + 48 + i * 64,x12x4[i]);
        store_littleendian(out + 52 + i * 64,x13x4[i]);
        store_littleendian(out + 56 + i * 64,x14x4[i]);
        store_littleendian(out + 60 + i * 64,x15x4[i]);

    }

  store_littleendian(out +  0 + 4 * 64,  x0);
  store_littleendian(out +  4 + 4 * 64,  x1);
  store_littleendian(out +  8 + 4 * 64,  x2);
  store_littleendian(out + 12 + 4 * 64,  x3);
  store_littleendian(out + 16 + 4 * 64,  x4);
  store_littleendian(out + 20 + 4 * 64,  x5);
  store_littleendian(out + 24 + 4 * 64,  x6);
  store_littleendian(out + 28 + 4 * 64,  x7);
  store_littleendian(out + 32 + 4 * 64,  x8);
  store_littleendian(out + 36 + 4 * 64,  x9);
  store_littleendian(out + 40 + 4 * 64, x10);
  store_littleendian(out + 44 + 4 * 64, x11);
  store_littleendian(out + 48 + 4 * 64, x12);
  store_littleendian(out + 52 + 4 * 64, x13);
  store_littleendian(out + 56 + 4 * 64, x14);
  store_littleendian(out + 60 + 4 * 64, x15);

  return 0;

}

static int crypto_core_chacha20x6(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
){

    uint32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
    uint32 j0, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14, j15;
    uint32x4_t  x0x4,  x1x4,  x2x4,  x3x4,  x4x4,  x5x4,  x6x4,  x7x4,  x8x4,  x9x4, x10x4, x11x4, x12x4, x13x4, x14x4, x15x4;
    uint32x4_t j12x4, j13x4, j14x4, j15x4;
    uint32x4x4_t last;

//

    j0  = x0  = load_littleendian( c +  0);
    j1  = x1  = load_littleendian( c +  4);
    j2  = x2  = load_littleendian( c +  8);
    j3  = x3  = load_littleendian( c + 12);
    j4  = x4  = load_littleendian( k +  0);
    j5  = x5  = load_littleendian( k +  4);
    j6  = x6  = load_littleendian( k +  8);
    j7  = x7  = load_littleendian( k + 12);
    j8  = x8  = load_littleendian( k + 16);
    j9  = x9  = load_littleendian( k + 20);
    j10 = x10 = load_littleendian( k + 24);
    j11 = x11 = load_littleendian( k + 28);
    j12 = x12 = load_littleendian(in +  8 + 4 * 16);
    j13 = x13 = load_littleendian(in + 12 + 4 * 16);
    j14 = x14 = load_littleendian(in +  0);
    j15 = x15 = load_littleendian(in +  4);

     x0x4 = vdupq_n_u32(j0);
     x1x4 = vdupq_n_u32(j1);
     x2x4 = vdupq_n_u32(j2);
     x3x4 = vdupq_n_u32(j3);
     x4x4 = vdupq_n_u32(j4);
     x5x4 = vdupq_n_u32(j5);
     x6x4 = vdupq_n_u32(j6);
     x7x4 = vdupq_n_u32(j7);
     x8x4 = vdupq_n_u32(j8);
     x9x4 = vdupq_n_u32(j9);
    x10x4 = vdupq_n_u32(j10);
    x11x4 = vdupq_n_u32(j11);
    last = vld4q_u32((uint32_t*)in);
    j14x4 = x14x4 = last.val[0];
    j15x4 = x15x4 = last.val[1];
    j12x4 = x12x4 = last.val[2];
    j13x4 = x13x4 = last.val[3];

    for (size_t i = ROUNDS / 2; i > 0; i -= 2) {
        quarterroundx4(&x0x4, &x4x4,  &x8x4, &x12x4);
        quarterround(&x0, &x4, &x8,&x12);
        quarterround(&x1, &x5, &x9,&x13);
        quarterroundx4(&x1x4, &x5x4,  &x9x4, &x13x4);
        quarterround(&x2, &x6,&x10,&x14);
        quarterround(&x3, &x7,&x11,&x15);
        quarterroundx4(&x2x4, &x6x4, &x10x4, &x14x4);
        quarterround(&x0, &x5,&x10,&x15);
        quarterround(&x1, &x6,&x11,&x12);
        quarterroundx4(&x3x4, &x7x4, &x11x4, &x15x4);
        quarterround(&x2, &x7, &x8,&x13);
        quarterround(&x3, &x4, &x9,&x14);
        quarterroundx4(&x0x4, &x5x4, &x10x4, &x15x4);
        quarterround(&x0, &x4, &x8,&x12);
        quarterround(&x1, &x5, &x9,&x13);
        quarterroundx4(&x1x4, &x6x4, &x11x4, &x12x4);
        quarterround(&x2, &x6,&x10,&x14);
        quarterround(&x3, &x7,&x11,&x15);
        quarterroundx4(&x2x4, &x7x4,  &x8x4, &x13x4);
        quarterround(&x0, &x5,&x10,&x15);
        quarterround(&x1, &x6,&x11,&x12);
        quarterroundx4(&x3x4, &x4x4,  &x9x4, &x14x4);
        quarterround(&x2, &x7, &x8,&x13);
        quarterround(&x3, &x4, &x9,&x14);
    }

    x0 += j0;
    x1 += j1;
    x2 += j2;
    x3 += j3;
    x4 += j4;
    x5 += j5;
    x6 += j6;
    x7 += j7;
    x8 += j8;
    x9 += j9;
    x10 += j10;
    x11 += j11;
    x12 += j12;
    x13 += j13;
    x14 += j14;
    x15 += j15;

    store_littleendian(out +  0 + 4 * 64,  x0);
    store_littleendian(out +  4 + 4 * 64,  x1);
    store_littleendian(out +  8 + 4 * 64,  x2);
    store_littleendian(out + 12 + 4 * 64,  x3);
    store_littleendian(out + 16 + 4 * 64,  x4);
    store_littleendian(out + 20 + 4 * 64,  x5);
    store_littleendian(out + 24 + 4 * 64,  x6);
    store_littleendian(out + 28 + 4 * 64,  x7);
    store_littleendian(out + 32 + 4 * 64,  x8);
    store_littleendian(out + 36 + 4 * 64,  x9);
    store_littleendian(out + 40 + 4 * 64, x10);
    store_littleendian(out + 44 + 4 * 64, x11);
    store_littleendian(out + 48 + 4 * 64, x12);
    store_littleendian(out + 52 + 4 * 64, x13);
    store_littleendian(out + 56 + 4 * 64, x14);
    store_littleendian(out + 60 + 4 * 64, x15);

//

    j0  = x0  = load_littleendian( c +  0);
    j1  = x1  = load_littleendian( c +  4);
    j2  = x2  = load_littleendian( c +  8);
    j3  = x3  = load_littleendian( c + 12);
    j4  = x4  = load_littleendian( k +  0);
    j5  = x5  = load_littleendian( k +  4);
    j6  = x6  = load_littleendian( k +  8);
    j7  = x7  = load_littleendian( k + 12);
    j8  = x8  = load_littleendian( k + 16);
    j9  = x9  = load_littleendian( k + 20);
    j10 = x10 = load_littleendian( k + 24);
    j11 = x11 = load_littleendian( k + 28);
    j12 = x12 = load_littleendian(in +  8 + 5 * 16);
    j13 = x13 = load_littleendian(in + 12 + 5 * 16);
    j14 = x14 = load_littleendian(in +  0);
    j15 = x15 = load_littleendian(in +  4);

    for (size_t i = ROUNDS / 2; i > 0; i -= 2) {
        quarterroundx4(&x0x4, &x4x4,  &x8x4, &x12x4);
        quarterround(&x0, &x4, &x8,&x12);
        quarterround(&x1, &x5, &x9,&x13);
        quarterroundx4(&x1x4, &x5x4,  &x9x4, &x13x4);
        quarterround(&x2, &x6,&x10,&x14);
        quarterround(&x3, &x7,&x11,&x15);
        quarterroundx4(&x2x4, &x6x4, &x10x4, &x14x4);
        quarterround(&x0, &x5,&x10,&x15);
        quarterround(&x1, &x6,&x11,&x12);
        quarterroundx4(&x3x4, &x7x4, &x11x4, &x15x4);
        quarterround(&x2, &x7, &x8,&x13);
        quarterround(&x3, &x4, &x9,&x14);
        quarterroundx4(&x0x4, &x5x4, &x10x4, &x15x4);
        quarterround(&x0, &x4, &x8,&x12);
        quarterround(&x1, &x5, &x9,&x13);
        quarterroundx4(&x1x4, &x6x4, &x11x4, &x12x4);
        quarterround(&x2, &x6,&x10,&x14);
        quarterround(&x3, &x7,&x11,&x15);
        quarterroundx4(&x2x4, &x7x4,  &x8x4, &x13x4);
        quarterround(&x0, &x5,&x10,&x15);
        quarterround(&x1, &x6,&x11,&x12);
        quarterroundx4(&x3x4, &x4x4,  &x9x4, &x14x4);
        quarterround(&x2, &x7, &x8,&x13);
        quarterround(&x3, &x4, &x9,&x14);
    }

    x0 += j0;
    x1 += j1;
    x2 += j2;
    x3 += j3;
    x4 += j4;
    x5 += j5;
    x6 += j6;
    x7 += j7;
    x8 += j8;
    x9 += j9;
    x10 += j10;
    x11 += j11;
    x12 += j12;
    x13 += j13;
    x14 += j14;
    x15 += j15;

    store_littleendian(out +  0 + 5 * 64,  x0);
    store_littleendian(out +  4 + 5 * 64,  x1);
    store_littleendian(out +  8 + 5 * 64,  x2);
    store_littleendian(out + 12 + 5 * 64,  x3);
    store_littleendian(out + 16 + 5 * 64,  x4);
    store_littleendian(out + 20 + 5 * 64,  x5);
    store_littleendian(out + 24 + 5 * 64,  x6);
    store_littleendian(out + 28 + 5 * 64,  x7);
    store_littleendian(out + 32 + 5 * 64,  x8);
    store_littleendian(out + 36 + 5 * 64,  x9);
    store_littleendian(out + 40 + 5 * 64, x10);
    store_littleendian(out + 44 + 5 * 64, x11);
    store_littleendian(out + 48 + 5 * 64, x12);
    store_littleendian(out + 52 + 5 * 64, x13);
    store_littleendian(out + 56 + 5 * 64, x14);
    store_littleendian(out + 60 + 5 * 64, x15);

     x0x4 += vdupq_n_u32(j0);
     x1x4 += vdupq_n_u32(j1);
     x2x4 += vdupq_n_u32(j2);
     x3x4 += vdupq_n_u32(j3);
     x4x4 += vdupq_n_u32(j4);
     x5x4 += vdupq_n_u32(j5);
     x6x4 += vdupq_n_u32(j6);
     x7x4 += vdupq_n_u32(j7);
     x8x4 += vdupq_n_u32(j8);
     x9x4 += vdupq_n_u32(j9);
    x10x4 += vdupq_n_u32(j10);
    x11x4 += vdupq_n_u32(j11);

    x12x4 += j12x4;
    x13x4 += j13x4;
    x14x4 += j14x4;
    x15x4 += j15x4;

    for(size_t i = 0; i < 4; i++){

        store_littleendian(out +  0 + i * 64, x0x4[i]);
        store_littleendian(out +  4 + i * 64, x1x4[i]);
        store_littleendian(out +  8 + i * 64, x2x4[i]);
        store_littleendian(out + 12 + i * 64, x3x4[i]);
        store_littleendian(out + 16 + i * 64, x4x4[i]);
        store_littleendian(out + 20 + i * 64, x5x4[i]);
        store_littleendian(out + 24 + i * 64, x6x4[i]);
        store_littleendian(out + 28 + i * 64, x7x4[i]);
        store_littleendian(out + 32 + i * 64, x8x4[i]);
        store_littleendian(out + 36 + i * 64, x9x4[i]);
        store_littleendian(out + 40 + i * 64,x10x4[i]);
        store_littleendian(out + 44 + i * 64,x11x4[i]);
        store_littleendian(out + 48 + i * 64,x12x4[i]);
        store_littleendian(out + 52 + i * 64,x13x4[i]);
        store_littleendian(out + 56 + i * 64,x14x4[i]);
        store_littleendian(out + 60 + i * 64,x15x4[i]);

    }



  return 0;

}




static const unsigned char sigma[16] = "expand 32-byte k";

int crypto_stream_chacha20(unsigned char *c,unsigned long long clen, const unsigned char *n, const unsigned char *k)
{
  unsigned char block[64];
  unsigned char kcopy[32];

  uint64_t inx6[6][2];

  if (!clen)
    return 0;

  for (size_t i = 0;i < 32;++i)
    kcopy[i] = k[i];
  for(size_t i = 0; i < 6; i++){
    inx6[i][0] = *(uint64_t*)n;
    inx6[i][1] = i;
  }

  while(clen >= 384){

    crypto_core_chacha20x6(c, (unsigned char*)inx6, kcopy, sigma);

    inx6[0][1] += 6;
    inx6[1][1] += 6;
    inx6[2][1] += 6;
    inx6[3][1] += 6;
    inx6[4][1] += 6;
    inx6[5][1] += 6;

    clen -= 384;
    c += 384;

  }

  while(clen >= 320){

    crypto_core_chacha20x5(c, (unsigned char*)inx6, kcopy, sigma);

    inx6[0][1] += 5;
    inx6[1][1] += 5;
    inx6[2][1] += 5;
    inx6[3][1] += 5;
    inx6[4][1] += 5;

    clen -= 320;
    c += 320;

  }

  while(clen >= 256){

    crypto_core_chacha20x4(c, (unsigned char*)inx6, kcopy, sigma);

    inx6[0][1] += 4;
    inx6[1][1] += 4;
    inx6[2][1] += 4;
    inx6[3][1] += 4;

    clen -= 256;
    c += 256;

  }


  while (clen >= 64) {
    crypto_core_chacha20(c, (unsigned char*)inx6, kcopy, sigma);

    inx6[0][1]++;

    clen -= 64;
    c += 64;
  }

  if (clen) {
    crypto_core_chacha20(block, (unsigned char*)inx6, kcopy, sigma);
    for (size_t i = 0;i < clen;++i)
      c[i] = block[i];
  }
  return 0;
}


