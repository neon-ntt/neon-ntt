
CFLAGS = -O3 -fomit-frame-pointer -march=native -mcpu=native \
	-Wall -Wextra -Wimplicit-function-declaration \
	-Wredundant-decls -Wmissing-prototypes \
	-Wundef -Wshadow

HEADERs = hal.h NTT_params.h ntt.h macros_common.i macros.i
SRCs = hal.c speed.c
ASMs = __asm_NTT.S __asm_iNTT.S __asm_poly.S

all: speed

speed: speed.c $(HEADERs) $(SRCs) $(ASMs)
	gcc -o speed -DBENCH $(CFLAGS) $(SRCs) $(ASMs)

.PHONY: clean
clean:
	rm -f *.h.gch
	rm -f speed

