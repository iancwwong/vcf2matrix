#!/usr/bin/python

import sys
from random import randint

# Check proper usage
if len(sys.argv) < 2:
	print "Usage: py rand_GT.py <num samples> <num SNPs>"
	sys.exit()

# Initiate variables and Genotype choices
numSamples = int(sys.argv[1])
numSNPs = int(sys.argv[2])
GT_options = ['0|0', '0|1', '1|0', '1|1']
numGT_options = len(GT_options)

# Generate random genotypes
for row in range(0,numSNPs):
	row_str = "";
	for col in range(0,numSamples):
		randGT = GT_options[randint(0,numGT_options-1)]
		row_str += randGT + "\t"
	print row_str.strip()