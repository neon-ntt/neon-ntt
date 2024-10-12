
CC = gcc

CFLAGS += -Wall -Wextra -Wmissing-prototypes -Wredundant-decls \
          -Wshadow -O3 -march=armv8-a

HEADERs = NTT_params.h tools.h naive_mult.h gen_table.h
SOURCEs = tools.c naive_mult.c gen_table.c

all: call_gen

call_gen: call_gen.c $(HEADERs) $(SOURCEs)
	$(CC) $(CFLAGS) $(SOURCEs) $< -o $@

.PHONY: clean
clean:
	rm -f call_gen

