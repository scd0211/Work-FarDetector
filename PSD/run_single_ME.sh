#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

Runno=$1
subrun=$2


run=`printf "%06d" "$Runno"`

indir="/gpfs/home/mlf/cdshin/FarDetector/Analysis/CompWaveform/tree/ME/R${run}"
outdir="${WORKDIR}/PDF/ME/R${run}"

mkdir -p $outdir
	
root -l -b -q ${WORKDIR}/MakePDF_ME.C\(${Runno},${subrun},\"${indir}\",\"${outdir}\"\)





