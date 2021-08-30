#ifndef API_H
#define API_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"


#define CRYPTO_PUBLICKEYBYTES DILITHIUM_CRYPTO_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES DILITHIUM_CRYPTO_SECRETKEYBYTES
#define CRYPTO_BYTES DILITHIUM_CRYPTO_BYTES



int crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

int crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

int crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk);

int crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

int crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen, const uint8_t *pk);

#endif
