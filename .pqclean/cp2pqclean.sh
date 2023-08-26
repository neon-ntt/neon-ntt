#!/bin/bash
BASE=`dirname $0`
BASE=`cd ${BASE} && pwd`
WORK=work
WORKPATCHED=work-patched

rm -rf ${WORK} ${WORKPATCHED}
mkdir -p ${WORK} ${WORKPATCHED}

# patch implementations
for scheme in {kyber512,kyber768,kyber1024,dilithium2,dilithium3,dilithium5}
do
    cp -rL ${scheme} ${WORK}

    # delete non scheme files
    rm -rf ${WORK}/${scheme}/ntt ${WORK}/${scheme}/microbenchmarks

    # delete fips202.[ch]
    rm ${WORK}/${scheme}/scheme/fips202.[ch]

    # delete checksum files for dilithium
    rm -f ${WORK}/${scheme}/checksum*

    # mv .i to .inc
    mv $WORK/${scheme}/scheme/macros.i $WORK/${scheme}/scheme/macros.inc
    mv $WORK/${scheme}/scheme/macros_common.i $WORK/${scheme}/scheme/macros_common.inc

    # copy to work-patched
    cp -r $WORK/${scheme} ${WORKPATCHED}

    # apply patches
    cd ${WORKPATCHED}
    for X in ${BASE}/pqclean-patches/${scheme}/*
    do
        patch -s -p1 < ${X}
    done
    cd ..
done

# process and copy to PQClean
cd ${WORKPATCHED}
# kyber
for k in {2,3,4}; do
    ka=$((k*256));
    if [ $k -eq 2 ]
    then
        eta1=3
    else
        eta1=2
    fi



    if [ $k -eq 4 ]
    then
        polycomp=160
        polyvec=$((k*352))
    else
        polycomp=128
        polyvec=$((k*320))
    fi
    cp -rL kyber$ka/scheme/* ~/git/PQClean/crypto_kem/kyber$ka/aarch64/
    unifdef -m -DKYBER_K=$k -U KYBER_90S -DKYBER_POLYVECCOMPRESSEDBYTES=$polyvec -UPROFILE_HASHING -DKYBER_ETA1=$eta1 -DKYBER_ETA2=2 -DKYBER_POLYCOMPRESSEDBYTES=$polycomp -DKYBER_N=256 -DKYBER_INDCPA_MSGBYTES=32 ~/git/PQClean/crypto_kem/kyber$ka/aarch64/*.[ch]
    namespc="s/PQCLEAN_NAMESPACE/PQCLEAN_KYBER${ka}_AARCH64/g"
    sed -i $namespc ~/git/PQClean/crypto_kem/kyber$ka/aarch64/*.[chS]
done

# saber
# for k in {2,3,4}; do
#     if [ $k -eq 2 ]
#     then
#         scheme=lightsaber
#         mu=10
#         et=3
#     fi
#     if [ $k -eq 3 ]
#     then
#         scheme=saber
#         mu=8
#         et=4
#     fi
#     if [ $k -eq 4 ]
#     then
#         scheme=firesaber
#         mu=6
#         et=6
#     fi

#     cp -rL ${scheme}/scheme/* ~/git/PQClean/crypto_kem/${scheme}/aarch64
#     unifdef -m -DSABER_L=$k -DSABER_MU=$mu -DSABER_ET=$et -U__ARM_ARCH_8_3__ -UPROFILE_HASHING ~/git/PQClean/crypto_kem/${scheme}/aarch64/*.[ch]
#     namespc="s/PQCLEAN_NAMESPACE/PQCLEAN_${scheme^^}_AARCH64/g"
#     sed -i $namespc ~/git/PQClean/crypto_kem/${scheme}/aarch64/*.[chS]
# done

#dilithium
for k in {2,3,5}; do
    if [ $k -eq 2 ]
    then
        gamma2=95232
        gamma1=131072
    else
        gamma2=261888
        gamma1=524288
    fi
    if [ $k -eq 2 ]
    then
        eta=2
    fi
    if [ $k -eq 3 ]
    then
        eta=4
    fi
    if [ $k -eq 5 ]
    then
        eta=2
    fi

    scheme=dilithium$k
    cp -rL ${scheme}/scheme/* ~/git/PQClean/crypto_sign/${scheme}/aarch64
    unifdef -m -DDILITHIUM_Q=8380417 -DDILITHIUM_MODE=$k -DGAMMA1=$gamma1 -DGAMMA2=$gamma2 -DETA=$eta -UPROFILE_HASHING ~/git/PQClean/crypto_sign/${scheme}/aarch64/*.[ch]
    namespc="s/PQCLEAN_NAMESPACE/PQCLEAN_${scheme^^}_AARCH64/g"
    sed -i $namespc ~/git/PQClean/crypto_sign/${scheme}/aarch64/*.[chS]
done