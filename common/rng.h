#ifndef RNG_H
#define RNG_H

#include "chacha20.h"

#define crypto_rng_KEYBYTES 32
#define crypto_rng_OUTPUTBYTES 736

//
#define crypto_stream crypto_stream_chacha20
#define KEYBYTES CHACHA20_KEYBYTES
#define NONCEBYTES CHACHA20_NONCEBYTES
#define OUTPUTBYTES crypto_rng_OUTPUTBYTES

extern unsigned char nonce[NONCEBYTES];

int crypto_rng(
        unsigned char *r, /* random output */
        unsigned char *n, /* new key */
  const unsigned char *g  /* old key */
);

#endif
