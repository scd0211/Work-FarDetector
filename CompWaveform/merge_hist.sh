#!/bin/bash
source ~/setup.sh

#RUNS=(422 423)
#RUNS=(454)
#RUNS=(421 424 425 429 430 431 432 433 434)
RUNS=(446 447 448 450 453 465 477)

#BASE="/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/PDF/ME"
BASE="/gpfs/home/mlf/cdshin/FarDetector/Analysis/CompWaveform/hist"
#OUTDIR="${BASE}/ME_BeforeN2"
#OUTDIR="${BASE}/ME_AfterN2"
#OUTDIR="${BASE}/FN_BeforeN2"
#OUTDIR="${BASE}/FN_AfterN2"
OUTDIR="${BASE}/FN_AfterN2_woDVTX"
#OUTDIR="${BASE}/FN_AfterN2_acci"

#Target=${BASE}/ME_before_N2.root
#Target=${BASE}/ME_after_N2.root

#Target=${BASE}/FN_before_N2.root
Target=${BASE}/FN_after_N2_woDVTX.root
#Target=${BASE}/FN_after_N2_acci.root


MAX_LAST_DIGIT=6

mkdir -p "$OUTDIR"

for r in "${RUNS[@]}"; do
  NAME=$(printf "R%06d" "$r")
  DIR="${BASE}/${NAME}"
  OUT="${OUTDIR}/${NAME}.root"

  echo "=== Processing ${NAME} ==="

  if [ ! -d "$DIR" ]; then
    echo "Skip: directory not found -> $DIR"
    continue
  fi

  cd "$DIR" || continue

  files=""
  for f in S*.root; do
    [ -e "$f" ] || continue   # S*.root 없을 때 대비

    num=${f#S}
    num=${num%.root}
    last_digit=$(( 10#${num} % 10 ))

    if [ $last_digit -le $MAX_LAST_DIGIT ]; then
      files="$files $f"
    fi
  done

  if [ -n "$files" ]; then
    echo "hadd -> $OUT"
    hadd -f "$OUT" $files
  else
    echo "No files matched in $DIR"
  fi
done


hadd -f ${Target} ${OUTDIR}/*
