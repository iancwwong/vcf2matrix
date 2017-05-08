# Vcf2Matrix

Attempts to convert a VCF file into a matrix quickly.

## Notes
* Possible optimisations:
	- Using 'char *' instead of 'string' throughout program
	- Read: using something other than ifstream
* Todo:
	- Decrease memory usage (program reads entire vcf into memory at this stage)
	
## Issues
* Multithreaded write crashes the program
	- try to have each converter thread writing to a subfile, concatenating all the created subfiles into final files, then removing the subfiles
	
## VCF 4.2 Standard
Found at:
https://samtools.github.io/hts-specs/VCFv4.2.pdf
