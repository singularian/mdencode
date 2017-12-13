# mdencode :octocat:
A message digest signature generator for files with modular floors  
mdencode is written in golang and uses an interface for pluggin formatters  
mdencode allows for the creation of sqlite3 db file signatures  

# Overview

This is a golang command line file signature generator with a modular floor.  
This program can generate a file signature at the file or block level and format and write the output in different formats.  
It can write to an sqlite db. The mdencode sqlite db files contain a table schema and can be exported into multiple formats.  

# Features

- Versioned Cryptographic File Signatures
- Optional File Crypto Signatures
- Optional N-Bit Sized File Block Modular Floors
- Optional N-Byte Block Crypto Signatures
- SQLite3 DB Crypto Signature Files
- Plugin Formatters
- Multiple Selectable Crypto Hash Signatures for Files and File Blocks

# Output Formats

- TEXT
- INFORM
- XML
- JSON
- BINARY
- CSV
- SQLITE3 DB
- BUILD YOUR OWN

# Digital Signature List

This is a partial list of the digital signatures. More can be added to the signature chain and are selectable via a binary flag argument for file blocks and files

- fnv
- md4
- md5
- ripe160
- sha
- sha3
- skein
- Tiger
- Whirlpool


# Modular Floor

This program uses a modular floor paired with a package of digital signatures. This makes the digital signature more unique. A modular floor consists of a polynomial
which is the modular exponent plus n times the modulus plus the remainder. It is possible to use a modular scan to find matching byte blocks associated with the digital signature package.
A digital signature package or group can consist of 1 or more block and file signatures. A modular floor is generaly computed at the block level for a file byte block and utilizes a big integer.

This is an example of the modular floor.  
```
$ ./decoder4  
  
modulo bigint  16777216  
modulo floor  35184372088832  
modulo ceil  70368744177664  
Found Block  [35 32 84 104 105 115]   [35 32 84 104 105 115]  
total time  2.438805771s  
modulo bigint  16777216  
modulo floor  35184372088832  
modulo ceil  70368744177664  
Found Block  [32 110 97 109 101 32]   [32 110 97 109 101 32]  
total time  2.200462445s  
```

# Mdencode usage

```
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
     Append the File Byteblock to the Hash List  
  -blockint  
     Append the File Byteblock Bigint to the Hash List  
  -out string  
     Output Filename  

Version: 1.0 復甦 復活
```

# Output Formats
Output Formats  
0 - 98 - Text  
99 - Inform  
100 - XML    
101 - CSV  
102 - CSV  
600 - Json  
1000 - Binary  
2000 - SQL Lite 3 DB File  

# Go Library Dependencies

go get -u golang.org/x/crypto/blake2s  
go get -u golang.org/x/crypto/md4  
go get -u golang.org/x/crypto/sha3  
go get -u golang.org/x/crypto/ripemd160  
go get -u github.com/steakknife/keccak  
go get -u github.com/asadmshah/murmur3  
go get -u github.com/dchest/siphash  
go get -u github.com/aead/skein  
go get -u github.com/cxmcc/tiger  
go get -u github.com/jzelinskie/whirlpool  
go get -u github.com/mattn/go-sqlite3  

# Command Line Examples

```
md -file=Makefile -block=100 -line=false -bh=0 -fh=1111111 -format=19 -line
md -file=Makefile -block=100 -line=false -bh=111 -fh=0 -format=19 -line
md -file=Makefile -block=100 -bh=111111 -fh=1111111 -format=19 -line=true -out=outputfile
md -file=Makefile -block=100 -bh=111111 -fh=1111111 -format=19 -append=true -line=true -out=outputfile
md -file=Makefile -block=100 -bh=111100001111 -fh=11011100001 -format=600 -append=true -line=true -out=outputfile
md -block=1000 -mod=256 -bh=001100 -fh=100100111 -format=100 -file=destiny.txt
```

# Eample usage

This is a file signature for Makefile with a block size of 100 bytes and a 128 bit modulus and the fileblock signature bit string and file bit string and a format specified by number 10. It is formatted by go flags args.

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=9 -file=america.txt -out=america.txt_b100_mod128_f9_.md
```

[Text Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_f9_.md)

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=100 -file=america.txt -out=america.txt_b100_mod128_f100_.md
```

[XML Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_f100_.md)

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=600 -file=america.txt -out=america.txt_b100_mod128_f600_.md
```

[JSON Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_f600_.md)

# Example mddbreport usage
This example shows how to generate an sql lite file db signature

```
md -file=Makefile -format=2000 -line=true -out=ssss
```

It can also generate a formatted signature from the ssss.db sqlite 3 file

This example shows how to list the files in a ssss.db file sqlite3 signature db

The columns are:

file id, filepath, filename, version

```
$ mddbreport ssss.db

file 1: /projects/src/github.com/singularian/mdencode/code/encode/ Makefile 1.0  

$ mddbreport filesig.db  

file 1: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ america.txt 1.0  
file 2: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ twelve.doc 1.0  
file 3: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ century.xml 1.0  
```

# Example mddbreport formats

This example shows how to use the mddbreport utility to display the file signatures in a format. 

```
$ mddbreport ssss.db 1
```

[ssss.db.txt](https://raw.githubusercontent.com/singularian/mdencode/master/examples/ssss.db.txt)

```
$ mddbreport ssss.db 1 600
```
[ssss.db.json](https://raw.githubusercontent.com/singularian/mdencode/master/examples/ssss.db.json)

```
$ mddbreport ssss.db 1 100
```
[ssss.db.xml](https://raw.githubusercontent.com/singularian/mdencode/master/examples/ssss.db.xml)

If zero is specified for the argument it will format and output all the signature db files in the numeric format
```
$ mddbreport ssss.db 0 100
```

# Donations

* BTC: `31ixWCPg2Ax5uhYk418C4KBFacnnri9Axb`
* ETH: `0x68907c4ac25e2041A797Ce8c639994cf9EbFdc8C`

# License 

[License](https://github.com/singularian/mdencode/blob/master/LICENSE)
