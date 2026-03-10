#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

Runno=$1
subrun=$2

run=`printf "%06d" "$Runno"`

outdir="${WORKDIR}/tree/Single/R${run}"

mkdir -p $outdir
	
root -l -b -q ${WORKDIR}/Make_Single.C\(${Runno},${subrun},\"${outdir}\"\)





