#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define ARRAY_N 256

#define NTT_N 256
#define LOGNTT_N 8

#define Q 8380417
#define omegaQ 1753
// omegaQ^{-1} mod Q1
#define invomegaQ (731434)

// R = 2^32 below
// R mod^+- Q
#define RmodQ (-4186625)
// round( RmodQ * R / (2 * Q) )
#define RmodQ_hi (-107282355)
// Qprime = -Q^(-1) mod^+- R
#define Qprime (-58728449)

// NTT_N^(-1) * R mod^+- Q
#define FINAL_SCALE (16382)
#define FINAL_SCALE_hi (4197891)
// invomegaQ * FINAL_SCALE mod^+- Q
#define FINAL_SCALE_F (-294725)
#define FINAL_SCALE_F_hi (-75523344)

#endif











