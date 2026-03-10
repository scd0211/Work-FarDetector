#!/bin/bash

WorkDir=`pwd`

snum=10

Runno=$1

run=`printf "%06d" "$Runno"`

InDIR=/home/mlf/pjs/FAREC/output/R${run}
FileName=Reco.tree_jsns2_far.r${run}

pattern="${FileName}.f*.root"

files=($(ls ${InDIR}/${pattern} 2>/dev/null))

	if [ ${#files[@]} -eq 0 ]; then
	echo "No files found"
	exit 1
	fi

	maxIndex=$(printf "%s\n" "${files[@]}" \
			| sed -e 's/.*f\([0-9]\+\)\.root/\1/' \
			| sort -n \
			| tail -1)

	maxIndex=$(expr "$maxIndex" + 0)

	echo $maxIndex


SubRun_Ini=0
SubRun_Fin=$maxIndex
	
	echo "Runno "${Runno}" / SubRun"${SubRun_Ini}"~"${SubRun_Fin}

nn=$(((SubRun_Fin-SubRun_Ini)/snum+1))

echo "$nn"

rm -f /home/mlf/cdshin/.lsf/*

for((var=1 ; var <= ${nn} ; var++));
	do
		srun=$((SubRun_Ini+snum*(var-1)))
		erun=$((SubRun_Ini+snum*(var)-1))

	if (( $erun < $SubRun_Ini  ))
	then
		continue
	fi

	if (( $srun < $SubRun_Ini ))
	then
		srun=$SubRun_Ini
	fi

	if (( $erun > $SubRun_Fin ))
	then
		erun=$SubRun_Fin
	fi

	echo "Submitting Run${Runno}, Subrun ${srun} ~ ${erun}"


bsub -q s "sh ${WorkDir}/run_multi.sh ${Runno} ${srun} ${erun}"

done 






