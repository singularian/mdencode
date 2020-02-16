# Project MDencode MDzip Overview  
  
[Decoder Parallel Examples](https://github.com/singularian/mdencode/blob/master/examples/mdencodeParallelmodscan2.txt)

# MDzip and MDunzip Overview

mdzip is the golang command line utility can compress a file into a md binary byte block files.
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

MDZip uses modular floor equation with two forms. One is the exponential floor while the other contains a multiple of the modular floor plus the remainder.
A modulus should be considered part of the file signature subset. A modulus is a digital signature.

![](https://github.com/singularian/mdencode/blob/master/diagrams/modularfloor.png)  
![](https://github.com/singularian/mdencode/blob/master/diagrams/modularfloorexponent.png)  


# Parallel Modulus Scan

The Modulus Scan can also be run in parallel. An example program decoderRandom shows a modulus scan with configurable number of goroutines.
Each of the goroutines runs a modulus scan in sync and when a result is founding matching the original signature block it returns a byteblock.

This is the usage of the prototype parallel modulus scan program.
It allows for the blocksize and modulus bitsize and thread count as well as the bytes either random or specified by the command line argument.
```
user@server:~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom
Usage  ./decoderRandom  -block=[BLOCKSIZE BYTES] -mod=[MODSIZE BITS] -thread=[THREADSIZE GOROUTINES] -start=[THREAD START] -end=[THREAD END] -bytes=[OPTIONAL JSON BYTESTRING]
Usage  ./decoderRandom  -block=12 -mod=64 -thread=16
Usage  ./decoderRandom  -block=8 -mod=64 -thread=10 -bytes=[1,2,3,4,5]
Usage  ./decoderRandom  -block=8 -mod=64 -thread=10 -bytes=[100,222,30,55,100]
Usage  ./decoderRandom  -block=8 -mod=64 -thread=10 -start=2 -end=5 -bytes=[100,222,30,55,100,11,123]
```

This is a 17 byte block with a 64 bit modulus.
The byte block was found in 1.87 seconds.

```
user@server:~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -mod=64 -thread=16 -bytes=[0,0,1,0,0,1,0,2,0,1,255,2,4,1,6,8,10]
buffer  [0 0 1 0 0 1 0 2 0 1 255 2 4 1 6 8 10]
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  17  byteblock  [0 0 1 0 0 1 0 2 0 1 255 2 4 1 6 8 10]
Found Block  [0 0 1 0 0 1 0 2 0 1 255 2 4 1 6 8 10]
Total time  1.873573s
random bytestring and modulusscan bytestring match  [0 0 1 0 0 1 0 2 0 1 255 2 4 1 6 8 10]   [0 0 1 0 0 1 0 2 0 1 255 2 4 1 6 8 10]
Found block  thread 0 random bytestring and modulusscan bytestring match [0 0 1 0 0 1 0 2 0 1 255 2 4 1 6 8 10] [0 0 1 0 0 1 0 2 0 1 255 2 4 1 6 8 10]
```

This is an example of a parallel modulus scan with 16 threads.
[Decoder Parallel Examples](https://github.com/singularian/mdencode/blob/master/examples/mdencodeParallelmodscan2.txt)


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

- [Example Usage](https://github.com/singularian/mdencode/blob/master/docs/Usage.md)

# MDZip Video Example 

These are some Ascii Art Video Examples 


[MDZip Example](https://asciinema.org/a/293119)  
[![MDzip Example](https://i.imgur.com/293119.png)](https://asciinema.org/a/293119)

# MDZip Example 2
[![MDZip Example 2](https://i.imgur.com/293112.png)](https://asciinema.org/a/293112) 

# MDZip Highway Hash Example  
[![MDZip Highway Hash Key Example](https://i.imgur.com/293740.png)](https://asciinema.org/a/293740)   
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

# Donations

* BTC: `32L3gyJdkiFwgJ72rsSzbCBjTjVFxJLHPm`
* ETH: `0x137eB09cF2e199dA2Aa79607f96719be1491a312`
* LTC: `MJh8XpUQ5wm8x1HCoQ7dewNqhrLfZZc31x`

# Author
MDEncode MDzip and MDunzip was developed by Scott Ross on a Ryzen CPU.

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
