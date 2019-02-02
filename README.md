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
- An example of a parallel signature modulus scan

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
This is the mdencode usage.
mdencode has three main golang programs. 

- md or mdencode - generates file signatures
- mddbreport - generates formatted signatures dumps of file signatures stored in a mdencode sqlite3 db
- mdnewdb  - creates new sqlite3 mdencode signatures

mdencode or md can generate a signature for a file or directory and a modular floor

mdencode can generate random file signatures and block signatures.
mdencode is split into encode and decode parts.

Random file signatures are file signatures that can change block or file hash lists and block size randomly from program executions.
In one run the mdencode program can run ripe160, sha1, sha224 and whirlpool on a file while in another run it might run just md4 and sha1.
This allows for a file signature to have the lists randomized and improves security by making signatures unpredictable. The signature lists
for mdencode are bit strings which can be random bit strings. This allows for the first random file signatures.

```
Usage of md:

  -file string
        Input Filename
  -block string
        File Block Size Bytes (default "40")
  -fh string
        File Hash Bit String List (default "011")
  -bh string
        Block Hash Bit String List (default "011")
  -mod string
        Modulus Size in Bits (default "32")
  -format int
        Output Format (default 10)
  -fr   
        Generate A Random File Hash Bit String List
  -br   
        Generate A Random Block Hash Bit String List
  -fbr  
        Generate A Random File Hash and Block Hash Bit String List
  -blockr
        Generate A Random File Block Size
  -modr
        Generate A Random File Block Modulus
  -all
        Randomize everything
  -out string
        Output Filename
  -dir string
        Input Directory
  -append
        Append To Output File
  -line
        File Hash as one line
  -byte 
        Append the File Byteblock to the Block Hash List
  -blockint
        Append the File Byteblock Bigint to the Block Hash List
  -log string
        Log Filename
  -initdb string
        Create SQLite3 Empty DB File
  -key string
        Signature Key (Minimum 16 bytes for siphash) (default "LomaLindaSanSerento9000")

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

# Example usage

This is a file signature for Makefile with a block size of 100 bytes and a 128 bit modulus and the fileblock signature bit string and file bit string and a format specified by number 10. It is formatted by go flags args.

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=10 -file=america.txt -out=america.txt_b100_mod128_f9.md
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

This is my block chopper.  

![This is my block chopper](https://github.com/singularian/mdencode/blob/master/diagrams/blockchopper.jpg "Coyau's Cleaver")

This program uses a modular floor paired with a package of digital signatures. This makes the digital signature more unique. 
A modular floor consists of a polynomial which is the modular exponent plus n times the modulus plus the remainder. 
It is possible to use a modular scan to find matching byte blocks associated with the digital signature package.
A digital signature package or group can consist of 1 or more block and file signatures. 
A modular floor is generaly computed at the block level for a file byte block and utilizes a big integer.

The modulus scan polynomial is calculated as follows.

```
modremainder    = fileblockbigint % modulus  
fileblockbigint = modulus * n + modremainder where n equals 0 to m  
fileblockbigint = modulus ^ (exponent p) + modulus * n + modremainder where p is an optional power and n equals 0 to m    
```

# Modular Floor Equation

This is the modular floor equation for two forms. One is the exponential floor while the other contains a multiple of the modular floor plus the remainder.
A modulus should be considered part of the file signature subset. A modulus is a digital signature.

![](https://github.com/singularian/mdencode/blob/master/diagrams/modularfloor.png)  
![](https://github.com/singularian/mdencode/blob/master/diagrams/modularfloorexponent.png)  

# Modular Floor Examples
  
This is an example of the modular floor.  
The program `decoderRandom` is creating a random 6 byte array and then running a modulus scan to find the corresponding file block associated with a digital signature.  
It creates a random 6 byte array and a 32 bit modulus and a modulus exponent and then creates a sha1 hash and md5 hash.  
It calculated and found a hashed 6 byte block in two seconds on a Pentium n3700.  


`$GOPATH/github.com/singularian/mdencode/code/decode/decoderRandom`
```
$ ./decoderRandom
random  6  bytes  [175 149 193 192 35 31]
modulus size bits  32
byte modulus  4294967296
block modulus  3250594591
modulus exponent  47
shasum  dc483a03987b1e24cc6e235671767b2c6ce17dd1
md5sum  a09f627e7dc71be69f7d0f20a214864f
modulo bigint  4294967296
modulo floor  140737488355328
modulo ceil  281474976710656
Found Block  [175 149 193 192 35 31]
total time  71.219466ms
random bytestring and modulusscan bytestring match  [175 149 193 192 35 31]   [175 149 193 192 35 31]
```
This example of decoderRandom uses a 32-bit and 24-bit modulus to calculate a 6 byte block associated with an sha1 and md5 signature.
```
$ ./decoderRandom
random  6  bytes  [139 111 156 34 193 144]
modulus size bits  32
byte modulus  4294967296
block modulus  2619523472
modulus exponent  47
shasum  af2757f99333d0f693cb70d5ff0e1caaf90cdb50
md5sum  448497bb2b4327b46de2d7cfc6b351c7
modulo bigint  4294967296
modulo floor  140737488355328
modulo ceil  281474976710656
Found Block  [139 111 156 34 193 144]
total time  58.27042ms
random bytestring and modulusscan bytestring match  [139 111 156 34 193 144]   [139 111 156 34 193 144]
```

[Other Decoder Examples](https://github.com/singularian/mdencode/blob/master/docs/EXAMPLES.md)

# Parallel Modulus Scan

The Modulus Scan can also be run in parallel. An example program decoderRandom4 shows a modulus scan with configurable number of goroutines.
Each of the goroutines runs a modulus scan in sync and when a result is founding matching the original signature block it returns a byteblock.

This is the usage of the prototype parallel modulus scan program.
It allows for the blocksize and modulus bitsize and thread count as well as the bytes either random or specified by the command line argument.
```
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./decoderRandom4
Usage  ./decoderRandom4  -block=[BLOCKSIZE BYTES] -mod=[MODSIZE BITS] -thread=[THREADSIZE GOROUTINES]
Usage  ./decoderRandom4  -block=8 -mod=64 -thread=10
Usage  ./decoderRandom4  -block=8 -mod=64 -thread=10 -bytes=[1,2,3,4,5]
```

This is an example of a parallel modulus scan with 16 threads.
[Decoder Parallel Examples](https://github.com/singularian/mdencode/blob/master/examples/mdencodeParallelmodscan2.txt)

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

```
git clone https://github.com/singularian/mdencode.git
```
make all builds mdencode to gobin by default
```
$GOPATH/github.com/singularian/mdencode/code/encode $ make all
```

make build builds the mdencode to the mdBuild directory
```
$GOPATH/github.com/singularian/mdencode/code/encode $ make build
```

This example builds the mdencode decoder files
```
$GOPATH/github.com/singularian/mdencode/code/decode $ make build
```

# Donations

* BTC: `31ixWCPg2Ax5uhYk418C4KBFacnnri9Axb`
* ETH: `0x68907c4ac25e2041A797Ce8c639994cf9EbFdc8C`

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
