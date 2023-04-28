
rmln () {

rm -f $2
ln -s $1 $2

}

for x in dilithium2 dilithium3 dilithium5 kyber512 kyber768 kyber1024 lightsaber saber firesaber
do
    rmln ../../common/feat.S $x/scheme/feat.S
    rmln ../../common/fips202.h $x/scheme/fips202.h
    rmln ../../common/fips202.c $x/scheme/fips202.c
    rmln ../../common/fips202x2.h $x/scheme/fips202x2.h
    rmln ../../common/fips202x2.c $x/scheme/fips202x2.c
    rmln ../../common/m1cycles.h $x/m1_benchmarks/m1cycles.h
    rmln ../../common/m1cycles.c $x/m1_benchmarks/m1cycles.c
    rmln ../../common/feat.S $x/m1_benchmarks/feat.S
    rmln ../../common/fips202.h $x/m1_benchmarks/fips202.h
    rmln ../../common/fips202.c $x/m1_benchmarks/fips202.c
    rmln ../../common/fips202x2.h $x/m1_benchmarks/fips202x2.h
    rmln ../../common/fips202x2.c $x/m1_benchmarks/fips202x2.c
    rmln ../../common/randombytes.h $x/m1_benchmarks/randombytes.h
    rmln ../../common/randombytes.c $x/m1_benchmarks/randombytes.c
    rmln ../../common/hal.h $x/microbenchmarks/hal.h
    rmln ../../common/hal.c $x/microbenchmarks/hal.c
done

