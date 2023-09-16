
CC = gcc-11

CFLAGS = -O3 -fomit-frame-pointer -march=native -mcpu=native \
	-Wall -Wextra -Wimplicit-function-declaration \
	-Wredundant-decls -Wmissing-prototypes \
	-Wundef -Wshadow

HEADERs = hal.h ntt.h NTT_params.h params.h  macros_common.i macros.i
SRCs = hal.c ntt.c
ASMs = __asm_NTT.S __asm_iNTT.S __asm_base_mul.S

all: test speed

test: test.c $(HEADERs) $(SRCs) $(ASMs)
	$(CC) -o $@ $< $(CFLAGS) $(SRCs) $(ASMs)

speed: speed.c $(HEADERs) $(SRCs) $(ASMs)
	$(CC) -o $@ $< $(CFLAGS) $(SRCs) $(ASMs)

.PHONY: clean
clean:
	rm -f *.h.gch
	rm -f test
	rm -f speed

