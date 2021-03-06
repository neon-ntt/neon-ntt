/*=============================================================================
This file has been adapted from the implementation 
(available at, Public Domain https://github.com/KULeuven-COSIC/SABER) 
of "Saber: Module-LWR based key exchange, CPA-secure encryption and CCA-secure KEM"
by : Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, and Frederik Vercauteren
Jose Maria Bermudo Mera, Michiel Van Beirendonck, Andrea Basso. 
=============================================================================*/
#ifndef PARAMS_H
#define PARAMS_H

/* Change this for different security strengths */
#ifndef SABER_L
// #define SABER_L 2 /* LightSaber */
// #define SABER_L 3 /* Saber */
#define SABER_L 4 /* FireSaber */
#endif 

/* Don't change anything below this line */
#if SABER_L == 2
	#define SABER_MU 10
	#define SABER_ET 3
#elif SABER_L == 3
	#define SABER_MU 8
	#define SABER_ET 4
#elif SABER_L == 4
	#define SABER_MU 6
	#define SABER_ET 6
#endif

#define SABER_EQ 13
#define SABER_EP 10
#define SABER_N 256

#define SABER_Q 8192 //2^13
#define SABER_P 1024

#define SABER_SEEDBYTES 32
#define SABER_NOISE_SEEDBYTES 32
#define SABER_KEYBYTES 32
#define SABER_HASHBYTES 32

#define SABER_POLYCOINBYTES (SABER_MU * SABER_N / 8)

#define SABER_POLYBYTES (SABER_EQ * SABER_N / 8)
#define SABER_POLYVECBYTES (SABER_L * SABER_POLYBYTES)

#define SABER_POLYCOMPRESSEDBYTES (SABER_EP * SABER_N / 8)
#define SABER_POLYVECCOMPRESSEDBYTES (SABER_L * SABER_POLYCOMPRESSEDBYTES)

#define SABER_SCALEBYTES_KEM (SABER_ET * SABER_N / 8)

#define SABER_INDCPA_PUBLICKEYBYTES (SABER_POLYVECCOMPRESSEDBYTES + SABER_SEEDBYTES)
#define SABER_INDCPA_SECRETKEYBYTES (SABER_POLYVECBYTES)

#define SABER_PUBLICKEYBYTES (SABER_INDCPA_PUBLICKEYBYTES)
#define SABER_SECRETKEYBYTES (SABER_INDCPA_SECRETKEYBYTES + SABER_INDCPA_PUBLICKEYBYTES + SABER_HASHBYTES + SABER_KEYBYTES)

#define SABER_BYTES_CCA_DEC (SABER_POLYVECCOMPRESSEDBYTES + SABER_SCALEBYTES_KEM)

#endif
