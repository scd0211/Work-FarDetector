#!/bin/bash

Ptag=$1
Run=$2        # Run number
SubRun_Ini=$3 # subrun start
SubRun_Fin=$4 # subrun end
Mode=$5       # mode number

if [ -z "$Mode" ]; then
  echo "Usage: $0 RUN SUBRUN_INI SUBRUN_FIN MODE"
  echo "MODE:"
  echo "  1 = run_single_PDF.sh"
  echo "  2 = run_single_SCORE.sh"
  exit 1
fi

for SubRun in $(seq ${SubRun_Ini} ${SubRun_Fin})
do

  if [ "$Mode" -eq 1 ]; then
	sh run_single_PDF.sh ${Ptag} ${Run} ${SubRun}

  elif [ "$Mode" -eq 2 ]; then
    sh run_single_SCORE.sh ${Ptag} ${Run} ${SubRun}
  else
    echo "Unknown MODE: $Mode"
    exit 2
  fi

done

