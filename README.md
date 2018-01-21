# Project mdencode :octocat:
mdencode is a golang command line file digital signature generator with modular floors.  
mdencode is written in golang and uses an interface for pluggin formatters.  
mdencode allows for the creation of sqlite3 db file signatures and the first random file signatures. 

This program can generate a file signature at the file or block level and format and write the output in multiple formats.  
It can also write to an sqlite db. The mdencode sqlite db files contain a table schema and can be exported into multiple formats.  

# Features

- Versioned Cryptographic File Signatures
- Optional File Crypto Signatures
- Optional N-Bit Sized File Block Modular Floors
- Optional N-Byte Block Crypto Signatures
- SQLite3 DB Crypto Signature Files
- Plugin Formatters
- Multiple Selectable Crypto Hash Signatures for Files and File Blocks
- The "First" Random File or Block signatures 

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



# Mdencode usage

```
Usage of md:

  -fh string
     File Hash Boolean String List (default "011")     
  -bh string  
     Block Hash Boolean String List (default "011")   
  -fr
     Generate A Random File Hash Boolean String List
  -br
     Generate A Random Block Hash Boolean String List
  -fbr
     Generate A Random File and Block Hash Boolean String List
  -block string  
     File Block Size Bytes (default "40")  
  -mod string  
     Modulus Size in Bits (default "32")       
  -append  
     Append To Output File     
  -file string  
     Input filename  
  -dir string
     Input Directory
  -format int  
     Output Format (default 10)
  -initdb string
     Create SQLite3 Empty DB File      
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

Formats:
0 .. 98 - Text
101     - CSV
102     - CSV
1000    - Binary
2000    - SQL Lite 3 DB File
3000    - Inform
4000    - JSON
5000    - GO XML
5001    - XML Formatter

Examples:

md -file=Makefile -block=100 -fh=1 -bh=0 -format=0 -line=true
md -file=Makefile -block=100 fh=1111111 -bh=0 -format=10 -line=false
md -file=Makefile -block=300 -fh=0 -bh=111 -format=10 -line
md -file=Makefile -block=100 -fh=1011111 -bh=1100111 -format=19 -line=true -out=outputfile
md -file=Makefile -block=100 -fh=10100111 -bh=100111   -format=19 -append=true -line=true -out=outputfile
md -file=Makefile -block=100 -fh=1011111101 -bh=10011101 -format=3000 -append=false -line=true -out=outputfile.inform
md -file=Makefile -block=100 -fh=1011111001 -bh=100111   -format=4000 -append=true -line=true -out=outputfile.json
md -file=Makefile -block=1000 -mod=0 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=100 -mod=64 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=100 -mod=128 -fh=1011111 -bh=100111  -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=350 -mod=32 -fh=101011 -br
md -file=Makefile -block=300 -mod=32 -fr -br
```

# Output Formats

This is a list of the current numerical format assignment.  
This is subject to change.  
```sh
0 .. 98 - Text
101     - CSV
102     - CSV
1000    - Binary
2000    - SQL Lite 3 DB File
3000    - Inform
4000    - JSON
5000    - GO XML
5001    - XML Formatter
```

# Command Line Examples

```sh
md -file=Makefile -block=100 -fh=1 -bh=0 -format=0 -line=true
md -file=Makefile -block=100 fh=1111111 -bh=0 -format=10 -line=false
md -file=Makefile -block=300 -fh=0 -bh=111 -format=10 -line
md -file=Makefile -block=100 -fh=1011111 -bh=1100111 -format=19 -line=true -out=outputfile
md -file=Makefile -block=100 -fh=10100111 -bh=100111   -format=19 -append=true -line=true -out=outputfile
md -file=Makefile -block=100 -fh=1011111101 -bh=10011101 -format=3000 -append=false -line=true -out=outputfile.inform
md -file=Makefile -block=100 -fh=1011111001 -bh=100111   -format=4000 -append=true -line=true -out=outputfile.json
md -file=Makefile -block=1000 -mod=0 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=100 -mod=64 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=100 -mod=128 -fh=1011111 -bh=100111  -format=5000 -append=true -line=true -out=outputfile.xml
md -file=Makefile -block=350 -mod=32 -fh=101011 -br
md -file=Makefile -block=300 -mod=32 -fr -br
```

# Eample usage

This is a file signature for Makefile with a block size of 100 bytes and a 128 bit modulus and the fileblock signature bit string and file bit string and a format specified by number 10. It is formatted by go flags args.

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=9 -file=america.txt -out=america.txt_b100_mod128_f9.md
```

[Text Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_f9.md)

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=5000 -file=america.txt -out=america.txt_b100_mod128_xml.md
```

[XML Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_xml.md)

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=4000 -file=america.txt -out=america.txt_b100_mod128_xml.md
```

[JSON Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_json.md)


# Example mddbreport usage

```
Usage of mddbreport:
  -append
        Append To Output File
  -dumpall
        Dump all the sql file signatures
  -file string
        SQLite3 DB Signature Filename
  -fileid uint
        Fileid
  -format int
        Output Format (default 4)
  -out string
        Output Filename

Version: 1.0 復甦 復活

Examples:
mddbreport -file=md.db -fileid=1 -format=10
mddbreport -file=md.db -fileid=1 -format=10 -out=outputfile.txt -append=true
mddbreport -file=md.db -fileid=1 -format=5000 -out=outputfile.xml -append=false

```

# Using mddbreport 

mddbreport is a utility to display the singatures in an sqlite3 db signature file.
This example shows how to generate an sqlite3 file db signature

```
md -file=Makefile -format=2000 -line=true -out=sqlite3_example
```

It can also generate a formatted signature from the sqlite3_example.db sqlite 3 file

This example shows how to list the files in a sqlite3_example.db file sqlite3 signature db

The columns are:

file id, filepath, filename, version

```
$ mddbreport -file=sqlite3_example.db

file 1: /projects/src/github.com/singularian/mdencode/code/encode/ Makefile 1.0  

$ mddbreport -file=filesig.db  

file 1: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ america.txt 1.0  
file 2: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ twelve.doc 1.0  
file 3: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ century.xml 1.0  
```

# Example mddbreport formats

This example shows how to use the mddbreport utility to display the file sqlite3 db signatures in a numerical format. 

```
$ mddbreport -file=example.db -fileid=1
```

[example.db.txt](https://raw.githubusercontent.com/singularian/mdencode/master/examples/example.db.txt)

```
$ mddbreport -file=example.db -fileid=1 -format=4000

```
[example.db.json](https://raw.githubusercontent.com/singularian/mdencode/master/examples/example.db.json)

```
$ mddbreport -file=example.db -fileid=1 -format=5001
```
[example.db.xml](https://raw.githubusercontent.com/singularian/mdencode/master/examples/example.db.xml)

If zero is specified for the argument it will format and output all the signature db files in the numeric format
```
$ mddbreport -file=example.db -fileid=0 -format=100
```

# Modular Floor

This program uses a modular floor paired with a package of digital signatures. This makes the digital signature more unique. 
A modular floor consists of a polynomial which is the modular exponent plus n times the modulus plus the remainder. 
It is possible to use a modular scan to find matching byte blocks associated with the digital signature package.
A digital signature package or group can consist of 1 or more block and file signatures. 
A modular floor is generaly computed at the block level for a file byte block and utilizes a big integer.

modremainder    = fileblockbigint % modulus  
fileblockbigint = modulus * n + modremainder where n equals 0 to x  
fileblockbigint = modulus ^ exponent + modulus * n + modremainder where n equals 0 to m    

This is an example of the modular floor. The program decoder4 is running a modulus scan to find the corresponding file block associated with a digital signature.  
It calculated a and found a hashed 6 byte block in two seconds on a Pentium n3700.
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

# Build

##### Basic Linux / Unix Go Build

git clone https://github.com/singularian/mdencode.git

make all builds mdencode to gobin by default

/github.com/singularian/mdencode/code/encode $ make all

make build builds the mdencode to the mdBuild directory

/github.com/singularian/mdencode/code/encode $ make build

# Donations

* BTC: `31ixWCPg2Ax5uhYk418C4KBFacnnri9Axb`
* ETH: `0x68907c4ac25e2041A797Ce8c639994cf9EbFdc8C`

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
