#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

Runno=$1
subrun=$2

run=`printf "%06d" "$Runno"`

outdir="${WORKDIR}/tree/ME/R${run}"

mkdir -p $outdir
	
root -l -b -q ${WORKDIR}/Make_ME_WF_old.C\(${Runno},${subrun},\"${outdir}\"\)
root -l -b -q ${WORKDIR}/Draw_ME_WF.C\(${Runno},${subrun}\)





