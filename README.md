# Vcf2Matrix

Attempts to convert a VCF file into a matrix quickly.

## Notes
* Possible optimisations:
	- Using 'char *' instead of 'string' throughout program
	- Faster method of string tokenisation / splitting (therefore processing)
	- Read: using something other than ifstream, eg mmap
	- Set process to be high priority
	- Compile using OpenMP and pragmas
	- Enable Optimisations through g++ (-O4)
	
* Todo:
	- Parser should generate the subfiles in another directory (eg "temp")
	- Decrease memory usage (program reads entire vcf into memory at this stage). Maybe read the vcf partially?
	- Allele frequency needs to be calculated based on percentage of samples that possess the alternative allele
	
## Issues
* Program crashes at around 1M samples (even if there's only ONE SNP)
	
## VCF 4.2 Standard
Found at:
https://samtools.github.io/hts-specs/VCFv4.2.pdf
