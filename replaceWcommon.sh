
rmln () {

rm -f $1
ln -s $2 $1

}

for x in dilithium2 dilithium3 dilithium5 kyber512 kyber768 kyber1024 lightsaber saber firesaber
do
    rmln $x/scheme/feat.S ../../common/feat.S
    rmln $x/scheme/fips202.h ../../common/fips202.h
    rmln $x/scheme/fips202.c ../../common/fips202.c
    rmln $x/scheme/fips202x2.h ../../common/fips202x2.h
    rmln $x/scheme/fips202x2.c ../../common/fips202x2.c
    rmln $x/m1_benchmarks/m1cycles.h ../../common/m1cycles.h
    rmln $x/m1_benchmarks/m1cycles.c ../../common/m1cycles.c
    rmln $x/m1_benchmarks/feat.S ../../common/feat.S
    rmln $x/m1_benchmarks/fips202.h ../../common/fips202.h
    rmln $x/m1_benchmarks/fips202.c ../../common/fips202.c
    rmln $x/m1_benchmarks/fips202x2.h ../../common/fips202x2.h
    rmln $x/m1_benchmarks/fips202x2.c ../../common/fips202x2.c
    rmln $x/microbenchmarks/hal.h ../../common/hal.h
    rmln $x/microbenchmarks/hal.c ../../common/hal.c
done

