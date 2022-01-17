#include <arm_neon.h>
#include <stddef.h>
#include "fips202x2.h"

#ifdef PROFILE_HASHING
#include "hal.h"
extern unsigned long long hash_cycles;
#endif

#define NROUNDS 24

// Define NEON operation
// c = load(ptr)
#define vload(ptr) vld1q_u64(ptr);
// ptr <= c;
#define vstore(ptr, c) vst1q_u64(ptr, c);
// c = a ^ b
#define vxor(c, a, b) c = veorq_u64(a, b);
// Rotate by n bit ((a << offset) ^ (a >> (64-offset)))
#define vROL(out, a, offset)    \
  out = vshlq_n_u64(a, offset); \
  out = vsriq_n_u64(out, a, 64 - offset);
// Xor chain: out = a ^ b ^ c ^ d ^ e
#define vXOR4(out, a, b, c, d, e) \
  out = veorq_u64(a, b);          \
  out = veorq_u64(out, c);        \
  out = veorq_u64(out, d);        \
  out = veorq_u64(out, e);
// Not And c = ~a & b
// #define vbic(c, a, b) c = vbicq_u64(b, a);
// Xor Not And: out = a ^ ( (~b) & c)
#define vXNA(out, a, b, c) \
  out = vbicq_u64(c, b);   \
  out = veorq_u64(out, a);
// Rotate by 1 bit, then XOR: a ^ ROL(b): SHA1 instruction, not support
#define vrxor(c, a, b) c = vrax1q_u64(a, b);
// End Define

/* Keccak round constants */
static const uint64_t neon_KeccakF_RoundConstants[NROUNDS] = {
    (uint64_t)0x0000000000000001ULL,
    (uint64_t)0x0000000000008082ULL,
    (uint64_t)0x800000000000808aULL,
    (uint64_t)0x8000000080008000ULL,
    (uint64_t)0x000000000000808bULL,
    (uint64_t)0x0000000080000001ULL,
    (uint64_t)0x8000000080008081ULL,
    (uint64_t)0x8000000000008009ULL,
    (uint64_t)0x000000000000008aULL,
    (uint64_t)0x0000000000000088ULL,
    (uint64_t)0x0000000080008009ULL,
    (uint64_t)0x000000008000000aULL,
    (uint64_t)0x000000008000808bULL,
    (uint64_t)0x800000000000008bULL,
    (uint64_t)0x8000000000008089ULL,
    (uint64_t)0x8000000000008003ULL,
    (uint64_t)0x8000000000008002ULL,
    (uint64_t)0x8000000000000080ULL,
    (uint64_t)0x000000000000800aULL,
    (uint64_t)0x800000008000000aULL,
    (uint64_t)0x8000000080008081ULL,
    (uint64_t)0x8000000000008080ULL,
    (uint64_t)0x0000000080000001ULL,
    (uint64_t)0x8000000080008008ULL};

/*************************************************
* Name:        KeccakF1600_StatePermutex2
*
* Description: The Keccak F1600 Permutation
*
* Arguments:   - uint64_t *state: pointer to input/output Keccak state
**************************************************/
extern void f1600x2(v128*, const uint64_t*);
static inline 
void KeccakF1600_StatePermutex2(v128 state[25])
{
  f1600x2(state, neon_KeccakF_RoundConstants);
}

/*************************************************
* Name:        keccakx2_absorb
*
* Description: Absorb step of Keccak;
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   - uint64_t *s: pointer to (uninitialized) output Keccak state
*              - unsigned int r: rate in bytes (e.g., 168 for SHAKE128)
*              - const uint8_t *m: pointer to input to be absorbed into s
*              - size_t mlen: length of input in bytes
*              - uint8_t p: domain-separation byte for different
*                           Keccak-derived functions
**************************************************/
static
void keccakx2_absorb(v128 s[25],
                     unsigned int r,
                     const uint8_t *in0,
                     const uint8_t *in1,
                     size_t inlen,
                     uint8_t p)
{
  size_t i, pos = 0;

  // Declare SIMD registers
  v128 tmp, mask;
  uint64x1_t a, b;
  uint64x2_t a1, b1, atmp1, btmp1;
  uint64x2x2_t a2, b2, atmp2, btmp2;
  // End

  for (i = 0; i < 25; ++i)
    s[i] = vdupq_n_u64(0);

  // Load in0[i] to register, then in1[i] to register, exchange them
  while (inlen >= r)
  {
    for (i = 0; i < r / 8 - 1; i += 4)
    {
      a2 = vld1q_u64_x2((uint64_t *)&in0[pos]);
      b2 = vld1q_u64_x2((uint64_t *)&in1[pos]);
      // BD = zip1(AB and CD)
      atmp2.val[0] = vzip1q_u64(a2.val[0], b2.val[0]);
      atmp2.val[1] = vzip1q_u64(a2.val[1], b2.val[1]);
      // AC = zip2(AB and CD)
      btmp2.val[0] = vzip2q_u64(a2.val[0], b2.val[0]);
      btmp2.val[1] = vzip2q_u64(a2.val[1], b2.val[1]);

      vxor(s[i + 0], s[i + 0], atmp2.val[0]);
      vxor(s[i + 1], s[i + 1], btmp2.val[0]);
      vxor(s[i + 2], s[i + 2], atmp2.val[1]);
      vxor(s[i + 3], s[i + 3], btmp2.val[1]);

      pos += 8 * 2 * 2;
    }
    // Last iteration
    i = r / 8 - 1;
    a = vld1_u64((uint64_t *)&in0[pos]);
    b = vld1_u64((uint64_t *)&in1[pos]);
    tmp = vcombine_u64(a, b);
    vxor(s[i], s[i], tmp);
    pos += 8;

    KeccakF1600_StatePermutex2(s);
    inlen -= r;
  }

  i = 0;
  while (inlen >= 16)
  {
    a1 = vld1q_u64((uint64_t *)&in0[pos]);
    b1 = vld1q_u64((uint64_t *)&in1[pos]);
    // BD = zip1(AB and CD)
    atmp1 = vzip1q_u64(a1, b1);
    // AC = zip2(AB and CD)
    btmp1 = vzip2q_u64(a1, b1);

    vxor(s[i + 0], s[i + 0], atmp1);
    vxor(s[i + 1], s[i + 1], btmp1);

    i += 2;
    pos += 8 * 2;
    inlen -= 8 * 2;
  }

  if (inlen >= 8)
  {
    a = vld1_u64((uint64_t *)&in0[pos]);
    b = vld1_u64((uint64_t *)&in1[pos]);
    tmp = vcombine_u64(a, b);
    vxor(s[i], s[i], tmp);

    i++;
    pos += 8;
    inlen -= 8;
  }

  if (inlen)
  {
    a = vld1_u64((uint64_t *)&in0[pos]);
    b = vld1_u64((uint64_t *)&in1[pos]);
    tmp = vcombine_u64(a, b);
    mask = vdupq_n_u64((1ULL << (8 * inlen)) - 1);
    tmp = vandq_u64(tmp, mask);
    vxor(s[i], s[i], tmp);
  }

  tmp = vdupq_n_u64((uint64_t)p << (8 * inlen));
  vxor(s[i], s[i], tmp);

  mask = vdupq_n_u64(1ULL << 63);
  vxor(s[r / 8 - 1], s[r / 8 - 1], mask);
}

/*************************************************
* Name:        keccak_squeezeblocks
*
* Description: Squeeze step of Keccak. Squeezes full blocks of r bytes each.
*              Modifies the state. Can be called multiple times to keep
*              squeezing, i.e., is incremental.
*
* Arguments:   - uint8_t *out: pointer to output blocks
*              - size_t nblocks: number of blocks to be squeezed (written to h)
*              - unsigned int r: rate in bytes (e.g., 168 for SHAKE128)
*              - uint64_t *s: pointer to input/output Keccak state
**************************************************/
static 
void keccakx2_squeezeblocks(uint8_t *out0,
                            uint8_t *out1,
                            size_t nblocks,
                            unsigned int r,
                            v128 s[25]){
  unsigned int i;

  uint64x1_t a, b;
  uint64x2x2_t a2, b2;

  while (nblocks > 0)
  {
    KeccakF1600_StatePermutex2(s);

    for (i = 0; i < r / 8 - 1; i += 4)
    {
      a2.val[0] = vuzp1q_u64(s[i], s[i + 1]);
      b2.val[0] = vuzp2q_u64(s[i], s[i + 1]);
      a2.val[1] = vuzp1q_u64(s[i + 2], s[i + 3]);
      b2.val[1] = vuzp2q_u64(s[i + 2], s[i + 3]);
      vst1q_u64_x2((uint64_t *)out0, a2);
      vst1q_u64_x2((uint64_t *)out1, b2);

      out0 += 32;
      out1 += 32;
    }

    i = r / 8 - 1;
    // Last iteration
    a = vget_low_u64(s[i]);
    b = vget_high_u64(s[i]);
    vst1_u64((uint64_t *)out0, a);
    vst1_u64((uint64_t *)out1, b);

    out0 += 8;
    out1 += 8;

    --nblocks;
  }
}

/*************************************************
* Name:        shake128x2_absorb
*
* Description: Absorb step of the SHAKE128 XOF.
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   - keccakx2_state *state: pointer to (uninitialized) output
*                                     Keccak state
*              - const uint8_t *in:   pointer to input to be absorbed into s
*              - size_t inlen:        length of input in bytes
**************************************************/
void shake128x2_absorb(keccakx2_state *state,
                       const uint8_t *in0,
                       const uint8_t *in1,
                       size_t inlen){
#ifdef PROFILE_HASHING
  uint64_t t0 = hal_get_time();
#endif
  keccakx2_absorb(state->s, SHAKE128_RATE, in0, in1, inlen, 0x1F);
#ifdef PROFILE_HASHING
  uint64_t t1 = hal_get_time();
  hash_cycles += (t1-t0);
#endif
}

/*************************************************
* Name:        shake128_squeezeblocks
*
* Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of
*              SHAKE128_RATE bytes each. Modifies the state. Can be called
*              multiple times to keep squeezing, i.e., is incremental.
*
* Arguments:   - uint8_t *out:    pointer to output blocks
*              - size_t nblocks:  number of blocks to be squeezed
*                                 (written to output)
*              - keccakx2_state *s: pointer to input/output Keccak state
**************************************************/
void shake128x2_squeezeblocks(uint8_t *out0,
                              uint8_t *out1,
                              size_t nblocks,
                              keccakx2_state *state){
#ifdef PROFILE_HASHING
  uint64_t t0 = hal_get_time();
#endif
  keccakx2_squeezeblocks(out0, out1, nblocks, SHAKE128_RATE, state->s);
#ifdef PROFILE_HASHING
  uint64_t t1 = hal_get_time();
  hash_cycles += (t1-t0);
#endif
}

/*************************************************
* Name:        shake256_absorb
*
* Description: Absorb step of the SHAKE256 XOF.
*              non-incremental, starts by zeroeing the state.
*
* Arguments:   - keccakx2_state *s:   pointer to (uninitialized) output Keccak state
*              - const uint8_t *in: pointer to input to be absorbed into s
*              - size_t inlen:      length of input in bytes
**************************************************/
void shake256x2_absorb(keccakx2_state *state,
                       const uint8_t *in0,
                       const uint8_t *in1,
                       size_t inlen){
#ifdef PROFILE_HASHING
  uint64_t t0 = hal_get_time();
#endif
  keccakx2_absorb(state->s, SHAKE256_RATE, in0, in1, inlen, 0x1F);
#ifdef PROFILE_HASHING
  uint64_t t1 = hal_get_time();
  hash_cycles += (t1-t0);
#endif
}

/*************************************************
* Name:        shake256_squeezeblocks
*
* Description: Squeeze step of SHAKE256 XOF. Squeezes full blocks of
*              SHAKE256_RATE bytes each. Modifies the state. Can be called
*              multiple times to keep squeezing, i.e., is incremental.
*
* Arguments:   - uint8_t *out:    pointer to output blocks
*              - size_t nblocks:  number of blocks to be squeezed
*                                 (written to output)
*              - keccakx2_state *s: pointer to input/output Keccak state
**************************************************/
void shake256x2_squeezeblocks(uint8_t *out0,
                              uint8_t *out1,
                              size_t nblocks,
                              keccakx2_state *state){
#ifdef PROFILE_HASHING
  uint64_t t0 = hal_get_time();
#endif
  keccakx2_squeezeblocks(out0, out1, nblocks, SHAKE256_RATE, state->s);
#ifdef PROFILE_HASHING
  uint64_t t1 = hal_get_time();
  hash_cycles += (t1-t0);
#endif
}

/*************************************************
* Name:        shake128
*
* Description: SHAKE128 XOF with non-incremental API
*
* Arguments:   - uint8_t *out:      pointer to output
*              - size_t outlen:     requested output length in bytes
*              - const uint8_t *in: pointer to input
*              - size_t inlen:      length of input in bytes
**************************************************/
void shake128x2(uint8_t *out0,
                uint8_t *out1,
                size_t outlen,
                const uint8_t *in0,
                const uint8_t *in1,
                size_t inlen){
#ifdef PROFILE_HASHING
  uint64_t t0 = hal_get_time();
#endif
  unsigned int i;
  size_t nblocks = outlen / SHAKE128_RATE;
  uint8_t t[2][SHAKE128_RATE];
  keccakx2_state state;

  shake128x2_absorb(&state, in0, in1, inlen);
  shake128x2_squeezeblocks(out0, out1, nblocks, &state);

  out0 += nblocks * SHAKE128_RATE;
  out1 += nblocks * SHAKE128_RATE;
  outlen -= nblocks * SHAKE128_RATE;

  if (outlen)
  {
    shake128x2_squeezeblocks(t[0], t[1], 1, &state);
    for (i = 0; i < outlen; ++i)
    {
      out0[i] = t[0][i];
      out1[i] = t[1][i];
    }
  }
#ifdef PROFILE_HASHING
  uint64_t t1 = hal_get_time();
  hash_cycles += (t1-t0);
#endif
}

/*************************************************
* Name:        shake256
*
* Description: SHAKE256 XOF with non-incremental API
*
* Arguments:   - uint8_t *out:      pointer to output
*              - size_t outlen:     requested output length in bytes
*              - const uint8_t *in: pointer to input
*              - size_t inlen:      length of input in bytes
**************************************************/
void shake256x2(uint8_t *out0,
                uint8_t *out1,
                size_t outlen,
                const uint8_t *in0,
                const uint8_t *in1,
                size_t inlen){
#ifdef PROFILE_HASHING
  uint64_t t0 = hal_get_time();
#endif
  unsigned int i;
  size_t nblocks = outlen / SHAKE256_RATE;
  uint8_t t[2][SHAKE256_RATE];
  keccakx2_state state;

  shake256x2_absorb(&state, in0, in1, inlen);
  shake256x2_squeezeblocks(out0, out1, nblocks, &state);

  out0 += nblocks * SHAKE256_RATE;
  out1 += nblocks * SHAKE256_RATE;
  outlen -= nblocks * SHAKE256_RATE;

  if (outlen)
  {
    shake256x2_squeezeblocks(t[0], t[1], 1, &state);
    for (i = 0; i < outlen; ++i)
    {
      out0[i] = t[0][i];
      out1[i] = t[1][i];
    }
  }
#ifdef PROFILE_HASHING
  uint64_t t1 = hal_get_time();
  hash_cycles += (t1-t0);
#endif
}
