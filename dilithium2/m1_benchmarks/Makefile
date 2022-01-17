CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
          -Wshadow -Wpointer-arith -fomit-frame-pointer -mtune=native -O3
NISTFLAGS += -Wno-unused-result -mpopcnt -maes \
             -mtune=native -O3 -fomit-frame-pointer
RM = /bin/rm

ASM = __asm_NTT.S __asm_iNTT.S __asm_poly.S
SOURCES = sign.c ntt.c packing.c poly.c polyvec.c reduce.c rounding.c randombytes.c
SOURCESKECCAK = $(SOURCES) fips202.c fips202x2.c symmetric-shake.c
HEADERS = params.h sign.h ntt.h packing.h poly.h polyvec.h reduce.h rounding.h randombytes.h
HEADERSKECCAK = $(HEADERS) fips202.h fips202x2.h symmetric.h macros.i macros_common.i

all: test speed

test: dilithium_test

speed: dilithium_speed

feat.o: feat.s
	$(CC) -c $< -o $@ $(CFLAGS) -march=armv8.2-a+sha3

dilithium_test: $(ASM) $(SOURCESKECCAK) $(HEADERSKECCAK) feat.o dilithium_test.c
	$(CC) $(CFLAGS) $(ASM) $(SOURCESKECCAK) feat.o dilithium_test.c -o dilithium_test

dilithium_speed: $(ASM) $(SOURCESKECCAK) $(HEADERSKECCAK) feat.o m1cycles.c dilithium_speed.c
	$(CC) $(CFLAGS) $(ASM) $(SOURCESKECCAK) feat.o m1cycles.c dilithium_speed.c -o dilithium_speed

.PHONY: clean

clean:
	-$(RM) -rf *.o
	-$(RM) -rf dilithium_test
	-$(RM) -rf dilithium_speed

