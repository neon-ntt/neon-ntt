CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
          -Wshadow -Wpointer-arith -fomit-frame-pointer -mtune=native -O3
NISTFLAGS += -Wno-unused-result -mpopcnt -maes \
             -mtune=native -O3 -fomit-frame-pointer
RM = /bin/rm

HASH_PATH = ../../common
HASH_HEADERS = $(HASH_PATH)/fips202.h
HASH_SOURCES = $(HASH_PATH)/fips202.c

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

ASM_HEADERs =
ASM_SOURCES =

HASH_HEADERS += symmetric.h
HASH_SOURCES += symmetric-shake.c

HEADERS = api.h config.h params.h sign.h ntt.h packing.h poly.h polyvec.h reduce.h rounding.h
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

dilithium_speed: dilithium_speed.c $(HEADERS) $(SOURCES) $(CYCLES_HEADERS) $(CYCLES_SOURCES)
	$(CC) $(CFLAGS) $(ASM) $(SOURCES) $(BENCHFLAGS) $(CYCLES_SOURCES) $< -o $@

.PHONY: clean

clean:
	-$(RM) -rf *.o
	-$(RM) -rf dilithium_test
	-$(RM) -rf dilithium_testvectors
	-$(RM) -rf dilithium_speed

