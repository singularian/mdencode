# mdencode
A message digest signature generator for files with modular floors  
mdencode is written in golang and uses an interface for pluggin formatters  
mdencode allows for the creation of sqlite3 db file signatures  

# Overview

This is a golang command line file signature generator with a modular floor.  
This program can generate a file signature at the file or block level and format and write the output in different formats.  
It can write to an sqlite db. The mdencode sqlite db files contain a table schema and can be exported into multiple formats.  

# Output Formats

1. TEXT
2. INFORM
3. XML
4. JSON
5. BINARY
6. CSV
7. SQLITE3 DB
8. BUILD YOUR OWN

# Digital Signature List

This is a partial list of the digital signatures. More can be added to the signature chain and are selectable via a binary flag argument for file blocks and files

1. md4
2. md5
3. sha
4. sha3
5. Tiger
6. Whirlpool




# Mdencode usage

Usage of md:
     
  -bh string  
     Block Hash Boolean String List (default "011")   
  -fh string  
     File Hash Boolean String List (default "011")         
  -block string  
     File Block Size Bytes (default "40")    
  -mod string  
     Modulus Size in Bits (default "32")       
  -append  
     Append To Output File     
  -file string  
     Input filename       
  -format int  
     Output Format (default 4)      
  -key string  
     Signature Key (Minimum 16 bytes for siphash) (default "LomaLindaSanSerento9000")  
  -line  
     File Hash as one line  
  -log string  
     Log Filename  
 -byte  
     Generate the File Byteblock      
  -out string  
     Output Filename  

Version: 1.0 復甦 復活

Output Formats  
0 - 98 - Text  
99 - Inform  
100 - XML    
101 - CSV  
102 - CSV  
600 - Json  
1000 - Binary  
2000 - SQL Lite 3 DB File  

# Command Line Examples

md -file=Makefile -block=100 -line=false -bh=0 -fh=1111111 -format=19 -line

md -file=Makefile -block=100 -line=false -bh=111 -fh=0 -format=19 -line

md -file=Makefile -block=100 -bh=111111 -fh=1111111 -format=19 -line=true -out=outputfile

md -file=Makefile -block=100 -bh=111111 -fh=1111111 -format=19 -append=true -line=true -out=outputfile

md -block=1000 -mod=256 -bh=001100 -fh=100100111 -format=100 -file=destiny.txt

# md example usage

This is a file signature for Makefile with a block size of 100 bytes and a 128 bit modulus and the fileblock signature bit string and file bit string and a format specified by number 10. It is formatted by go flags args.

md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=9 -file=america.txt -out=america.txt_b100_mod128_f9_.md

https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_f9_.md

# md example usage of the input file Makefile and the output format json

md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=600 -file=america.txt -out=america.txt_b100_mod128_f600_.md

https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_f600_.md

# mddbreport usage
This example shows how to generate an sql lite file db signature

md -file=Makefile -format=2000 -line=true -out=ssss

It can also generate a formatted signature from the ssss.db sqlite 3 file.

nsross@nsross-desktop:~/projects/src/github.com/singularian/mdencode/code/encode$ mddbreport ssss.db

file 1: /home/nsross/projects/src/github.com/singularian/mdencode/code/encode/ Makefile 1.0

# mddbreport text format example

nsross@nsross-desktop:~/projects/src/github.com/singularian/mdencode/code/encode$ mddbreport ssss.db 1

https://raw.githubusercontent.com/singularian/mdencode/master/examples/ssss.db.txt

# mddbreport json format example

nsross@nsross-desktop:~/projects/src/github.com/singularian/mdencode/code/encode$ mddbreport ssss.db 1 600

https://raw.githubusercontent.com/singularian/mdencode/master/examples/ssss.db.json

# mddbreport xml format example

https://raw.githubusercontent.com/singularian/mdencode/master/examples/ssss.db.xml

# License 

https://github.com/singularian/mdencode/blob/master/LICENSE
