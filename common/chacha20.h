#ifndef CHACHA20_OPT_H
#define CHACHA20_OPT_H

#define CHACHA20_KEYBYTES 32
#define CHACHA20_NONCEBYTES 8

int crypto_stream_chacha20(unsigned char *c,unsigned long long clen, const unsigned char *n, const unsigned char *k);

#endif
