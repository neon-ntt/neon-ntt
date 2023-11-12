#include <string.h>
#include "chacha20.h"
#include "rng.h"

unsigned char __attribute__((aligned (16)))nonce[NONCEBYTES] = {0};

int crypto_rng(
        unsigned char *r, /* random output */
        unsigned char *n, /* new key */
  const unsigned char *g  /* old key */
)
{
  unsigned char __attribute__((aligned (16)))x[KEYBYTES + OUTPUTBYTES];
  crypto_stream(x,sizeof x,nonce,g);
  memcpy(n,x,KEYBYTES);
  memcpy(r,x + KEYBYTES,OUTPUTBYTES);
  return 0;
}
