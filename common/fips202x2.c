
/*
 * We offer
 * CC0 1.0 Universal or the following MIT License for this file.
 * You may freely choose one of them that applies.
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 * Copyright (c) 2023: Vincent Hwang
 *
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

#include "fips202x2.h"
#include <stddef.h>
#include <arm_neon.h>

#ifdef PROFILE_HASHING
#include "hal.h"
extern unsigned long long hash_cycles;
#endif

#define NROUNDS 24

// Rotate by n bit ((a << offset) ^ (a >> (64-offset)))
#define ROL(a, offset) vsriq_n_u64(vshlq_n_u64(a, offset), a, 64 - offset)

/*************************************************
* Name:        load64
*
* Description: Load 8 bytes into uint64_t in little-endian order
*
* Arguments:   - const uint8_t *x: pointer to input byte array
*
* Returns the loaded 64-bit unsigned integer
**************************************************/
static uint64_t load64(const uint8_t x[8]) {
    unsigned int i;
    uint64_t r = 0;

    for (i = 0; i < 8; i++) {
        r |= (uint64_t)x[i] << 8 * i;
    }

    return r;
}

/*************************************************
* Name:        store64
*
* Description: Store a 64-bit integer to array of 8 bytes in little-endian order
*
* Arguments:   - uint8_t *x: pointer to the output byte array (allocated)
*              - uint64_t u: input 64-bit unsigned integer
**************************************************/
static void store64(uint8_t x[8], uint64_t u) {
    unsigned int i;

    for (i = 0; i < 8; i++) {
        x[i] = u >> 8 * i;
    }
}

/* Keccak round constants */
static const uint64_t KeccakF_RoundConstants[NROUNDS] = {
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
    (uint64_t)0x8000000080008008ULL
};

/*************************************************
* Name:        KeccakF1600_StatePermutex2
*
* Description: The Keccak F1600 Permutation
*
* Arguments:   - uint64_t *state: pointer to input/output Keccak state
**************************************************/
extern void f1600x2(v128 *, const uint64_t *);
static inline
void KeccakF1600_StatePermutex2(v128 state[25]) {
    #if (__APPLE__ && __ARM_FEATURE_CRYPTO) || (__ARM_FEATURE_SHA3) /* although not sure what is being implemented, we find something fast */
    f1600x2(state, KeccakF_RoundConstants);
    #else
    v128 Aba, Abe, Abi, Abo, Abu;
    v128 Aga, Age, Agi, Ago, Agu;
    v128 Aka, Ake, Aki, Ako, Aku;
    v128 Ama, Ame, Ami, Amo, Amu;
    v128 Asa, Ase, Asi, Aso, Asu;
    v128 BCa, BCe, BCi, BCo, BCu; // tmp
    v128 Da, De, Di, Do, Du;      // D
    v128 Eba, Ebe, Ebi, Ebo, Ebu;
    v128 Ega, Ege, Egi, Ego, Egu;
    v128 Eka, Eke, Eki, Eko, Eku;
    v128 Ema, Eme, Emi, Emo, Emu;
    v128 Esa, Ese, Esi, Eso, Esu;

    //copyFromState(A, state)
    Aba = state[0];
    Abe = state[1];
    Abi = state[2];
    Abo = state[3];
    Abu = state[4];
    Aga = state[5];
    Age = state[6];
    Agi = state[7];
    Ago = state[8];
    Agu = state[9];
    Aka = state[10];
    Ake = state[11];
    Aki = state[12];
    Ako = state[13];
    Aku = state[14];
    Ama = state[15];
    Ame = state[16];
    Ami = state[17];
    Amo = state[18];
    Amu = state[19];
    Asa = state[20];
    Ase = state[21];
    Asi = state[22];
    Aso = state[23];
    Asu = state[24];

    for (int round = 0; round < NROUNDS; round += 2) {

        BCa = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
        BCe = Abe ^ Age ^ Ake ^ Ame ^ Ase;
        BCi = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
        BCo = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
        BCu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;

        //thetaRhoPiChiIotaPrepareTheta(round, A, E)
        Da = BCu ^ ROL(BCe, 1);
        De = BCa ^ ROL(BCi, 1);
        Di = BCe ^ ROL(BCo, 1);
        Do = BCi ^ ROL(BCu, 1);
        Du = BCo ^ ROL(BCa, 1);

        Aba ^= Da;
        BCa = Aba;
        Age ^= De;
        BCe = ROL(Age, 44);
        Aki ^= Di;
        BCi = ROL(Aki, 43);
        Amo ^= Do;
        BCo = ROL(Amo, 21);
        Asu ^= Du;
        BCu = ROL(Asu, 14);
        Eba =   BCa ^ ((~BCe)&  BCi );
        Eba ^= vdupq_n_u64(KeccakF_RoundConstants[round]);
        Ebe =   BCe ^ ((~BCi)&  BCo );
        Ebi =   BCi ^ ((~BCo)&  BCu );
        Ebo =   BCo ^ ((~BCu)&  BCa );
        Ebu =   BCu ^ ((~BCa)&  BCe );

        Abo ^= Do;
        BCa = ROL(Abo, 28);
        Agu ^= Du;
        BCe = ROL(Agu, 20);
        Aka ^= Da;
        BCi = ROL(Aka,  3);
        Ame ^= De;
        BCo = ROL(Ame, 45);
        Asi ^= Di;
        BCu = ROL(Asi, 61);
        Ega =   BCa ^ ((~BCe)&  BCi );
        Ege =   BCe ^ ((~BCi)&  BCo );
        Egi =   BCi ^ ((~BCo)&  BCu );
        Ego =   BCo ^ ((~BCu)&  BCa );
        Egu =   BCu ^ ((~BCa)&  BCe );

        Abe ^= De;
        BCa = ROL(Abe,  1);
        Agi ^= Di;
        BCe = ROL(Agi,  6);
        Ako ^= Do;
        BCi = ROL(Ako, 25);
        Amu ^= Du;
        BCo = ROL(Amu,  8);
        Asa ^= Da;
        BCu = ROL(Asa, 18);
        Eka =   BCa ^ ((~BCe)&  BCi );
        Eke =   BCe ^ ((~BCi)&  BCo );
        Eki =   BCi ^ ((~BCo)&  BCu );
        Eko =   BCo ^ ((~BCu)&  BCa );
        Eku =   BCu ^ ((~BCa)&  BCe );

        Abu ^= Du;
        BCa = ROL(Abu, 27);
        Aga ^= Da;
        BCe = ROL(Aga, 36);
        Ake ^= De;
        BCi = ROL(Ake, 10);
        Ami ^= Di;
        BCo = ROL(Ami, 15);
        Aso ^= Do;
        BCu = ROL(Aso, 56);
        Ema =   BCa ^ ((~BCe)&  BCi );
        Eme =   BCe ^ ((~BCi)&  BCo );
        Emi =   BCi ^ ((~BCo)&  BCu );
        Emo =   BCo ^ ((~BCu)&  BCa );
        Emu =   BCu ^ ((~BCa)&  BCe );

        Abi ^= Di;
        BCa = ROL(Abi, 62);
        Ago ^= Do;
        BCe = ROL(Ago, 55);
        Aku ^= Du;
        BCi = ROL(Aku, 39);
        Ama ^= Da;
        BCo = ROL(Ama, 41);
        Ase ^= De;
        BCu = ROL(Ase,  2);
        Esa =   BCa ^ ((~BCe)&  BCi );
        Ese =   BCe ^ ((~BCi)&  BCo );
        Esi =   BCi ^ ((~BCo)&  BCu );
        Eso =   BCo ^ ((~BCu)&  BCa );
        Esu =   BCu ^ ((~BCa)&  BCe );

        //    prepareTheta
        BCa = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
        BCe = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
        BCi = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
        BCo = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
        BCu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;

        //thetaRhoPiChiIotaPrepareTheta(round+1, E, A)
        Da = BCu ^ ROL(BCe, 1);
        De = BCa ^ ROL(BCi, 1);
        Di = BCe ^ ROL(BCo, 1);
        Do = BCi ^ ROL(BCu, 1);
        Du = BCo ^ ROL(BCa, 1);

        Eba ^= Da;
        BCa = Eba;
        Ege ^= De;
        BCe = ROL(Ege, 44);
        Eki ^= Di;
        BCi = ROL(Eki, 43);
        Emo ^= Do;
        BCo = ROL(Emo, 21);
        Esu ^= Du;
        BCu = ROL(Esu, 14);
        Aba =   BCa ^ ((~BCe)&  BCi );
        Aba ^= vdupq_n_u64(KeccakF_RoundConstants[round + 1]);
        Abe =   BCe ^ ((~BCi)&  BCo );
        Abi =   BCi ^ ((~BCo)&  BCu );
        Abo =   BCo ^ ((~BCu)&  BCa );
        Abu =   BCu ^ ((~BCa)&  BCe );

        Ebo ^= Do;
        BCa = ROL(Ebo, 28);
        Egu ^= Du;
        BCe = ROL(Egu, 20);
        Eka ^= Da;
        BCi = ROL(Eka, 3);
        Eme ^= De;
        BCo = ROL(Eme, 45);
        Esi ^= Di;
        BCu = ROL(Esi, 61);
        Aga =   BCa ^ ((~BCe)&  BCi );
        Age =   BCe ^ ((~BCi)&  BCo );
        Agi =   BCi ^ ((~BCo)&  BCu );
        Ago =   BCo ^ ((~BCu)&  BCa );
        Agu =   BCu ^ ((~BCa)&  BCe );

        Ebe ^= De;
        BCa = ROL(Ebe, 1);
        Egi ^= Di;
        BCe = ROL(Egi, 6);
        Eko ^= Do;
        BCi = ROL(Eko, 25);
        Emu ^= Du;
        BCo = ROL(Emu, 8);
        Esa ^= Da;
        BCu = ROL(Esa, 18);
        Aka =   BCa ^ ((~BCe)&  BCi );
        Ake =   BCe ^ ((~BCi)&  BCo );
        Aki =   BCi ^ ((~BCo)&  BCu );
        Ako =   BCo ^ ((~BCu)&  BCa );
        Aku =   BCu ^ ((~BCa)&  BCe );

        Ebu ^= Du;
        BCa = ROL(Ebu, 27);
        Ega ^= Da;
        BCe = ROL(Ega, 36);
        Eke ^= De;
        BCi = ROL(Eke, 10);
        Emi ^= Di;
        BCo = ROL(Emi, 15);
        Eso ^= Do;
        BCu = ROL(Eso, 56);
        Ama =   BCa ^ ((~BCe)&  BCi );
        Ame =   BCe ^ ((~BCi)&  BCo );
        Ami =   BCi ^ ((~BCo)&  BCu );
        Amo =   BCo ^ ((~BCu)&  BCa );
        Amu =   BCu ^ ((~BCa)&  BCe );

        Ebi ^= Di;
        BCa = ROL(Ebi, 62);
        Ego ^= Do;
        BCe = ROL(Ego, 55);
        Eku ^= Du;
        BCi = ROL(Eku, 39);
        Ema ^= Da;
        BCo = ROL(Ema, 41);
        Ese ^= De;
        BCu = ROL(Ese, 2);
        Asa =   BCa ^ ((~BCe)&  BCi );
        Ase =   BCe ^ ((~BCi)&  BCo );
        Asi =   BCi ^ ((~BCo)&  BCu );
        Aso =   BCo ^ ((~BCu)&  BCa );
        Asu =   BCu ^ ((~BCa)&  BCe );

    }

    state[0] = Aba;
    state[1] = Abe;
    state[2] = Abi;
    state[3] = Abo;
    state[4] = Abu;
    state[5] = Aga;
    state[6] = Age;
    state[7] = Agi;
    state[8] = Ago;
    state[9] = Agu;
    state[10] = Aka;
    state[11] = Ake;
    state[12] = Aki;
    state[13] = Ako;
    state[14] = Aku;
    state[15] = Ama;
    state[16] = Ame;
    state[17] = Ami;
    state[18] = Amo;
    state[19] = Amu;
    state[20] = Asa;
    state[21] = Ase;
    state[22] = Asi;
    state[23] = Aso;
    state[24] = Asu;
    #endif
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
                     uint8_t p) {

    unsigned int i;
    uint64_t t64x2[2];
    v128 t128;

    for(i = 0; i < 25; i++){
        s[i] = s[i] ^ s[i];
    }

    while (inlen >= r) {
        for(i = 0; i < r / 8; i++){
            t64x2[0] = load64(in0 + 8 * i);
            t64x2[1] = load64(in1 + 8 * i);
            t128 = vld1q_u64(t64x2);
            s[i] ^= t128;
        }
        in0 += r;
        in1 += r;
        inlen -= r;
        KeccakF1600_StatePermutex2(s);
    }

    for(i = 0; i < inlen; i++){
        t64x2[0] = in0[i];
        t64x2[1] = in1[i];
        t128 = vld1q_u64(t64x2);
        s[i / 8] ^= t128 << 8 * (i & 7);
    }

    s[i / 8] ^= (uint64_t)p << 8 * (i & 7);
    s[(r - 1) / 8] ^= 1ULL << 63;

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
                            v128 s[25]) {
    unsigned int i;

    while (nblocks > 0) {
        KeccakF1600_StatePermutex2(s);
        for (i = 0; i < r / 8; i++) {
            store64(out0 + 8 * i, *((uint64_t*)(s + i) + 0));
            store64(out1 + 8 * i, *((uint64_t*)(s + i) + 1));
        }
        out0 += r;
        out1 += r;
        nblocks -= 1;
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
                       size_t inlen) {
    #ifdef PROFILE_HASHING
    uint64_t t0 = hal_get_time();
    #endif
    keccakx2_absorb(state->s, SHAKE128_RATE, in0, in1, inlen, 0x1F);
    #ifdef PROFILE_HASHING
    uint64_t t1 = hal_get_time();
    hash_cycles += (t1 - t0);
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
                              keccakx2_state *state) {
    #ifdef PROFILE_HASHING
    uint64_t t0 = hal_get_time();
    #endif
    keccakx2_squeezeblocks(out0, out1, nblocks, SHAKE128_RATE, state->s);
    #ifdef PROFILE_HASHING
    uint64_t t1 = hal_get_time();
    hash_cycles += (t1 - t0);
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
                       size_t inlen) {
    #ifdef PROFILE_HASHING
    uint64_t t0 = hal_get_time();
    #endif
    keccakx2_absorb(state->s, SHAKE256_RATE, in0, in1, inlen, 0x1F);
    #ifdef PROFILE_HASHING
    uint64_t t1 = hal_get_time();
    hash_cycles += (t1 - t0);
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
                              keccakx2_state *state) {
    #ifdef PROFILE_HASHING
    uint64_t t0 = hal_get_time();
    #endif
    keccakx2_squeezeblocks(out0, out1, nblocks, SHAKE256_RATE, state->s);
    #ifdef PROFILE_HASHING
    uint64_t t1 = hal_get_time();
    hash_cycles += (t1 - t0);
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
                size_t inlen) {
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

    if (outlen) {
        shake128x2_squeezeblocks(t[0], t[1], 1, &state);
        for (i = 0; i < outlen; ++i) {
            out0[i] = t[0][i];
            out1[i] = t[1][i];
        }
    }
    #ifdef PROFILE_HASHING
    uint64_t t1 = hal_get_time();
    hash_cycles += (t1 - t0);
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
                size_t inlen) {
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

    if (outlen) {
        shake256x2_squeezeblocks(t[0], t[1], 1, &state);
        for (i = 0; i < outlen; ++i) {
            out0[i] = t[0][i];
            out1[i] = t[1][i];
        }
    }
    #ifdef PROFILE_HASHING
    uint64_t t1 = hal_get_time();
    hash_cycles += (t1 - t0);
    #endif
}
