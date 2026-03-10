#!/bin/bash

source ~/setup.sh

RUN=549
#RUN=683

rm -f png/*

for i in {0..300}; do
  echo "Running $i"
		root -l -b -q DrawWF.C\(${RUN},0,${i}\)
done
