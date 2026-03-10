#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

ptag=$1
Runno=$2
subrun=$3

#MEPDF="${WORKDIR}/root/PDF/ME_beforeN2.root"
#FNPDF="${WORKDIR}/root/PDF/FN_beforeN2.root"

MEPDF="${WORKDIR}/root/PDF/ME_afterN2.root"
FNPDF="${WORKDIR}/root/PDF/FN_afterN2.root"


DIR1="/gpfs/home/mlf/cdshin/FarDetector/Analysis/CompWaveform/tree"
DIR2="${WORKDIR}/root/SCORE"


#DIR1="/gpfs/home/mlf/cdshin/FarDetector/Analysis/CompWaveform/tree/Acci"
#DIR2="${WORKDIR}/root/SCORE/Acci"

run=`printf "%06d" "$Runno"`


  if [ "$ptag" -eq 1 ]; then
		indir="${DIR1}/ME/R${run}"
		outdir="${DIR2}/ME/R${run}"

  elif [ "$ptag" -eq 2 ]; then
		indir="${DIR1}/FN/R${run}"
		outdir="${DIR2}/FN/R${run}"
  else
    echo "Unknown MODE: $Mode"
    exit 2
  fi



mkdir -p $outdir
	
root -l -b -q ${WORKDIR}/MakeScore.C\(${ptag},${Runno},${subrun},\"${MEPDF}\",\"${FNPDF}\",\"${indir}\",\"${outdir}\"\)
#root -l -b -q ${WORKDIR}/MakeScoreAcci.C\(${ptag},${Runno},${subrun},\"${MEPDF}\",\"${FNPDF}\",\"${indir}\",\"${outdir}\"\)





