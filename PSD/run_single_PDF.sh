#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

ptag=$1
Runno=$2
subrun=$3

run=`printf "%06d" "$Runno"`


  if [ "$ptag" -eq 1 ]; then
		indir="/gpfs/home/mlf/cdshin/FarDetector/Analysis/CompWaveform/tree/ME/R${run}"
		outdir="${WORKDIR}/root/PDF/ME/R${run}"

  elif [ "$ptag" -eq 2 ]; then
		indir="/gpfs/home/mlf/cdshin/FarDetector/Analysis/CompWaveform/tree/FN/R${run}"
		outdir="${WORKDIR}/root/PDF/FN/R${run}"
  else
    echo "Unknown MODE: $Mode"
    exit 2
  fi


mkdir -p $outdir
	
root -l -b -q ${WORKDIR}/MakePDF.C\(${ptag},${Runno},${subrun},\"${indir}\",\"${outdir}\"\)





