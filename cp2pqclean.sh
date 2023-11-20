
if [[ "$OSTYPE" = "darwin"* ]]
then
    SED=gsed
else
    SED=sed
fi

TMP=tmp

PQCleanPATH=~/Desktop/git/public/PQClean

unifdef_inplace_kyber() {
unifdef -UPROFILE_HASHING -DKYBER_ETA2=2 -DKYBER_N=256 -DKYBER_INDCPA_MSGBYTES=32 $1 > tmp
mv tmp $1
rm -f tmp
}

unifdef_inplace_dilithium() {
unifdef -DDILITHIUM_Q=8380417 -UPROFILE_HASHING $1 > tmp
mv tmp $1
rm -f tmp
}

for i in kyber512 kyber768 kyber1024
do
    rm -rf $TMP
    mkdir $TMP
    cp $i/scheme/* $TMP
    cp common/feat.S $TMP/
    cp common/fips202x2.h $TMP/
    cp common/fips202x2.c $TMP/
    cp LICENSE $TMP/LICENSE

    cd $TMP
    $SED -i "s/macros.i/macros.inc/g" *.[chS]
    $SED -i "s/macros_common.i/macros_common.inc/g" *.[ichS]
    mv macros.i macros.inc
    mv macros_common.i macros_common.inc

    if [ $i = kyber512 ]
    then
        namespc="s/KYBER_AARCH64/PQCLEAN_KYBER512_AARCH64/g"
    elif [ $i = kyber768 ]
    then
        namespc="s/KYBER_AARCH64/PQCLEAN_KYBER768_AARCH64/g"
    elif [ $i = kyber1024 ]
    then
        namespc="s/KYBER_AARCH64/PQCLEAN_KYBER1024_AARCH64/g"
    fi

    $SED -i $namespc *.[chS]

    for j in *.c *.h
    do
        unifdef_inplace_kyber $j
    done

    cp * $PQCleanPATH/crypto_kem/$i/aarch64/

    cd ../
done

for i in dilithium2 dilithium3 dilithium5
do
    rm -rf $TMP
    mkdir $TMP
    cp $i/scheme/* $TMP
    cp common/feat.S $TMP/
    cp common/fips202x2.h $TMP/
    cp common/fips202x2.c $TMP/
    cp LICENSE $TMP/LICENSE

    cd $TMP
    $SED -i "s/macros.i/macros.inc/g" *.[chS]
    $SED -i "s/macros_common.i/macros_common.inc/g" *.[ichS]
    mv macros.i macros.inc
    mv macros_common.i macros_common.inc


    if [ $i = dilithium2 ]
    then
        namespc="s/DILITHIUM_AARCH64/PQCLEAN_DILITHIUM2_AARCH64/g"
    elif [ $i = dilithium3 ]
    then
        namespc="s/DILITHIUM_AARCH64/PQCLEAN_DILITHIUM3_AARCH64/g"
    elif [ $i = dilithium5 ]
    then
        namespc="s/DILITHIUM_AARCH64/PQCLEAN_DILITHIUM5_AARCH64/g"
    fi

    $SED -i $namespc *.[chS]

    for j in *.c *.h
    do
        unifdef_inplace_dilithium $j
    done

    cp * $PQCleanPATH/crypto_sign/$i/aarch64/

    cd ../
done

rm -rf $TMP

