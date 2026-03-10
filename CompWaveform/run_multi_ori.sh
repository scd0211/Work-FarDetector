#!/bin/bash

Run=$1        # Run number
SubRun_Ini=$2 # subrun start
SubRun_Fin=$3 # subrun end

for SubRun in $(seq ${SubRun_Ini} ${SubRun_Fin})
do
	sh run_single.sh ${Run} ${SubRun}
done

