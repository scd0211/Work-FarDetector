#!/bin/bash

WorkDir=$(pwd)

snum=10

Ptag=2

# ME before N2
#RUN_LIST=(422 423)
#RUN_LIST=(454)

# FN before N2
#RUN_LIST=(421 424 425 429 430 431 432 433 434)
RUN_LIST=(446 447 448 450 453 465 477)
#RUN_LIST=(465 477)

find /home/mlf/cdshin/.lsf -mindepth 1 -maxdepth 1 -type f -delete

for Runno in "${RUN_LIST[@]}"
do
  run=$(printf "%06d" "$Runno")

  InDIR=/home/mlf/pjs/FAREC/input/R${run}
  FileName=tree_jsns2_far.r${run}
  pattern="${InDIR}/${FileName}.f"*.root

  files=( $pattern )

  if [ ${#files[@]} -eq 0 ]; then
    echo "[Run ${Runno}] No files found: ${pattern}"
    continue
  fi

  maxIndex=$(printf "%s\n" "${files[@]}" \
    | sed -e 's/.*f\([0-9]\+\)\.root/\1/' \
    | sort -n \
    | tail -1)

  maxIndex=$(expr "$maxIndex" + 0)

  SubRun_Ini=0
  SubRun_Fin=$maxIndex

  echo "[Run ${Runno}] SubRun ${SubRun_Ini} ~ ${SubRun_Fin}"

  nn=$(((SubRun_Fin-SubRun_Ini)/snum+1))
  echo "[Run ${Runno}] Number of jobs: $nn"

  for ((var=1; var<=nn; var++))
  do
    srun=$((SubRun_Ini + snum*(var-1)))
    erun=$((SubRun_Ini + snum*(var) - 1))

    if (( erun < SubRun_Ini )); then
      continue
    fi

    if (( srun < SubRun_Ini )); then
      srun=$SubRun_Ini
    fi

    if (( erun > SubRun_Fin )); then
      erun=$SubRun_Fin
    fi

    echo "Submitting Run${Runno}, Subrun ${srun} ~ ${erun}, extra=${PTAG} ${MODE}"

		bsub -q s "sh ${WorkDir}/run_multi.sh ${Ptag} ${Runno} ${srun} ${erun}"
  done

done

