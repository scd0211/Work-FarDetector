#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

Runno=$1
subrun=$2

run=`printf "%06d" "$Runno"`

outdir="${WORKDIR}/tree/Acci/FN/R${run}"

mkdir -p $outdir
	
#root -l -b -q ${WORKDIR}/Make_FN_acci_WF_old.C\(${Runno},${subrun},\"${outdir}\"\)
root -l -b -q ${WORKDIR}/Draw_FN_acci_WF.C\(${Runno},${subrun}\)





