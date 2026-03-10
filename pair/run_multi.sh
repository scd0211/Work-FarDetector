#!/bin/bash

Ptag=2
Run=$1        # Run number
SubRun_Ini=$2 # subrun start
SubRun_Fin=$3 # subrun end

if [ -z "$Ptag" ]; then
  echo "Usage: $0 RUN SUBRUN_INI SUBRUN_FIN MODE"
  echo "MODE:"
  echo "  1 = run_single_PDF.sh"
  echo "  2 = run_single_SCORE.sh"
  exit 1
fi

for SubRun in $(seq ${SubRun_Ini} ${SubRun_Fin})
do

  if [ "$Ptag" -eq 1 ]; then
	sh run_single_ME.sh ${Run} ${SubRun}

  elif [ "$Ptag" -eq 2 ]; then
	sh run_single_FN.sh ${Run} ${SubRun}
  else
    echo "Unknown MODE: $Ptag"
    exit 2
  fi

done

