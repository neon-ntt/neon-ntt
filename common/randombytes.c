#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "randombytes.h"

#if defined(NORAND) || defined(BENCH) || defined(BENCH_RAND)

#pragma message("using non-random randombytes!")

#include <stdlib.h>
#include <string.h>

#include "rng.h"

unsigned char __attribute__((aligned (16)))keybytes[crypto_rng_KEYBYTES] = {
  0x49, 0x54, 0xcc, 0x49, 0xa4, 0x94, 0xba, 0x0,
  0x41, 0x76, 0x78, 0x17, 0x5f, 0xb9, 0xfb, 0x23,
  0x18, 0x91, 0x65, 0xb7, 0x90, 0xb4, 0x9f, 0x65,
  0x91, 0x6c, 0xe4, 0xc1, 0xde, 0xac, 0xf4, 0x6c
};
unsigned char __attribute__((aligned (16)))outbytes[crypto_rng_OUTPUTBYTES];
unsigned long long pos = crypto_rng_OUTPUTBYTES;


static void randombytes_internal(uint8_t *x, size_t xlen){

#ifdef SIMPLE

  while (xlen > 0) {
    if (pos == crypto_rng_OUTPUTBYTES) {
      crypto_rng(outbytes,keybytes,keybytes);
      pos = 0;
    }
    *x++ = outbytes[pos]; xlen -= 1;
    outbytes[pos++] = 0;
  }

#else /* same output but optimizing copies */

  while (xlen > 0) {
    unsigned long long ready;

    if (pos == crypto_rng_OUTPUTBYTES) {
      while (xlen > crypto_rng_OUTPUTBYTES) {
        crypto_rng(x,keybytes,keybytes);
        x += crypto_rng_OUTPUTBYTES;
        xlen -= crypto_rng_OUTPUTBYTES;
      }
      if (xlen == 0) return;

      crypto_rng(outbytes,keybytes,keybytes);
      pos = 0;
    }

    ready = crypto_rng_OUTPUTBYTES - pos;
    if (xlen <= ready) ready = xlen;
    memcpy(x,outbytes + pos,ready);
    memset(outbytes + pos,0,ready);
    x += ready;
    xlen -= ready;
    pos += ready;
  }

#endif

}

void randombytes(uint8_t *x, size_t xlen)
{
  randombytes_internal(x,xlen);
}

#else

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#else
#include <fcntl.h>
#include <errno.h>
#ifdef __linux__
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#else
#include <unistd.h>
#endif
#endif

#ifdef _WIN32
void randombytes(uint8_t *out, size_t outlen) {
  HCRYPTPROV ctx;
  size_t len;

  if(!CryptAcquireContext(&ctx, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    abort();

  while(outlen > 0) {
    len = (outlen > 1048576) ? 1048576 : outlen;
    if(!CryptGenRandom(ctx, len, (BYTE *)out))
      abort();

    out += len;
    outlen -= len;
  }

  if(!CryptReleaseContext(ctx, 0))
    abort();
}
#elif defined(__linux__) && defined(SYS_getrandom)
void randombytes(uint8_t *out, size_t outlen) {
  ssize_t ret;

  while(outlen > 0) {
    ret = syscall(SYS_getrandom, out, outlen, 0);
    if(ret == -1 && errno == EINTR)
      continue;
    else if(ret == -1)
      abort();

    out += ret;
    outlen -= ret;
  }
}
#else
void randombytes(uint8_t *out, size_t outlen) {
  static int fd = -1;
  ssize_t ret;

  while(fd == -1) {
    fd = open("/dev/urandom", O_RDONLY);
    if(fd == -1 && errno == EINTR)
      continue;
    else if(fd == -1)
      abort();
  }

  while(outlen > 0) {
    ret = read(fd, out, outlen);
    if(ret == -1 && errno == EINTR)
      continue;
    else if(ret == -1)
      abort();

    out += ret;
    outlen -= ret;
  }
}

#endif

#endif
