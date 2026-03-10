#!/bin/bash
source ~/setup.sh

#RUNS=(421 424 425 429 430 431 432 433 434)
#BASE="/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/PDF/FN"

#RUNS=(422 423)
#BASE="/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/PDF/ME"

#RUNS=(454)
#BASE="/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/PDF/ME"

RUNS=(446 447 448 450 453 465 477)
BASE="/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/PDF/FN"

#OUTDIR="${BASE}/BeforeN2"
OUTDIR="${BASE}/AfterN2"
rm -rf ${OUTDIR}

MAX_LAST_DIGIT=6
CHUNK_SIZE=50          # 한번에 hadd할 파일 개수 (환경에 맞게 5~30 조절)
QUEUE="s"              # bsub -q s
LOGDIR="${OUTDIR}/log"

mkdir -p "$OUTDIR" "$LOGDIR"

for r in "${RUNS[@]}"; do
  NAME=$(printf "R%06d" "$r")
  DIR="${BASE}/${NAME}"
  FINAL="${OUTDIR}/${NAME}.root"
  TMPDIR="${OUTDIR}/tmp_${NAME}"

  echo "=== Processing ${NAME} ==="

  if [ ! -d "$DIR" ]; then
    echo "Skip: directory not found -> $DIR"
    continue
  fi

  mkdir -p "$TMPDIR"

  # 대상 파일 리스트 만들기
  cd "$DIR" || continue
  files=()
  for f in S*.root; do
    [ -e "$f" ] || continue
    num=${f#S}; num=${num%.root}
    last_digit=$(( 10#${num} % 10 ))
    if [ $last_digit -le $MAX_LAST_DIGIT ]; then
      files+=("$DIR/$f")
    fi
  done

  if [ ${#files[@]} -eq 0 ]; then
    echo "No files matched in $DIR"
    continue
  fi

  # chunk로 나눠서 bsub 제출
  njob=0
  chunk_files=()
  chunk_id=0

  submit_one_chunk () {
    local cid=$1
    shift
    local out="${TMPDIR}/chunk_${cid}.root"
    local log="${LOGDIR}/${NAME}_chunk_${cid}.log"
    bsub -q "$QUEUE" -J "hadd_${NAME}_${cid}" -o "$log" \
      "source ~/setup.sh; hadd -f '$out' $*"
  }

  for f in "${files[@]}"; do
    chunk_files+=("$f")
    if [ ${#chunk_files[@]} -ge $CHUNK_SIZE ]; then
      submit_one_chunk "$chunk_id" "${chunk_files[@]}"
      chunk_files=()
      chunk_id=$((chunk_id+1))
      njob=$((njob+1))
    fi
  done
  # 남은 chunk
  if [ ${#chunk_files[@]} -gt 0 ]; then
    submit_one_chunk "$chunk_id" "${chunk_files[@]}"
    njob=$((njob+1))
  fi

  echo "Submitted $njob chunk jobs for $NAME"

  # 최종 merge job (chunk job들이 끝난 뒤 실행)
  # -w "ended(jobname)" 조건을 OR로 연결
  dep=""
  for i in $(seq 0 $((njob-1))); do
    jn="hadd_${NAME}_${i}"
    if [ -z "$dep" ]; then
      dep="ended(${jn})"
    else
      dep="${dep} && ended(${jn})"
    fi
  done

  final_log="${LOGDIR}/${NAME}_final.log"
  bsub -q "$QUEUE" -J "hadd_${NAME}_final" -w "$dep" -o "$final_log" \
    "source ~/setup.sh; hadd -f '$FINAL' ${TMPDIR}/chunk_*.root"

done

