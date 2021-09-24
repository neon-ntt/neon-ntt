
CFLAGS += -Wall -Wextra -Wmissing-prototypes -Wredundant-decls \
          -Wshadow -mtune=native -O3

HEADERS = NTT_params.h tools.h gen_table.h
SRCs = tools.c gen_table.c

all: $(HEADERS) $(SRCs) call_gen.c
	gcc -o gen_table $(CFLAGS) $(SRCs) call_gen.c

.PHONY: clean
clean:
	rm -f gen_table