# Vcf2Matrix

Attempts to convert a VCF file into a matrix quickly.

## Notes
* The purpose is to maximise efficiency. This tool attempts to achieve this in the following ways:
	- Sequential read of VCF file ONCE (eg when parsing VCF file)
	- Multithreaded conversion
* When parsing the samples' names, Reader will look for the particular line (that entails the samples' names) and write directly.

## ToDo
* Make NULL to nullptr (in appropriate cases)
* Make number of threads created DYNAMIC
* To maximise efficiency for large writes, Writer should write whenever ~4K Bytes are ready be written.

## Issues
