CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
          -Wshadow -Wpointer-arith -fomit-frame-pointer -mtune=native -O3
NISTFLAGS += -Wno-unused-result -mpopcnt -maes \
             -mtune=native -O3 -fomit-frame-pointer
RM = /bin/rm

ASM = __asm_NTT.S __asm_iNTT.S __asm_mul.S __asm_narrow.S __asm_pack_unpack.S
SOURCES = SABER_indcpa.c kem.c randombytes.c pack_unpack.c verify.c cbd.c
SOURCESKECCAK = $(SOURCES) fips202.c fips202x2.c
HEADERS = SABER_params.h NTT_params.h NTT.h pack_unpack.h verify.h cbd.h SABER_indcpa.h kem.h randombytes.h
HEADERSKECCAK = $(HEADERS) fips202.h fips202x2.h macros.i macros_common.i

all: test speed

test: saber_test

speed: saber_speed

feat.o: feat.s
	$(CC) -c $< -o $@ $(CFLAGS) -march=armv8.2-a+sha3

saber_test: $(ASM) $(SOURCESKECCAK) $(HEADERSKECCAK) feat.o saber_test.c
	$(CC) $(CFLAGS) $(ASM) $(SOURCESKECCAK) feat.o saber_test.c -o saber_test

saber_speed: $(ASM) $(SOURCESKECCAK) $(HEADERSKECCAK) feat.o saber_speed.c
	$(CC) $(CFLAGS) $(ASM) $(SOURCESKECCAK) feat.o m1cycles.c saber_speed.c -o saber_speed

.PHONY: bench clean

clean:
	-$(RM) -f *.o
	-$(RM) -f saber_test
	-$(RM) -f saber_speed

