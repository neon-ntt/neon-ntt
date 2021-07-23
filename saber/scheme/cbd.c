/*=============================================================================
This file has been adapted from the implementation
(available at, Public Domain https://github.com/KULeuven-COSIC/SABER)
of "Saber: Module-LWR based key exchange, CPA-secure encryption and CCA-secure KEM"
by : Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, and Frederik Vercauteren
Jose Maria Bermudo Mera, Michiel Van Beirendonck, Andrea Basso.
 * Copyright (c) 2020 by Cryptographic Engineering Research Group (CERG)
 * ECE Department, George Mason University
 * Fairfax, VA, U.S.A.
 * Author: Duc Tri Nguyen
=============================================================================*/

#include <arm_neon.h>
#include "cbd.h"

#define vload4(c, ptr) c = vld4q_u8(ptr);
#define vstore4(ptr, c) vst4q_u16(ptr, c);

// c = a & b
#define vand8(c, a, b) c = vandq_u8(a, b);

// c = a >> n
#define vsr8(c, a, n) c = vshrq_n_u8(a, n);

// c = a + b
#define vadd8(c, a, b) c = vaddq_u8(a, b);

// low c = (uint16_t) (a - b)
#define vsubll8(c, a, b) c = vsubl_u8(a, b);

// high c = (uint16_t) (a - b)
#define vsublh8(c, a, b) c = vsubl_high_u8(a, b);

#if SABER_MU == 6
static uint32_t load24_littleendian(const uint8_t x[3])
{
  uint32_t r;
  r  = (uint32_t)x[0];
  r |= (uint32_t)x[1] << 8;
  r |= (uint32_t)x[2] << 16;
  return r;
}

static void cbd3(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES])
{
  unsigned int i,j;
  uint32_t t,d;
  int16_t a,b;

  for(i=0;i<SABER_N/4;i++) {
    t  = load24_littleendian(buf+3*i);
    d  = t & 0x00249249;
    d += (t>>1) & 0x00249249;
    d += (t>>2) & 0x00249249;

    for(j=0;j<4;j++) {
      a = (d >> (6*j+0)) & 0x7;
      b = (d >> (6*j+3)) & 0x7;
      s[4*i+j] = a - b;
    }
  }
}
#endif


#if SABER_MU == 8
static 
void neon_cbd4(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES])
{
    uint8x16x4_t neon_buf, res, tmp, a, b;  // 20
    uint16x8x4_t store1, store2;            // 8

    uint8x16_t const_0x11, const_0xf;       // 2
    const_0x11 = vdupq_n_u8(0x11);
    const_0xf = vdupq_n_u8(0xf);

    // Total SIMD register: 30
    for (int i = 0; i < SABER_POLYCOINBYTES; i += 4 * 16)
    {
        // 0, 4, ...
        // 1, 5, ...
        // 2, 6, ...
        // 3, 7, ...
        vload4(neon_buf, &buf[i]);

        // d = t
        vand8(res.val[0], neon_buf.val[0], const_0x11);
        vand8(res.val[1], neon_buf.val[1], const_0x11);
        vand8(res.val[2], neon_buf.val[2], const_0x11);
        vand8(res.val[3], neon_buf.val[3], const_0x11);

        // d += (t >> 1) & 0x11
        vsr8(tmp.val[0], neon_buf.val[0], 1);
        vsr8(tmp.val[1], neon_buf.val[1], 1);
        vsr8(tmp.val[2], neon_buf.val[2], 1);
        vsr8(tmp.val[3], neon_buf.val[3], 1);

        vand8(tmp.val[0], tmp.val[0], const_0x11);
        vand8(tmp.val[1], tmp.val[1], const_0x11);
        vand8(tmp.val[2], tmp.val[2], const_0x11);
        vand8(tmp.val[3], tmp.val[3], const_0x11);

        vadd8(res.val[0], res.val[0], tmp.val[0]);
        vadd8(res.val[1], res.val[1], tmp.val[1]);
        vadd8(res.val[2], res.val[2], tmp.val[2]);
        vadd8(res.val[3], res.val[3], tmp.val[3]);

        // d += (t >> 2) & 0x11
        vsr8(tmp.val[0], neon_buf.val[0], 2);
        vsr8(tmp.val[1], neon_buf.val[1], 2);
        vsr8(tmp.val[2], neon_buf.val[2], 2);
        vsr8(tmp.val[3], neon_buf.val[3], 2);

        vand8(tmp.val[0], tmp.val[0], const_0x11);
        vand8(tmp.val[1], tmp.val[1], const_0x11);
        vand8(tmp.val[2], tmp.val[2], const_0x11);
        vand8(tmp.val[3], tmp.val[3], const_0x11);

        vadd8(res.val[0], res.val[0], tmp.val[0]);
        vadd8(res.val[1], res.val[1], tmp.val[1]);
        vadd8(res.val[2], res.val[2], tmp.val[2]);
        vadd8(res.val[3], res.val[3], tmp.val[3]);

        // d += (t >> 3) & 0x11
        vsr8(tmp.val[0], neon_buf.val[0], 3);
        vsr8(tmp.val[1], neon_buf.val[1], 3);
        vsr8(tmp.val[2], neon_buf.val[2], 3);
        vsr8(tmp.val[3], neon_buf.val[3], 3);

        vand8(tmp.val[0], tmp.val[0], const_0x11);
        vand8(tmp.val[1], tmp.val[1], const_0x11);
        vand8(tmp.val[2], tmp.val[2], const_0x11);
        vand8(tmp.val[3], tmp.val[3], const_0x11);

        vadd8(res.val[0], res.val[0], tmp.val[0]);
        vadd8(res.val[1], res.val[1], tmp.val[1]);
        vadd8(res.val[2], res.val[2], tmp.val[2]);
        vadd8(res.val[3], res.val[3], tmp.val[3]);

        // Get a
        // 0, 4, 8 , 12 -- 16, 20, 24, 28
        // 1, 5, 9 , 13 -- 17, 21, 25, 29
        // 2, 6, 10, 14 -- 18, 22, 26, 30
        // 3, 7, 11, 15 -- 19, 23, 27, 31
        vand8(a.val[0], res.val[0], const_0xf);
        vand8(a.val[1], res.val[1], const_0xf);
        vand8(a.val[2], res.val[2], const_0xf);
        vand8(a.val[3], res.val[3], const_0xf);

        // Get b
        // 0, 4, 8 , 12 -- 16, 20, 24, 28
        // 1, 5, 9 , 13 -- 17, 21, 25, 29
        // 2, 6, 10, 14 -- 18, 22, 26, 30
        // 3, 7, 11, 15 -- 19, 23, 27, 31
        vsr8(b.val[0], res.val[0], 4);
        vsr8(b.val[1], res.val[1], 4);
        vsr8(b.val[2], res.val[2], 4);
        vsr8(b.val[3], res.val[3], 4);

        // a - b
        // 0, 4, 8 , 12 -- 16, 20, 24, 28 | 0
        // 1, 5, 9 , 13 -- 17, 21, 25, 29 | 1
        // 2, 6, 10, 14 -- 18, 22, 26, 30 | 2
        // 3, 7, 11, 15 -- 19, 23, 27, 31 | 3
        vsubll8(store1.val[0], vget_low_u8(a.val[0]), vget_low_u8(b.val[0]));
        vsubll8(store1.val[1], vget_low_u8(a.val[1]), vget_low_u8(b.val[1]));
        vsubll8(store1.val[2], vget_low_u8(a.val[2]), vget_low_u8(b.val[2]));
        vsubll8(store1.val[3], vget_low_u8(a.val[3]), vget_low_u8(b.val[3]));

        // a - b
        // 32, 36, 40, 44 -- 48, 52, 56, 60
        // 33, 37, 41, 45 -- 49, 53, 57, 61
        // 34, 38, 42, 46 -- 50, 54, 58, 62
        // 35, 39, 43, 47 -- 51, 55, 59, 63
        vsublh8(store2.val[0], a.val[0], b.val[0]);
        vsublh8(store2.val[1], a.val[1], b.val[1]);
        vsublh8(store2.val[2], a.val[2], b.val[2]);
        vsublh8(store2.val[3], a.val[3], b.val[3]);

        vstore4(&s[i], store1);
        vstore4(&s[i + 32], store2);
    }
}
#endif

#if SABER_MU==10
static uint64_t load40_littleendian(const uint8_t x[5])
{
  uint64_t r;
  r  = (uint64_t) x[0];
  r |= (uint64_t) x[1] << 8;
  r |= (uint64_t) x[2] << 16;
  r |= (uint64_t) x[3] << 24;
  r |= (uint64_t) x[4] << 32;
  return r;
}

static 
void cbd5(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES])
{
  uint64_t t, d, a[4], b[4];
  int i, j;

  for (i = 0; i < SABER_N / 4; i++)
  {
    t = load40_littleendian(buf);
    d = 0;
    for (j = 0; j < 5; j++)
      d += (t >> j) & 0x0842108421UL;

    a[0] = d & 0x1f;
    b[0] = (d >> 5) & 0x1f;
    a[1] = (d >> 10) & 0x1f;
    b[1] = (d >> 15) & 0x1f;
    a[2] = (d >> 20) & 0x1f;
    b[2] = (d >> 25) & 0x1f;
    a[3] = (d >> 30) & 0x1f;
    b[3] = (d >> 35);

    s[4 * i + 0] = (uint16_t)(a[0] - b[0]);
    s[4 * i + 1] = (uint16_t)(a[1] - b[1]);
    s[4 * i + 2] = (uint16_t)(a[2] - b[2]);
    s[4 * i + 3] = (uint16_t)(a[3] - b[3]);
    
    buf += 5;
  }
}
#endif

void cbd(uint16_t s[SABER_N], const uint8_t buf[SABER_POLYCOINBYTES])
{
#if SABER_MU == 6
  cbd3(s, buf);
#elif SABER_MU == 8 
  neon_cbd4(s, buf);
#elif SABER_MU == 10
  cbd5(s, buf);
#else
#error "Unsupported SABER parameter."
#endif
}
