
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "api.h"
#include "randombytes.h"

#define NTESTS 5

static void printbytes(const uint8_t *x, size_t xlen) {
    size_t i;
    for (i = 0; i < xlen; i++) {
        printf("%02x", x[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t key_a[KYBER_AARCH64_CRYPTO_BYTES], key_b[KYBER_AARCH64_CRYPTO_BYTES];
    uint8_t pk[KYBER_AARCH64_CRYPTO_PUBLICKEYBYTES];
    uint8_t sendb[KYBER_AARCH64_CRYPTO_CIPHERTEXTBYTES];
    uint8_t sk_a[KYBER_AARCH64_CRYPTO_SECRETKEYBYTES];
    int i, j;
    for (i = 0; i < NTESTS; i++) {
        // Key-pair generation
        crypto_kem_keypair(pk, sk_a);

        printbytes(pk, KYBER_AARCH64_CRYPTO_PUBLICKEYBYTES);
        printbytes(sk_a, KYBER_AARCH64_CRYPTO_SECRETKEYBYTES);

        // Encapsulation
        crypto_kem_enc(sendb, key_b, pk);

        printbytes(sendb, KYBER_AARCH64_CRYPTO_CIPHERTEXTBYTES);
        printbytes(key_b, KYBER_AARCH64_CRYPTO_BYTES);

        // Decapsulation
        crypto_kem_dec(key_a, sendb, sk_a);
        printbytes(key_a, KYBER_AARCH64_CRYPTO_BYTES);

        for (j = 0; j < KYBER_AARCH64_CRYPTO_BYTES; j++) {
            if (key_a[j] != key_b[j]) {
                printf("ERROR\n");
                return -1;
            }
        }
    }
    return 0;
}


