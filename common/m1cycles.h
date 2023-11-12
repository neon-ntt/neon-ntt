#ifndef M1CYCLES_H
#define M1CYCLES_H

#ifdef __APPLE__

void setup_rdtsc(void);
unsigned long long int rdtsc(void);

#endif

#endif
