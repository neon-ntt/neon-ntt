CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
          -Wshadow -Wpointer-arith -fomit-frame-pointer -mtune=native -O3
NISTFLAGS += -Wno-unused-result -mpopcnt -maes \
             -mtune=native -O3 -fomit-frame-pointer
RM = /bin/rm

ASM = __asm_NTT.S __asm_iNTT.S  __asm_base_mul.S __asm_poly.S feat.S
SOURCES = kem.c indcpa.c polyvec.c neon_polyvec.c poly.c neon_poly.c ntt.c cbd.c reduce.c verify.c randombytes.c
SOURCESKECCAK = $(SOURCES) fips202x2.c fips202.c neon_symmetric-shake.c symmetric-shake.c rejsample.c
HEADERS = params.h crypto_kem.h indcpa.h polyvec.h poly.h NTT_params.h ntt.h cbd.h reduce.h verify.h randombytes.h
HEADERSKECCAK = $(HEADERS) fips202.h fips202x2.h symmetric.h rejsample.h macros.i macros_common.i

all: test speed

test: kyber_test

speed: kyber_speed

kyber_test: $(ASM) $(SOURCESKECCAK) $(HEADERSKECCAK) kyber_test.c
	$(CC) $(CFLAGS) $(ASM) $(SOURCESKECCAK) kyber_test.c -o kyber_test

kyber_speed: $(ASM) $(SOURCESKECCAK) $(HEADERSKECCAK) m1cycles.c kyber_speed.c
	$(CC) $(CFLAGS) $(ASM) $(SOURCESKECCAK) m1cycles.c kyber_speed.c -o kyber_speed

.PHONY: bench clean

clean:
	-$(RM) -rf *.o
	-$(RM) -rf kyber_test
	-$(RM) -rf kyber_speed



