#!/bin/bash
BASE=`dirname $0`
BASE=`cd ${BASE} && pwd`

PATCHES=${BASE}/pqclean-patches
WORK=work
WORKPATCHED=work-patched

if [ ! -e "${WORK}" ]
then
  echo "${WORK} not found"
  exit
fi

rm -rf ${PATCHES}
mkdir -p ${PATCHES}/tmp/
for scheme in kyber512 kyber768 kyber1024 lightsaber saber firesaber dilithium2 dilithium3 dilithium5}
do
  echo $scheme;
  V1=${WORK}/${scheme}/scheme/
  if [ ! -e "${V1}" ]
  then
    echo "${V1} not found"
    exit
  fi
  V2=${WORKPATCHED}/${scheme}/scheme
  if [ ! -e "${V2}" ]
  then
    echo "${V2} not found"
    exit
  fi
  mkdir -p ${PATCHES}/tmp/${scheme}
  mkdir -p ${PATCHES}/${scheme}
  diff -ruN --no-dereference ${V1} ${V2} > ${PATCHES}/tmp/${scheme}/p

  ( cd ${PATCHES}/tmp/${scheme}
    splitpatch ${PATCHES}/tmp/${scheme}/p
    rm ${PATCHES}/tmp/${scheme}/p
    for X in *
    do
    sed -i '1,2 s/\t.*$//' ${X}
    sed -i '$ s/diff.*//' ${X}
    done
  )
  mv ${PATCHES}/tmp/${scheme}/* ${PATCHES}/${scheme}/

done

rm -rf ${PATCHES}/tmp/

