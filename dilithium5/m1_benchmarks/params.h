#ifndef PARAMS_H
#define PARAMS_H

#define CRYPTO_NAMESPACE(s) dilithium_##s
#define CRYPTO_NAMESPACETOP crypto_sign

//#define DILITHIUM_MODE 2
//#define DILITHIUM_MODE 3
#define DILITHIUM_MODE 5

#define SEEDBYTES 32
#define CRHBYTES 64
#define N 256
#define DILITHIUM_Q 8380417
#define D 13
#define ROOT_OF_UNITY 1753

#if DILITHIUM_MODE == 2

#define K 4
#define L 4
#define ETA 2
#define TAU 39
#define BETA 78
#define GAMMA1 (1 << 17)
#define GAMMA2 ((DILITHIUM_Q-1)/88)
#define OMEGA 80
#define CRYPTO_ALGNAME "Dilithium2"

#elif DILITHIUM_MODE == 3

#define K 6
#define L 5
#define ETA 4
#define TAU 49
#define BETA 196
#define GAMMA1 (1 << 19)
#define GAMMA2 ((DILITHIUM_Q-1)/32)
#define OMEGA 55
#define CRYPTO_ALGNAME "Dilithium3"

#elif DILITHIUM_MODE == 5

#define K 8
#define L 7
#define ETA 2
#define TAU 60
#define BETA 120
#define GAMMA1 (1 << 19)
#define GAMMA2 ((DILITHIUM_Q-1)/32)
#define OMEGA 75
#define CRYPTO_ALGNAME "Dilithium5"

#else

#error "No parameter specified!"

#endif

#define POLYT1_PACKEDBYTES  320
#define POLYT0_PACKEDBYTES  416
#define POLYVECH_PACKEDBYTES (OMEGA + K)


#if GAMMA1 == (1 << 17)
#define POLYZ_PACKEDBYTES   576
#elif GAMMA1 == (1 << 19)
#define POLYZ_PACKEDBYTES   640
#endif

#if GAMMA2 == (DILITHIUM_Q-1)/88
#define POLYW1_PACKEDBYTES  192
#elif GAMMA2 == (DILITHIUM_Q-1)/32
#define POLYW1_PACKEDBYTES  128
#endif

#if ETA == 2
#define POLYETA_PACKEDBYTES  96
#elif ETA == 4
#define POLYETA_PACKEDBYTES 128
#endif

#define DILITHIUM_CRYPTO_PUBLICKEYBYTES (SEEDBYTES + K*POLYT1_PACKEDBYTES)
#define DILITHIUM_CRYPTO_SECRETKEYBYTES (3*SEEDBYTES \
        + L*POLYETA_PACKEDBYTES \
        + K*POLYETA_PACKEDBYTES \
        + K*POLYT0_PACKEDBYTES)
#define DILITHIUM_CRYPTO_BYTES (SEEDBYTES + L*POLYZ_PACKEDBYTES + POLYVECH_PACKEDBYTES)

#endif
