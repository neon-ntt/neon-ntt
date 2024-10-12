
CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
          -Wshadow -Wpointer-arith -fomit-frame-pointer -mtune=native -O3
NISTFLAGS += -Wno-unused-result -mpopcnt -maes \
             -mtune=native -O3 -fomit-frame-pointer
RM = /bin/rm



HASH_PATH = ../../common
HASH_HEADERS = $(HASH_PATH)/fips202.h $(HASH_PATH)/fips202x2.h
HASH_SOURCES = $(HASH_PATH)/fips202.c $(HASH_PATH)/fips202x2.c $(HASH_PATH)/feat.S

SORT_PATH = .
SORT_HEADERS =
SORT_SOURCES =

RAND_PATH = ../../common
RAND_HEADERS = $(RAND_PATH)/randombytes.h $(RAND_PATH)/rng.h $(RAND_PATH)/chacha20.h
RAND_SOURCES = $(RAND_PATH)/randombytes.c $(RAND_PATH)/rng.c $(RAND_PATH)/chacha20.c

CFLAGS += -I$(HASH_PATH) -I$(SORT_PATH) -I$(RAND_PATH)

CYCLES_PATH = ../../common
CYCLES_HEADERS = $(CYCLES_PATH)/cycles.h
CYCLES_SOURCES = $(CYCLES_PATH)/cycles.c

BENCHFLAGS = -I$(CYCLES_PATH) -DBENCH

ASM_HEADERs = macros.i macros_common.i
ASM_SOURCES = __asm_NTT.S __asm_iNTT.S  __asm_mul.S __asm_narrow.S __asm_pack_unpack.S

ASM = __asm_NTT.S __asm_iNTT.S __asm_mul.S __asm_narrow.S __asm_pack_unpack.S feat.S
SOURCES = SABER_indcpa.c kem.c randombytes.c pack_unpack.c verify.c cbd.c
SOURCESKECCAK = $(SOURCES) fips202.c fips202x2.c
HEADERS = SABER_params.h NTT_params.h NTT.h pack_unpack.h verify.h cbd.h SABER_indcpa.h kem.h randombytes.h
HEADERSKECCAK = $(HEADERS) fips202.h fips202x2.h macros.i macros_common.i

HEADERS = SABER_params.h NTT_params.h NTT.h pack_unpack.h verify.h cbd.h SABER_indcpa.h kem.h
SOURCES = SABER_indcpa.c kem.c pack_unpack.c verify.c cbd.c

HEADERS += $(ASM_HEADERS) $(C_HEADERS) $(HASH_HEADERS) $(SORT_HEADERS) $(RAND_HEADERS)
SOURCES += $(ASM_SOURCES) $(C_SOURCES) $(HASH_SOURCES) $(SORT_SOURCES) $(RAND_SOURCES)

all: test speed

test: saber_test

speed: saber_speed

saber_test: saber_test.c $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $< -o $@

saber_speed: saber_speed.c $(HEADERS) $(SOURCES) $(CYCLES_HEADERS) $(CYCLES_SOURCES)
	$(CC) $(CFLAGS) $(BENCHFLAGS) $(SOURCES) $(CYCLES_SOURCES) $< -o $@

.PHONY: clean

clean:
	-$(RM) -f *.o
	-$(RM) -f saber_test
	-$(RM) -f saber_speed

