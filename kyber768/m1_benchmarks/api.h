
#ifndef API_H
#define API_H

/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#include <stdint.h>
#include "params.h"

#define CRYPTO_PUBLICKEYBYTES KYBER_NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define KYBER_AARCH64_CRYPTO_PUBLICKEYBYTES KYBER_PUBLICKEYBYTES
// #define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES           KYBER_SSBYTES

#define crypto_kem_keypair KYBER_NAMESPACE(crypto_kem_keypair)
int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

#define crypto_kem_enc KYBER_NAMESPACE(crypto_kem_enc)
int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

#define crypto_kem_dec KYBER_NAMESPACE(crypto_kem_dec)
int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif

