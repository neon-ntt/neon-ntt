This repository accompanies the paper **Neon NTT: Faster Dilithium, Kyber, and Saber on Cortex-A72 and Apple M1** available at https://eprint.iacr.org/2021/986.


Authors: 
 - Hanno Becker `<hanno.becker@arm.com>`
 - Vincent Hwang `<vincentvbh7@gmail.com>`
 - [Matthias J. Kannwischer](https://kannwischer.eu/) `<matthias@kannwischer.eu>`
 - [Bo-Yin Yang](https://homepage.iis.sinica.edu.tw/pages/byyang/) `<by@crypto.tw>` 
 - Shang-Yi Yang `<nick.yang@chelpis.com>`

It contains our source code for Dilithium, Kyber, and Saber optimized for Cortex-A72.
The code is also executable on other Armv8 cores and the Apple M1.
However, the benchmarking code in this repository has only been tested with the Cortex-A72.

# Installation

We have tested this code with a Raspberry Pi4 running Ubuntu 21.04.
It is essential that you are running a 64-bit OS to be able to execute aarch64 code.
The 64-bit Raspbian OS can be used, but we have not tested all code with it.

The Makefiles included in this repo assume that you are natively compiling your
code using gcc. We have tested it with gcc 10.3.0.

## Cloning the code
Clone the code together with all submodules

```
git clone --recurse-submodules https://github.com/neon-ntt/neon-ntt
```

## Enable access to the cycle counter

For accurate benchmarking, we make use of the performance counters.
By default the access from user mode is disabled. You will need to enable it using a kernel module.

We have included on here that you can install
```
cd enable_ccr
make install
```

Alternatively, you can get one from: https://github.com/rdolbeau/enable_arm_pmu.

You may have to install the kernel headers manually:
https://www.raspberrypi.com/documentation/computers/linux_kernel.html

```
sudo apt install raspberrypi-kernel-headers
```

In case the kernel headers can still not be found, you may have to change the
`uname -r` to `uname -m` in the Makefile.

# Benchmarking

For each of the schemes, we provide three folders:
- `ntt`: Code for the core polynomial arithmetic.
- `microbenchmarks`: Standalone code for benchmarking individual functions; `make` will produce a benchmarking binary (when executed on an A72). For running benchmarks, user space access to the PMU cycle count register needs to be enabled. A kernel module that enables it, can, for example, be found in [pqax](https://github.com/mupq/pqax#enable-access-to-performance-counters).
- `scheme`: Contains the entire code for the scheme; ready to be placed in supercop (see below)

```
.
├── LICENSE
├── README.md
├── dilithium2
│   ├── checksumbig
│   ├── checksumsmall
│   ├── microbenchmarks
│   ├── ntt
│   └── scheme
├── dilithium3
│   ├── checksumbig
│   ├── checksumsmall
│   ├── microbenchmarks
│   ├── ntt
│   └── scheme
├── dilithium5
│   ├── checksumbig
│   ├── checksumsmall
│   ├── microbenchmarks
│   ├── ntt
│   └── scheme
├── firesaber
│   ├── microbenchmarks
│   ├── ntt
│   └── scheme
├── gen_table
│   ├── Dilithium
│   ├── Kyber
│   ├── README.md
│   ├── Saber
│   ├── common -> multi-moduli-ntt-saber/gen_table/common/
│   └── multi-moduli-ntt-saber
├── kyber1024
│   ├── microbenchmarks
│   ├── ntt
│   └── scheme
├── kyber512
│   ├── microbenchmarks
│   ├── ntt
│   └── scheme
├── kyber768
│   ├── microbenchmarks
│   ├── ntt
│   └── scheme
├── lightsaber
│   ├── microbenchmarks
│   ├── ntt
│   └── scheme
└── saber
    ├── microbenchmarks
    ├── ntt
    └── scheme
```
## Benchmarking in SUPERCOP

The following instructions should allow to benchmark the full schemes using SUPERCOP on the Raspberry Pi4 running Ubuntu 21.04:
- Download https://bench.cr.yp.to/supercop/supercop-20210604.tar.xz
- Remove every line except the first from okcompilers/c and okcompilers/cpp to speed up benchmarking.
- Remove `#include <sys/sysctl.h>` from cpucycles/armv8.h
- Make sure that the access to the cycle counters from user mdoe is enabled before proceeding.
- Run `./do-part used` (this will take a couple of hours)
- Copy over the scheme you want
  - e.g., `cp -rL kyber768/scheme <SUPERCOP_PATH>/crypto_kem/kyber768/`
  - Note that for Dilithium the testvectors have changed: https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/BjfjRMIdnhM/m/W7kkVOFDBAAJ
  - You will have to copy over the updated testvectors
  - e.g., `cp dilithium2/checksum* <SUPERCOP_PATH>/crypto_sign/dilithium2`
- Benchmark using, e.g., `./do-part crypto_kem kyber768`
- Results will be in `bench/<hostname>/data`
- If you want to run more iterations, change `TIMINGS` in `crypto_{kem/sign}/measure.c` before running `./do-part crypto_{kem/sign} ...`


# Troubleshooting
## `Illegal instruction` when running benchmarks

Probably access to the PMU cycle counters from user space is not enabled. For enabling it, see https://github.com/mupq/pqax#enable-access-to-performance-counters.


# License

This repository includes code from other sources that has the following license/license waivers
- Kyber reference code https://github.com/pq-crystals/kyber/blob/master/LICENSE: CC0
- Saber reference code https://github.com/KULeuven-COSIC/SABER/blob/master/LICENSE: public domain
- Dilithium reference code https://github.com/pq-crystals/dilithium/blob/master/LICENSE: CC0
- fips202.{c,h} http://bench.cr.yp.to/supercop.html: public domain
- fips202x2.{c,h} https://github.com/cothan/kyber/blob/master/neon/fips202x2.c: CC0

All remaining code is covered by CC0.