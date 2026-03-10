#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

Runno=$1
subrun=$2

run=`printf "%06d" "$Runno"`

outdir="${WORKDIR}/tree/FN/R${run}"

mkdir -p $outdir
	
root -l -b -q ${WORKDIR}/MakeFN.C\(${Runno},${subrun},\"${outdir}\"\)





