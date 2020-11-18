# Project MDencode Overview :octocat:  
  
MDencode is a [GO](https://golang.org/) and [C++](https://github.com/singularian/mdencode/tree/master/code/decode_cpp) command line file digital signature generator with modular floors.  
This program is written in GO and uses an interface for programmable formatters.  
MDencode allows for the creation of sqlite3 db file signatures and the first random file signatures and signature decryption. 

This program can generate a file signature at the file or block level and format and write the output in multiple formats.  
It can also write to an sqlite db. The mdencode sqlite db files contain a table schema and can be exported into multiple formats.  

MDencode also contains an mdzip and mdunzip utility for encoding and decoding files with modular floors and signatures.

# Features

- Versioned Cryptographic File Signatures
- Optional File Crypto Signatures
- Optional N-Bit Sized File Block Modular Floors
- Optional N-Byte Block Crypto Signatures
- SQLite3 DB Crypto Signature Files
- Plugin Formatters
- Multiple Crypto Hash [Signatures](https://github.com/singularian/mdencode/blob/master/docs/hashList.md) for Files and File Blocks
- The "First" [Random File Signature](https://github.com/singularian/mdencode/blob/master/docs/RandomSignature.md) or Block signatures 
- An example of a parallel signature modulus scan with an encoder and decoder ([mdzip and mdunzip](https://github.com/singularian/mdencode/blob/master/docs/mdzip.md))

# Output Formats

- TEXT
- INFORM
- HTML
- XML
- JSON
- MARK DOWN
- BINARY
- IMAGES
- CSV
- SQLITE3 DB
- BUILD YOUR OWN

# Digital Signature List

This is a list of the digital signatures and the overall objectives.  
More can be added to the signature chain and are selectable via a binary flag argument for file blocks and files.
You can also add signatures. The design goal is to have hundreds of selectable signatures.

- The capability to add more signatures 
- A chief design goal is to have hundreds of user selectable file or block and block group signatures
- Prioritize file signature quantity over usability
- A large extensible [Digital Signature List](https://github.com/singularian/mdencode/blob/master/docs/hashList.md)

# Digital Signature Heirarchy

These are the types of file signatures that mdencode will implement.

- File Signatures
- File Group Block Signatures. TODO. These create signatures of multiple Block Signatures. 
  If the blocksize is 10 and blockgroups is 10 it will generate signatures every 100 bytes.
  It minimizes block collisions.
- File Block Signatures

![This is my block chopper](https://github.com/singularian/mdencode/blob/master/diagrams/modulusScanHeirarchy.png)

# MDencode usage
This is the mdencode usage.
mdencode has these main GO subprograms. 

- md or mdencode - generates file signatures with optional modular floors
- mddbreport     - generates formatted signatures dumps of file signatures stored in a mdencode sqlite3 db
- mdnewdb        - creates new sqlite3 mdencode signatures
- mdbinlist      - displays a binary file signature in the integer parameter format (xml or text or JSON etc).
- mdzip          - creates a binary signature file with file signatures or block signatures and a modular floor
- mdunzip        - uses a modulus scan to permute over the mdzip file binary signature blocks and find a corresponding byte block

mdencode or md can generate a signature for a file or directory and a modular floor

mdencode can generate random file signatures and block signatures.
mdencode is split into encode and decode parts.

Random file signatures are file signatures that can change block or file hash lists and block size randomly from program executions.
In one run the mdencode program can run ripe160, sha1, sha224 and whirlpool on a file while in another run it might run just md4 and sha1.
This allows for a file signature to have the lists randomized and improves security by making signatures unpredictable. The signature lists
for mdencode are bit strings which can be random bit strings. This allows for the first random file signatures.

- [Example Usage](https://github.com/singularian/mdencode/blob/master/docs/Usage.md)

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
4300    - Mark Down
4500    - HTML
5000    - GO XML
5001    - XML Formatter
8000    - Image PNG Formatter
8001    - Image PNG Formatter
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
md -uh=112203300111022033333 -block=100 -mod=64 -file=Makefile
```

# Example usage

This is a file signature for Makefile with a block size of 100 bytes and a 128 bit modulus and the fileblock signature bit string and file bit string and a format specified by number 10. It is formatted by GO flags args.

[Ascii Cast Example](https://asciinema.org/a/293089)

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

```
md -file=america.txt -block=100 -mod=128 -format=4000 -fh=10000111 -bh=10001111 -out=america.txt_b100_mod128_json.md
```

[Mark Down Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b300_mod32_MD.md)

```
md -file=america.txt -block=300 -mod=32 -format=4300 -fh=10000111 -bh=10001111 -out=america.txt_b300_mod32_MD.md
```

[Image Examples](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b300_mod32.png)

```
md -file=america.txt -block=300 -mod=32 -format=8000 -fh=10000111 -bh=10001111 -out=hashImage.png
```

![hashImage.png](https://github.com/singularian/mdencode/blob/master/examples/hashImage.png "hashImage.png")

# Example mddbreport usage

[Example Usage](https://github.com/singularian/mdencode/blob/master/docs/Usage.md)

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

The [modulus scan](https://github.com/singularian/mdencode/blob/master/docs/modulusScan.md) polynomial is calculated as follows.

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
  
This is an example of the modular floor using the C++ test program.  
The program `decoderRandom2` is using a random 14 byte array and then running a modulus scan to find the corresponding file block associated with a digital signature.  
It uses a 32 bit modulus and calculates modulus exponent floor or ceiling and then creates a fast64 hash. A modulus scan splits up the parallel search on 32 threads. 
It calculated and found the 14 byte block (13 bytes compressed) in 4 hours on a Ryzen 3950x.

13/14 Modulus Encoding
- 8 bytes for the 64-bit fasthash 64 
- 4 bytes for the 32-bit modulus 
- 1 byte for the modulus exponent 

`$GOPATH/github.com/singularian/mdencode/code/decode_cpp/decoderRandomTestHC2`
```
Run on the ryzen 3950x with 32 threads.

./decoderRandomTestHC2 --mod=32 --threads=32 --hl 4  --hex=000000001211211111111122FFFC
hash values  4
Start Time               Sun Nov 15 17:52:53 2020
Block Size               14
Random Byteblock         000000001211211111111122FFFC
Random Byteblock Hex     00  00  00  00  12  11  21  11  11  11  11  22  FF  FC
Random Byteblock Int     0   0   0   0   18  17  33  17  17  17  17  34  255 252
Random Byteblock Bigint  85318574045349531549692
Modulus Size             32
Modulus Bigint           4294967296
Modulus Remainder        287506428
Modulus 2   ^ Exponent   76
Modulus Mod ^ Exponent   2
Block Signatures         fast64 5986481724077706591
Thread Count             32
Logging                  false

Found Match

Elapsed Time (s) 14601.1    = 4.05 hours
Modulus Scan thread 17 and Random byteblock match
0 0 0 0 18 17 33 17 17 17 17 34 255 252
0 0 0 0 18 17 33 17 17 17 17 34 255 252
```

Second example with a larger block running on a ryzen 3950x.

```
./decoderRandomTestHC2 --mod=32 --threads=32 --hl 4  --hex=0000000016412161123F1822FFFC --log=true
hash values  4 
Start Time               Tue Nov 17 21:44:08 2020
Block Size               14
Random Byteblock         0000000016412161123F1822FFFC
                         1   2   3   4   5   6   7   8   9   10  11  12  13  14  
Random Byteblock Hex     00  00  00  00  16  41  21  61  12  3F  18  22  FF  FC  
Random Byteblock Int     0   0   0   0   22  65  33  97  18  63  24  34  255 252 
Random Byteblock Bigint  105093506211661505298428
Modulus Size             32
Modulus Bigint           4294967296
Modulus Remainder        404946940
Modulus 2   ^ Exponent   76
Modulus Mod ^ Exponent   2
Block Signatures         fast64 10504795572753995326 
Thread Count             32
Logging                  true

Running decode modscanFound Match

Elapsed Time (s) 43036
Modulus Scan thread 31 and Random byteblock match
0 0 0 0 22 65 33 97 18 63 24 34 255 252 
0 0 0 0 22 65 33 97 18 63 24 34 255 252 
```

[Other Decoder Examples](https://github.com/singularian/mdencode/blob/master/docs/EXAMPLES.md)

# Parallel Modulus Scan

There are currently two example modulus scan programs. These include

- GO test modulus scan ```$GOPATH/github.com/singularian/mdencode/code/testdecode/decoderRandomTestHC``` uses GO routines to run the modulus scan for a signature on a byte block
- C++ test modulus scan ```$GOPATH/github.com/singularian/mdencode/code/decode_cpp/decoderRandomTestHC2``` uses C++ threads to run the modulus scan

MDZip and MDUnzip also use a parallel modulus scan currently with the GO core. In the future it will include the C++ core.


This is an example of a parallel modulus scan with 16 threads.
[Decoder Parallel Examples](https://github.com/singularian/mdencode/blob/master/examples/mdencodeParallelmodscan2.txt)

# MDzip and MDunzip Overview

mdzip is the GO command line utility can compress a file into a md binary byte block files. 
mdzip takes an optional input key for the 32 byte Highway Hash 64 and Highway Hash 128 and Highway Hash 256. It can also take input keylist parameters for other signature keys.

mdunzip will decompress the input signature binary block file to an output file.  

mdunzip has a context hash list for each decode thread. mdzip can use multiple block signatures and mdunzip will use the specified signatures to unzip the file.

Proccessing power limits the size of the input block.  

**MDZip Features**
- User Specified Modulus Size
- User Specified Block Size
- File Hash Signatures
- Block Hash Signatures
- Optional Signature Keys 
- 32 Byte Highway Hash Signature Keys which change the signature

**MDUnzip Features**
- Multithreaded Parallel Modulus Scan
- Decrypts an MDZip file to an output file 
- File Signature Post Validation
- Uses the MDZip Signature Keys to change the output file signature Modulus Scan


# MDZip Examples

These are mdzip and mdunzip command line examples. They also illustrate setting the Highway Hash 32 byte signature key. MDunzip will use the specified Highway Hash Key.  

```bash
mdzip -file=decoderRandom.go -block=40 fh=11111 bh=01001 -mod=64 -out=decoderRandom.go.mdz  
mdzip -file=decoderRandom.go -block=41 fh=101111111 bh=0101 -mod=64 -out=decoderRandom.go.mdz  
mdzip -file=decoderRandom.go -block=41 uh=3333 -mod=64 -out=decoderRandom.go.mdz  
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000000000000000000001   -fh=111110000000011111 -keylist=hw64:FFFF11111201111111111  
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000000001  -fh=111110000000011111 -keylist=hw128:FFFF11111201111111111  
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=00000000000000000000001 -fh=111110000000011111 -keylist=hw256:FFFF11111100DA111CCFF111  

mdunzip -file=decoderRandom.go.mdz -out=decoderRandom.go.mdz.uncompressed -thread=16
```

# MDZip Usage

- [MDZip Usage](https://github.com/singularian/mdencode/blob/master/docs/mdzip.md)
- [General Usage](https://github.com/singularian/mdencode/blob/master/docs/Usage.md)

# MDZip Video Example 

These are some Ascii Art Video Examples 

[MDZip Example](https://asciinema.org/a/293119)  
[MDZip Example 2](https://asciinema.org/a/293112)   
[MDZip Highway Hash Key Example](https://asciinema.org/a/293740)  

# Collision Handling

These are a brief list of collision management in mdzip and mdunzip.

- Using File Level Signatures to validate Block Level Signatures.
- **Using File Level Signatures** to differentiate between File Block Signature Collisions.
- **Signature Switching.** If one signature or signature group has a collsion switch to another signature or signature group.
- **Change the signature key.** An example is Highway Hash with a 256 bit key which can be changed to different keys.
- **Use Optional Block Groups** as a wrapper around signature blocks with collisions or add additional signatures.
- **Use Optional File Block Collision Numbers**. Collision numbers can designate the correct match.
- Use Block Signature Pairs with Modulus / Modular Floors.
- Using Modular floors to skip numbers

# Build

This is a guide for building MDencode on Linux.

[Example Build](https://github.com/singularian/mdencode/blob/master/docs/Build.md)

# Docker Container 

This is a Docker Hub Docker File for MDencode

[Docker Container](https://hub.docker.com/r/annatarsol/mdencode)

### Docker Pull

docker pull annatarsol/mdencode  

### Docker Container Run

docker run -it annatarsol/mdencode/latest /bin/bash  

# Donations

* BTC: `32L3gyJdkiFwgJ72rsSzbCBjTjVFxJLHPm`
* ETH: `0x137eB09cF2e199dA2Aa79607f96719be1491a312`
* LTC: `MJh8XpUQ5wm8x1HCoQ7dewNqhrLfZZc31x`

# Author
MDEncode was developed by Scott Ross on a Ryzen CPU.

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
