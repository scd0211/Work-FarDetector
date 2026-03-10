#!/bin/bash
source ~/setup.sh

OUTFILE=hist/FN_after_N2.root

FILES="
hist/R000446.root
hist/R000447.root
hist/R000448.root
hist/R000450.root
hist/R000453.root
"

hadd -f ${OUTFILE} ${FILES}

