#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

Runno=$1
subrun=$2

run=`printf "%06d" "$Runno"`

outdir="${WORKDIR}/tree/ME/R${run}"

mkdir -p $outdir
	
root -l -b -q ${WORKDIR}/MakeME.C\(${Runno},${subrun},\"${outdir}\"\)





