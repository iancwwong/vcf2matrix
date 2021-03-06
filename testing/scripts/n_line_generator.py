#!/usr/bin/python

import sys
from random import randint

# Check proper usage
if len(sys.argv) < 2:
	print "Usage: py n_line_generator.py <num samples> <num SNPs>"
	sys.exit()

# Initiate variables and Genotype choices
numSamples = int(sys.argv[1])
numSNPs = int(sys.argv[2])
GT_options = ['0|0', '0|1', '1|0', '1|1']
numGT_options = len(GT_options)

# Write out the vcf header
print "##fileformat=VCFv4.2"
print "##fileDate=20090805"
print '##FORMAT=<ID=GT,Number=1,Type=String,Description="Genotype">'

# Build and write the content header
header_cols = ['#CHROM', 'POS', 'ID', 'REF', 'ALT', 'QUAL', 'FILTER', 'INFO', 'FORMAT']
row_str = "";
for data in header_cols:
	row_str += data + "\t"
for i in range(0, numSamples):
	sample_name = 'SAMPLE' + str(i+1)
	row_str += sample_name + "\t"
print row_str.strip()

# For now, have other fields to be set values
chrom = "1"
pos = "990417"
id = "rs2465136"
ref = "T"
alt = "C"
qual = "100"
filter = "PASS"
info = "."
format = "GT"
snp_info = [chrom, pos, id, ref, alt, qual, filter, info, format]

numLinesWritten = 0

# Generate random genotypes
for row in range(0,numSNPs):
	row_str = "";
	
	# Construct the SNP info
	for data in snp_info:
		row_str += data + "\t"
	
	# Append SNP sample genotypes
	for col in range(0,numSamples):
		#randGT = GT_options[randint(0,numGT_options-1)]
		randGT = GT_options[0]
		row_str += randGT + "\t"
	print row_str.strip()
	
	numLinesWritten += 1
