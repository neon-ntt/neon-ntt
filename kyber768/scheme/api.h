#ifndef KYBER_AARCH64_API_H
#define KYBER_AARCH64_API_H

/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#include <stdint.h>

#define KYBER_AARCH64_CRYPTO_SECRETKEYBYTES  2400
#define KYBER_AARCH64_CRYPTO_PUBLICKEYBYTES  1184
#define KYBER_AARCH64_CRYPTO_CIPHERTEXTBYTES 1088
#define KYBER_AARCH64_CRYPTO_BYTES           32
#define KYBER_AARCH64_CRYPTO_ALGNAME         "Kyber768"

int KYBER_AARCH64_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

int KYBER_AARCH64_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

int KYBER_AARCH64_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
