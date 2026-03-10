#!/bin/bash
WORKDIR=`pwd`
source ~/setup.sh

run=422
end=1000

rm -f txt/*

for ((subrun=0; subrun<=end; subrun++)); do
	
root -l -b -q ME_pairing_self.C\(${run},${subrun}\)

done




