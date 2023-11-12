
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "params.h"
#include "api.h"
#include "randombytes.h"

#define MAXMLEN 2048

static void printbytes(const uint8_t *x, size_t xlen) {
    size_t i;
    for (i = 0; i < xlen; i++) {
        printf("%02x", x[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];

    uint8_t mi[MAXMLEN];
    uint8_t sm[MAXMLEN + CRYPTO_BYTES];
    uint8_t sig[CRYPTO_BYTES];

    size_t smlen;
    size_t siglen;
    size_t mlen;

    int r;
    size_t i, k;

    /* i = 0, 1, 4, 16, 64, 256, 1024 */
    for (i = 0; i < MAXMLEN; i = (i == 0) ? i + 1 : i << 2) {
        randombytes(mi, i);

        crypto_sign_keypair(pk, sk);

        printbytes(pk, CRYPTO_PUBLICKEYBYTES);
        printbytes(sk, CRYPTO_SECRETKEYBYTES);

        crypto_sign(sm, &smlen, mi, i, sk);
        crypto_sign_signature(sig, &siglen, mi, i, sk);

        printbytes(sm, smlen);
        printbytes(sig, siglen);

        // By relying on m == sm we prevent having to allocate CRYPTO_BYTES
        // twice
        r = crypto_sign_open(sm, &mlen, sm, smlen, pk);
        r |= crypto_sign_verify(sig, siglen, mi, i, pk);

        if (r) {
            printf("ERROR: signature verification failed\n");
            return -1;
        }
        for (k = 0; k < i; k++) {
            if (sm[k] != mi[k]) {
                printf("ERROR: message recovery failed\n");
                return -1;
            }
        }
    }
    return 0;
}

