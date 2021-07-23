This repository accompanies the paper **Neon NTT: Faster Dilithium, Kyber, and Saber on Cortex-A72 and Apple M1** available at XXX.


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
