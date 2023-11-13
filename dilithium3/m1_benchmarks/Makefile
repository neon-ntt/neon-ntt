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
M1_CYCLES_HEADERS = $(CYCLES_PATH)/m1cycles.h
M1_CYCLES_SOURCES = $(CYCLES_PATH)/m1cycles.c
A72_CYCLES_HEADERS = $(CYCLES_PATH)/hal.h
A72_CYCLES_SOURCES = $(CYCLES_PATH)/hal.c
CYCLES_HEADERS = $(M1_CYCLES_HEADERS) $(A72_CYCLES_HEADERS)
CYCLES_SOURCES = $(M1_CYCLES_SOURCES) $(A72_CYCLES_SOURCES)

BENCHFLAGS = -I$(CYCLES_PATH) -DBENCH

ASM_HEADERs = macros.i macros_common.i
ASM_SOURCES = __asm_NTT.S __asm_iNTT.S __asm_poly.S

HASH_HEADERS += symmetric.h
HASH_SOURCES += symmetric-shake.c

HEADERS = params.h sign.h ntt.h packing.h poly.h polyvec.h reduce.h rounding.h
SOURCES = sign.c ntt.c packing.c poly.c polyvec.c reduce.c rounding.c

HEADERS += $(ASM_HEADERS) $(C_HEADERS) $(HASH_HEADERS) $(SORT_HEADERS) $(RAND_HEADERS)
SOURCES += $(ASM_SOURCES) $(C_SOURCES) $(HASH_SOURCES) $(SORT_SOURCES) $(RAND_SOURCES)

all: test speed

test: dilithium_test dilithium_testvectors

speed: dilithium_speed

dilithium_test: dilithium_test.c $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $< -o $@

dilithium_testvectors: dilithium_testvectors.c $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $< -o $@ -DNORAND

dilithium_speed: dilithium_speed.c $(HEADERS) $(SOURCES) $(M1_CYCLES_HEADERS) $(M1_CYCLES_SOURCES)
	$(CC) $(CFLAGS) $(ASM) $(SOURCES) $(BENCHFLAGS) $(M1_CYCLES_SOURCES) $< -o $@

.PHONY: clean

clean:
	-$(RM) -rf *.o
	-$(RM) -rf dilithium_test
	-$(RM) -rf dilithium_testvectors
	-$(RM) -rf dilithium_speed

