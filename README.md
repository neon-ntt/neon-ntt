This repository accompanies the paper **Neon NTT: Faster Dilithium, Kyber, and Saber on Cortex-A72 and Apple M1** available at https://eprint.iacr.org/2021/986.


Authors: 
 - Hanno Becker `<hanno.becker@arm.com>`
 - Vincent Hwang `<vincentvbh7@gmail.com>`
 - [Matthias J. Kannwischer](https://kannwischer.eu/) `<matthias@kannwischer.eu>`
 - [Bo-Yin Yang](https://homepage.iis.sinica.edu.tw/pages/byyang/) `<by@crypto.tw>` 
 - Shang-Yi Yang `<nick.yang@chelpis.com>`

It contains our source code for Dilithium (`dilithium3`), Kyber (`kyber768`), and Saber (`saber`) optimized for Cortex-A72. 

# Benchmarking 

For each of the schemes, we provide three folders: 
- `ntt`: Code for the core polynomial arithmetic. 
- `microbenchmarks`: Standalone code for benchmarking individual functions; `make` will produce a benchmarking binary (when executed on an A72).
- `scheme`: Contains the entire code for the scheme; ready to be placed in supercop. 

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

