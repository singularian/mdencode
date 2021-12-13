# Project MDencode Overview :octocat:  
  
MDencode is a [GO](https://golang.org/) file signature and [C++](https://github.com/singularian/mdencode/blob/master/docs/mdzip_cpp.md) command line file digital signature encoder with modular floors.  
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
- Multiple [Signatures](https://github.com/singularian/mdencode/blob/master/docs/hashList.md) for Files and File Blocks
- The "First" [Random File Signature](https://github.com/singularian/mdencode/blob/master/docs/RandomSignature.md) or Block signatures 
- An example of a parallel signature modulus scan with an encoder and decoder ([mdzip and mdunzip](https://github.com/singularian/mdencode/blob/master/docs/mdzip_cpp.md))

# MDencode Components

- [MDencode File Signatures](https://github.com/singularian/mdencode/blob/master/docs/mdencodeFile.md)
- [mdzip/mdunzip C++](https://github.com/singularian/mdencode/blob/master/docs/mdzip_cpp.md)
- [mdzip/mdunzip Go](https://github.com/singularian/mdencode/blob/master/docs/mdzip_go.md)



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
It calculated and found the 14 byte block (13 bytes compressed) in 4 hours on a Ryzen 3950x. It compresses the 14 byte block to 13 bytes and decompresses it to 14 bytes.  

13/14 Modulus Encoding
- 8 bytes for the 64-bit fasthash 64 - (It can use any number of 64 bit hashes)
- 4 bytes for the 32-bit modulus 
- 1 byte for the modulus exponent 

C++ Test program
- ```$GOPATH/github.com/singularian/mdencode/code/testdecode_cpp/mdtest```

```
./mdtest --mod=32 --threads=32 --hl 4  --hex=000000001211211111111122FFFC
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

Second modulus scan decompression example with a larger block running on a ryzen 3950x.

```
./mdtest --mod=32 --threads=32 --hl 4  --hex=0000000016412161123F1822FFFC --log=true
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

Running decode modscan
Found Match

Elapsed Time (s) 43036 = 12 hours
Modulus Scan thread 31 and Random byteblock match
0 0 0 0 22 65 33 97 18 63 24 34 255 252 
0 0 0 0 22 65 33 97 18 63 24 34 255 252 
```

Another modulus scan decompression example with a larger byte block 

```
./mdtest --mod=32 --threads=32 --hl 4  --hex=0000002020412161123F1822FFFC --log=true
hash values  4

Start Time               Sat Nov 21 19:59:59 2020
Block Size               14
Random Byteblock         0000002020412161123F1822FFFC
                         1   2   3   4   5   6   7   8   9   10  11  12  13  14
Random Byteblock Hex     00  00  00  20  20  41  21  61  12  3F  18  22  FF  FC
Random Byteblock Int     0   0   0   32  32  65  33  97  18  63  24  34  255 252
Random Byteblock Bigint  38837943398708491548033020
Modulus Size             32
Modulus Bigint           4294967296
Modulus Remainder        404946940
Modulus 2   ^ Exponent   85
Modulus Mod ^ Exponent   2
Block Signatures         fast64 2256604700901092545
Thread Count             32
Logging                  true


Running decode modscan

Found Match

Elapsed Time (s) 220182.811267 - 61.16 hours
Modulus Scan thread 31 and Random byteblock match
0   0   0   32  32  65  33  97  18  63  24  34  255 252
0   0   0   32  32  65  33  97  18  63  24  34  255 252
```

This is the Sip47 modulus scan decompression example running on a ryzen 3950x.
It could use a 4 to 7 bit bitstream to encode the modulus exponent.

```
./mdtest --mod=32  --hl 30 --randbh  --log --hex=0066BDF2F888F000FF00 --block=10

Start Time               Sun Oct 31 19:06:06 2021
Block Size               10
Random Byteblock         0066BDF2F888F000FF00
                         1   2   3   4   5   6   7   8   9   10
Random Byteblock Hex     00  66  BD  F2  F8  88  F0  00  FF  00
Random Byteblock Int     0   102 189 242 248 136 240 0   255 0
Random Byteblock Bigint  1895255171002932461312
Modulus Size             32
Modulus Bigint           4294967295
Modulus Remainder        2918447087
Modulus 2   ^ Exponent   70
Modulus Mod ^ Exponent   2
Block Signatures         sip40 5246BACF79
Blockkeylist             sip40 keys 8E197EC7727CECD973118AEE2E72BE65
Thread Count             32
Logging                  true

Hash Block Vector
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           30           sip40       5           16
                         Total       5           16

Running decode modscan

Found Match

Elapsed Time (s) 2448.636829 = 40.8 minutes
Modulus Scan thread 15 and Random byteblock match
0 102 189 242 248 136 240   0 255   0
0 102 189 242 248 136 240   0 255   0
```

[Other Decoder Examples](https://github.com/singularian/mdencode/blob/master/docs/EXAMPLES.md)

# Parallel Modulus Scan

There are currently two test modulus scan programs. These include

- GO test modulus scan ```$GOPATH/github.com/singularian/mdencode/code/testdecode_go/decoderRandomTestHC``` uses GO routines to run the modulus scan for a signature on a byte block
- C++ test modulus scan ```$GOPATH/github.com/singularian/mdencode/code/testdecode_cpp/mdtest``` uses C++ threads to run the modulus scan

MDZip and MDUnzip also use a parallel modulus scan with the GO or C++ core. [mdzip C++](https://github.com/singularian/mdencode/tree/master/docs/mdzip_cpp.md).  


These are examples of a parallel modulus scan for mdzip C++ with Siphash 40.  
  
- [Siphash 40 MDzip With No Increment Key](https://github.com/singularian/mdencode/blob/master/docs/tests/mdzip_siphash40_test.md)  
- [Siphash 40 MDzip With Decrement Key](https://github.com/singularian/mdencode/blob/master/docs/tests/mdzip_siphash40_test_dec.md)  
- [Siphash 40 MDzip With Increment Key](https://github.com/singularian/mdencode/blob/master/docs/tests/mdzip_siphash40_test_inc.md)  
- [Siphash 47 MDzip With No Increment Key](https://github.com/singularian/mdencode/blob/master/docs/tests/mdzip_siphash47_test_noinc.md)

# MDzip and MDunzip Overview

mdzip is the C++ and GO command line utility can compress a file into a md binary byte block files. 
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

# MDZip Usage

- [MDZip C++ Usage](https://github.com/singularian/mdencode/blob/master/docs/mdzip_cpp.md)
- [MDZip GO Usage](https://github.com/singularian/mdencode/blob/master/docs/mdzip_go.md)

# MDZip Video Example 

These are some Asciinema Video Examples 

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
MDEncode was developed by Scott Ross on a Ryzen 2700x and Ryzen 3950x CPU.

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
