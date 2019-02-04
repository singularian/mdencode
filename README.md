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

# Encoding Types

These are the types of file signatures that mdencode will implement.

- File Signatures
- File Group Block Signatures. TODO. These create signatures of multiple Block Signatures. 
  If the blocksize is 10 and blockgroups is 10 it will generate signatures every 100 bytes.
  It minimizes block collisions.
- File Block Signatures


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
The program `decoderRandom4` is creating a random 6 byte array and then running a modulus scan to find the corresponding file block associated with a digital signature.  
It creates a random 10 byte array and a 64 bit modulus and a modulus exponent and then creates a sha1 hash and md5 hash.  
It calculated and found a hashed 10 byte block in two seconds on a Ryzen 2700x.

`$GOPATH/github.com/singularian/mdencode/code/decode/decoderRandom4 `
```
$ ./decoderRandom4 -mod=64 -block=10 -thread=16

2019/02/04 03:09:40 Starting Modulus Scan Random  0
2019/02/04 03:09:40 blocksize  10
2019/02/04 03:09:40 byte block [33 209 186 43 161 221 31 71 174 90]
2019/02/04 03:09:40 byte bigint 159706878439989726129754
2019/02/04 03:09:40 modulus bit size  64
2019/02/04 03:09:40 byte block modulus  18446744073709551616
2019/02/04 03:09:40 byte block modulus remainder  13414993886137790042
2019/02/04 03:09:40 modulus exponent  77
2019/02/04 03:09:40 shasum  8235d77c7b1b6d3c88e099f49c4f6d8613a72640
2019/02/04 03:09:40 md5sum  a58cdac0fc1ec2afc9a8b2728d6e5c40
2019/02/04 03:09:40 Starting decoderRandom
2019/02/04 03:09:40 modulo bigint 18446744073709551616
2019/02/04 03:09:40 modulo floor  151115727451828646838272  ceil  302231454903657293676544  modceiltwo  2
  :
  :

Found Block  [33 209 186 43 161 221 31 71 174 90]
Total time  11.112ms
random bytestring and modulusscan bytestring match  [33 209 186 43 161 221 31 71 174 90]   [33 209 186 43 161 221 31 71 174 90]
result  thread 1 random bytestring and modulusscan bytestring match [33 209 186 43 161 221 31 71 174 90] [33 209 186 43 161 221 31 71 174 90]
```
This example of decoderRandom4 uses a 64-bit modulus to calculate a 12 byte block associated with an sha1 and md5 signature.
It uses 16 threads and a parallel modulus scan and was run on a Ryzen 2700x. 
```
$ ./decoderRandom4 -mod=64 -block=12 -thread=16 
2019/02/04 02:53:35 Starting Modulus Scan Random  0
2019/02/04 02:53:35 blocksize  12
2019/02/04 02:53:35 byte block [197 170 222 253 176 100 237 126 149 160 151 34]
2019/02/04 02:53:35 byte bigint 61175117369235458384514291490
2019/02/04 02:53:35 modulus bit size  64
2019/02/04 02:53:35 byte block modulus  18446744073709551616
2019/02/04 02:53:35 byte block modulus remainder  12710545176278374178
2019/02/04 02:53:35 modulus exponent  95
2019/02/04 02:53:35 shasum  5f02bab5517b15600692ca5e5c0c97a44a77e86a
2019/02/04 02:53:35 md5sum  6a6e942ac8efc828bddb5e38c5ce721d
2019/02/04 02:53:35 Starting decoderRandom
2019/02/04 02:53:35 modulo bigint 18446744073709551616
2019/02/04 02:53:35 modulo floor  39614081257132168796771975168  ceil  79228162514264337593543950336  modceiltwo  2
2019/02/04 02:53:35 modulo floor  95   39614081257132168796771975168  ceil  79228162514264337593543950336
2019/02/04 02:53:35 mfloor  39614081257132168796771975168   0
2019/02/04 02:53:35 modulo floor equals zero  0   0
2019/02/04 02:53:35 modulo floor equals zero setting  39614081257132168796771975168   39614081257132168796771975168
2019/02/04 02:53:35 modremainder  39614081257132168796771975168   39614081257132168796771975168
2019/02/04 02:53:35 thread  0   16  modstart test result floor  39614081269842713973050349346  initial remainder  39614081257132168796771975168  iterator  295147905179352825856  mult = mod * thc  295147905179352825856  mult2 = m * thnum  0
 :
 :
 :
Total time  2m53.344956s
random bytestring and modulusscan bytestring match  [197 170 222 253 176 100 237 126 149 160 151 34]   [197 170 222 253 176 100 237 126 149 160 151 34]
result  thread 13 random bytestring and modulusscan bytestring match [197 170 222 253 176 100 237 126 149 160 151 34] [197 170 222 253 176 100 237 126 149 160 151 34]
```

[Other Decoder Examples](https://github.com/singularian/mdencode/blob/master/docs/EXAMPLES.md)

# Parallel Modulus Scan

The Modulus Scan can also be run in parallel. An example program decoderRandom4 shows a modulus scan with configurable number of goroutines.
Each of the goroutines runs a modulus scan in sync and when a result is founding matching the original signature block it returns a byteblock.

This is the usage of the prototype parallel modulus scan program.
It allows for the blocksize and modulus bitsize and thread count as well as the bytes either random or specified by the command line argument.
```
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./decoderRandom4
Usage  ./decoderRandom4  -block=[BLOCKSIZE BYTES] -mod=[MODSIZE BITS] -thread=[THREADSIZE GOROUTINES] -bytes=[OPTIONAL JSON BYTESTRING]
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
