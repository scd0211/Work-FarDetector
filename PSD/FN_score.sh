#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

Runno=$1
subrun=$2

MEPDF="${WORKDIR}/root/PDF/ME_beforeN2.root"
FNPDF="${WORKDIR}/root/PDF/FN_beforeN2.root"


run=`printf "%06d" "$Runno"`

indir="/gpfs/home/mlf/cdshin/FarDetector/Analysis/CompWaveform/tree/FN/R${run}"
outdir="${WORKDIR}/root/SCORE/FN/R${run}"

mkdir -p $outdir
	
root -l -b -q ${WORKDIR}/MakeScore_FN.C\(${Runno},${subrun},\"${MEPDF}\",\"${FNPDF}\",\"${indir}\",\"${outdir}\"\)





