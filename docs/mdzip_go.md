# Project MDencode Prototype MDzip GO Overview  
  
MDzip GO is a command line utility can compress a file into a MDzip binary byte block file.  
MDunzip will decompress the input signature binary block file to an output file.   
MDunzip and MDzip use a Hash Context list to encode and decode a file.  
MDZip uses a Prefix binary file with file and byte signatures with a modular floor. It can also use a signature keylist.  
MDzip takes an optional input key for the 32 byte Highway Hash 64 and Highway Hash 128 and Highway Hash 256. It can also take input keylist parameters for other signature keys.  
MDunzip will decompress the input signature binary block file to an output file.  
Block size is limited by the number of CPU threads and processors.   
MZunzip runs a mutlithreaded modulus scan on the MDzip file and writes the results to an unzipped output file.  
It currently has a different format than the C++ mdzip encoder.

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
- File Signature List post validation
- Uses the MDZip Signature Keys to change the output file signature Modulus Scan

# MDZip File Format

This is the MDZip file format

[File Prefix]  
[File Signature Blocks]  
[File Block Signature Blocks]  


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

The Modulus Scan is run in parallel within MDunzip.  The modulus scan is configurable number of goroutine threads and the start and end thread.  
Each of the goroutines runs a modulus scan in sync and when a result is founding matching the original signature block it returns a byteblock.

It allows for the blocksize and modulus bitsize and thread count as well as the bytes either random or specified by the command line argument.

# MDZip Examples

These are MDzip and MDunzip command line examples. They also illustrate setting the Highway Hash 32 byte signature key. MDunzip will use the specified Highway Hash Key.  

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

[MD Encode Main Example Usage](https://github.com/singularian/mdencode/blob/master/docs/Usage.md)

This is the MDZip Example Usage

These are the 3 main programs.

- mdzip    - creates an mdzip binary file with a modular floor
- mdunzip  - unzips and decodes an mdzip file to an output file
- mdlist   - displays the block signatures of an mdzip file
                                                                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_go$ ./mdzip                     
USAGE of ./mdzip:                                                                                   
                                                                                                    
  -file string                                                                                      
        Input Filename                                                                              
  -block string                                                                                     
        File Block Size Bytes (default "40")                                                        
  -fh string                                                                                        
        File Hash List (default "011") CSV or Binary  
  -bh string                                                                                        
        Block Hash List (default "01001") CSV or Binary 
  -uh string
        Quarternian Hash String List
  -mod string                                                                                       
        Modulus Size in Bits (default "32")                                                         
  -format int                                                                                       
        Output Format (default 10)                                                                  
  -out string                                                                                       
        Output Filename                                                                             
  -keylist string
        Signature Hash Keylist
  -log string                                                                                       
        Log Filename  


MDzip Examples:
mdzip -mod=64 -block=12 -file=randomfile -out=randomfile.mdz -bh=000000000001001 -fh=111
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000001 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E

MDunzip Examples:
mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -thread=16 -val

Signature Keys Format:
A signature Key parameter "-keylist" is delineatead with a keyname colon hexvalue comma

  -keylist=keyname:hexvalue  
  -keylist=keyname:hexvalue,key2:hexvalue  
  -keylist=keyname:hexvalue,key2:hexvalue  
  -keylist=keyname:hexvalue,key2:hexvalue,key3:keyvalue,...  

Keylist:
aes8        - uint64 integer 1 to 18 numbers                       
ax1         - (xxhash128 key 1) uint64 integer 1 to 18 numbers     
ax2         - (xxhash128 key 2) uint64 integer 1 to 18 numbers     
blake2s_128 - 16+ hex characters                                   
blake2s_256 - 16+ hex characters                                   
ct64        - uint64 integer 1 to 18 numbers                       
fh32        - uint64 integer 1 to 18 numbers                       
fh64        - uint64 integer 1 to 18 numbers                       
hw32        - 1 to 64 hex characters                               
hw64        - 1 to 64 hex characters                               
hw128       - 1 to 64 hex characters                               
hw256       - 1 to 64 hex characters                               
jn64        - uint64 integer 1 to 18 numbers                       
me32        - uint64 integer 1 to 18 numbers                       
me64        - uint64 integer 1 to 18 numbers                       
me128       - uint64 integer 1 to 18 numbers                       
mm32        - uint32 integer 1 to 9 numbers                        
murmur3     - uint64 integer 1 to 18 numbers                       
sip64       - 1 to 32 hex characters                               
sip128      - 1 to 32 hex characters                               
spk32       - uint64 integer 1 to 18 numbers                       
spk64       - uint64 integer 1 to 18 numbers                       
t1ha        - uint64 integer 1 to 18 numbers                       
xxh32       - uint32 integer 1 to 9 numbers                        
xxhash64    - uint64 integer 1 to 18 numbers                       
wy          - uint64 integer 1 to 18 numbers                                                                        

Keylist Examples:
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=11 -fh=110011 -keylist=aes8:12345
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000001 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000000000000000000111 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E,hw128:000FFFFFFF,hw256:0011
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=00000000000000000000000000000000000000000000001 -fh=110011 -keylist=sip64:FFF11CCAA09
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000000000000000000000000000000000000000000000001 -fh=110011 -keylist=sip128:FFF11CCAA09
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=00000000000000000000000000000000000000000000000000000001 -fh=110011 -keylist=xxhash:112222201
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000011 -fh=110011 -keylist=blake2s_128:123456789012345672222211000,blake2s_256:123456789A12345F
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=01 -fh=110011 -keylist=ax1:123456,ax2:789347
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=1-5,6,7,11 -fh=110011 -keylist=ax1:123456,ax2:789347

Build Time: 2020-01-18T13:42:02-0500    
Version:    1.0.8 復甦 復活
```                                                                                                 
                                                                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_go$ ./mdunzip                   
USAGE of mdunzip:
                                                                                                    
  -file string                                                                                      
        Input Filename                                                                              
  -out string                                                                                       
        Output Filename                                                                             
  -thread string                                                                                    
        Go Routine Threadsize   
  -val bool
        Run the File Hash List Post Validation

Examples:
mdzip -mod=64 -block=12 -file=randomfile -out=randomfile.mdz -bh=000000000001001 -fh=111
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000001 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E
mdunzip -file=randomfile.mdz -out=randomfile.mdz.out
mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -val
```                                                                                                 
                                                                                                    
mdlist can display the contents of an mdzip file                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./mdlist
USAGE of ./mdlist:  
Examples:  
./mdlist  [FILENAME]   
```

mdcmd is a command line modulus scan decoder that takes and input signature list and block parameters and runs the modulus scan
```
USAGE of mdcmd:

  -block string
        File Block Size Bytes
  -mod
        Modulus Size in Bits
  -h
        Block Hash String List aes8:sha1
  -bh
        Block Hash Bit String List
  -keylist
        Signature Key List
  -hex
        Specify a Hash Byte Block HEX byte string
  -exp
        Modulus Exponent
  -rem
        Modulus Remainder
  -out
        Output Filename
  -start
        Thread Start (Allows threads to be divided between multiple computers)
  -end
        Thread End (Allows threads to be divided between multiple computers)
  -thread string
        Go Routine Threadsize


Examples:
mdcmd -block=10 -mod=64 -h=sha1_64 -hex=17af4b9d3c68b6bb -exp=79 -rem=5324266269509948425 -thread=16
```

# MDZip Video Example 

These are some Ascii Art Video Examples 

# MDZip Example 1

[![MDzip Example](https://asciinema.org/a/293119.png)](https://asciinema.org/a/293119)

# MDZip Example 2

[Example 2](https://asciinema.org/a/293112)  

# MDZip Highway Hash Example 3

[![MDZip Highway Hash Key Example](https://asciinema.org/a/293740.png)](https://asciinema.org/a/293740)   


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
- Change the Modulus number

# Build

This is a guide for building MDencode on Linux.

[Example Build](https://github.com/singularian/mdencode/blob/master/docs/Build.md)

# Docker Container  

A Docker container was also created in Dockerhub.

[Docker Container](https://hub.docker.com/r/annatarsol/mdencode)

# MDzip Test

The MDzip provides a commandline modulus test scan with the hash context list.  It generates a random byte block n-bytes or can use a hex or decimal byte block of n-bytes.  
It also can use a N-bit modulus and a hash list and signature key as well as the thread number and then runs a modulus scan and compares results.  
If the byte blocks match the modulus scan result is a success.  

path: github.com/singularian/mdencode/code/testdecode_go/cmd/mdtestHC.go

This is the Usage for mdtest.

```
USAGE of ./mdtest:

  -block string
        File Block Size Bytes (default "40")
  -all
        Run all the block signatures
  -bh string
        Block Hash List (default "01001") CSV or Binary
  -mod string
        Modulus Size in Bits (default "32")
  -thread
        Thread Number
  -start
        Thread Start
  -end
        Thread End
  -bytes
        JSON Bytestring
  -hex
        Hex Bytestring
  -keylist string
        Signature Hash Keylist

Usage  ./mdtest  -block=12 -mod=64 -thread=16 -bh=1010101
Usage  ./mdtest  -block=9 -mod=64 -thread=10 -bh=11111 -bytes=[1,2,3,4,5]
Usage  ./mdtest  -block=8 -mod=64 -thread=10 -bh=1 -bytes=[100,222,30,55,100]
Usage  ./mdtest  -block=8 -mod=64 -thread=10 -bh=101 -hex=FF0C3FDDAF
Usage  ./mdtest  -block=20 -mod=128 -thread=16 -bh=0000000000000000001 -keylist=aes8:F01100119900112FF11
Usage  ./mdtest  -mod=64 -thread=16 -start=2 -end=5 -bytes=[100,222,30,55,100,11,123]
Usage  ./mdtest  -mod=64 -thread=16 -start=2 -end=5 -hex=0F0F0F22CDFF
Usage  ./mdtest  -block=11 -mod=64 -bh=1,5,15,16 -thread=16
```

## Example 1: mdtest Tests  

These are some examples for mdtest. The Hashlist Context test program. 

```
./mdtest -block=12 -mod=64 -thread=16 -bh=1 -keylist=aes8:F0110033119900112FF11
hashlist  [aes8] 1
Found Block [aes8 777f8b3e50699983] result = 777f8b3e50699983 Found Block  [78 185 242 227 135 253 126 154 144 154 16 165]
Total time  9.5400239s
random bytestring and modulusscan bytestring match  [78 185 242 227 135 253 126 154 144 154 16 165]   [78 185 242 227 135 253 126 154 144 154 16 165]  hex bytes  4EB9F2E387FD7E9A909A10A5
Found block  thread 3 random bytestring and modulusscan bytestring match [78 185 242 227 135 253 126 154 144 154 16 165] = [78 185 242 227 135 253 126 154 144 154 16 165]

./mdtest -block=20 -mod=128 -thread=16 -bh=1 -keylist=aes8:F01100119900112FF11
hashlist  [aes8] 1
Found Block [aes8 81def47df5b10ba2] result = 81def47df5b10ba2 Found Block  [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244]
Total time  29.688835s
random bytestring and modulusscan bytestring match  [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244]   [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244]  hex bytes  3826CB1F1497B2B43A9ACCF5B3D89348B675D8F4
Found block  thread 15 random bytestring and modulusscan bytestring match [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244] = [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244]


./mdtest -block=20 -mod=128 -thread=16 -bh=0000000000000000001 -keylist=aes8:F01100119900112FF11
hashlist  [hmac256] 0000000000000000001
Found Block [hmac256 baf64c746d51cc640faa447da3a4caecc6f35ca2dc42dd5f22bd94ed96f6697f] result = baf64c746d51cc640faa447da3a4caecc6f35ca2dc42dd5f22bd94ed96f6697f Found Block  [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51]
Total time  3m27.7438341s
random bytestring and modulusscan bytestring match  [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51]   [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51]  hex bytes  DEFB113AFD9A2D02C52755EF90B2BBACAB758733
Found block  thread 10 random bytestring and modulusscan bytestring match [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51] = [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51]

```

## Example 2                                                                                                                                                                  
                                                                                                                                                                              
This examples shows a 14 byte block encoded in a 13 byte block. This is an example of modulus scan compression where the mdzip byte block is smaller than the input block.    
It uses 2 32-bit signatures CRC-32 and FNV-32 and a 32-bit modulus and 1 byte for the modulus exponent.                                                                       
```                                                                                                                                                                           
user@server:~/projects/src/github.com/singularian/mdencode/build$ ./mdtestgo -block=14 -mod=32 -bh=0000000010000001 -thread=16 -hex=0000000012002000000000000000   
hashlist  [crc32 fnv32] 0000000010000001                                                                                                                                      
Found Block [crc32 0b964a44 fnv32 7c87ac27] result = 0b964a447c87ac27 Found Block  [0 0 0 0 18 0 32 0 0 0 0 0 0 0]                                                            
Total time  25h57m30.5502149s                                                                                                                                                 
random bytestring and modulusscan bytestring match  [0 0 0 0 18 0 32 0 0 0 0 0 0 0]   [0 0 0 0 18 0 32 0 0 0 0 0 0 0]  hex bytes  0000000012002000000000000000                
Found block  thread 0 random bytestring and modulusscan bytestring match [0 0 0 0 18 0 32 0 0 0 0 0 0 0] = [0 0 0 0 18 0 32 0 0 0 0 0 0 0]                                    
                                                                                                                                                                              
```                                                                                                                                                                           
                                                                                                                                                                              
## Example 3                                                                                                                                                                  
                                                                                                                                                                              
This examples shows a 14 byte block encoded in a 13 byte block. This is an example of modulus scan compression where the mdzip byte block is smaller than the input block.    
It uses 2 32-bit signatures CRC-32 and FNV-32 and a 32-bit modulus and 1 byte for the modulus exponent. It was run on a 60 core server with 120 threads.                      
                                                                                                                                                                              
```                                                                                                                                                                           
user@instance-2:~/projects/src/github.com/singularian/mdencode/build$ ./mdtestgo -block=14 -mod=32 -bh=0000000010000001 -thread=120 -hex=000000001200200000000000FFFC                   
Running Decode                                                                                                                                                                
Block Size        14                                                                                                                                                          
Hashlist          [crc32 fnv32]                                                                                                                                               
Binary Hashlist   0000000010000001                                                                                                                                            
Thread Size       120                                                                                                                                                         
Mod Size          32                                                                                                                                                          
Mod Exponent      76                                                                                                                                                          
Mod Remainder     65532                                                                                                                                                       
                                                                                                                                                                              
Found Block [crc32 2cb90901 fnv32 41874fba] result = 2cb9090141874fba Found Block  [0 0 0 0 18 0 32 0 0 0 0 0 255 252]                                                        
Total time  17h2m51.520645146s                                                                                                                                                
Random bytestring and modulusscan bytestring match  [0 0 0 0 18 0 32 0 0 0 0 0 255 252]   [0 0 0 0 18 0 32 0 0 0 0 0 255 252]  hex bytes  000000001200200000000000FFFC        
Found block  thread 64 random bytestring and modulusscan bytestring match [0 0 0 0 18 0 32 0 0 0 0 0 255 252] = [0 0 0 0 18 0 32 0 0 0 0 0 255 252]                           
```                                                                                                                                                                           
                                                                                                                                                                              
## Example 4                                                                                                                                                                  
                                                                                                                                                                              
This examples shows a 14 byte block encoded in a 13 byte block. This is an example of modulus scan compression where the mdzip byte block is smaller than the input block.    
It uses 2 32-bit signatures CRC-32 and FNV-32 and a 32-bit modulus and 1 byte for the modulus exponent.                                                                       
It was run on 9 servers with 32 cores with a total of 288 cpu threads.                                                                                                        
                                                                                                                                                                              
```                                                                                                                                                                           
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=0 -end=32  -hex=0000000012002000000011DDFFFC                                                  
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=32 -end=64  -hex=0000000012002000000011DDFFFC                                                 
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=64 -end=96  -hex=0000000012002000000011DDFFFC                                                 
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=96 -end=128  -hex=0000000012002000000011DDFFFC                                                
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=128 -end=160 -hex=0000000012002000000011DDFFFC                                                
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=160 -end=192 -hex=0000000012002000000011DDFFFC                                                
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=192 -end=224 -hex=0000000012002000000011DDFFFC                                                
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=224 -end=256 -hex=0000000012002000000011DDFFFC                                                
./mdtest -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=256 -end=288 -hex=0000000012002000000011DDFFFC                                                
                                                                                                                                                                              
Thread End        96                                                                                                                                                          
Threads           288                                                                                                                                                         
Mod Size          32                                                                                                                                                          
Mod Exponent      76                                                                                                                                                          
Mod Remainder     382551851004 - TODO: It is currently using mod start modremainder + thread number + mod                                                                     
Found Block [crc32 414d9f98 fnv32 06e18008] result = 414d9f9806e18008 Found Block  [0 0 0 0 18 0 32 0 0 0 17 221 255 252]                                                     
Total time  2h24m0.469392664s                                                                                                                                                 
Random bytestring and modulusscan bytestring match  [0 0 0 0 18 0 32 0 0 0 17 221 255 252]   [0 0 0 0 18 0 32 0 0 0 17 221 255 252]  hex bytes  0000000012002000000011DDFFFC  
Found block  thread 64 random bytestring and modulusscan bytestring match [0 0 0 0 18 0 32 0 0 0 17 221 255 252] = [0 0 0 0 18 0 32 0 0 0 17 221 255 252]                     
```

## Example 5

This example uses a 13 byte hash block with high way hash 64 (8 bytes) plus a 32-bit modulus + 1 byte for the exponent to encode a  
14 byte block. Resulting in one byte savings.

```
./mdtest.exe -block=14 -mod=32 -bh="31," -thread=16 -hex=0000000012002000000211DDFFFC
Running Decode

Start Time        2020-04-13 12:55:34.1399714 -0400 EDT m=+0.004789501
Block Size        14
Block Bytes       00 00 00 00 12 00 20 00 00 02 11 DD FF FC
Block Bytes       0000000012002000000211DDFFFC
Input Hashlist    31,
Hashlist          [hw64]
Hashlist Size     1
Thread Start      0
Thread End        16
Threads           16
Mod Size          32
Mod Exponent      76
Mod Remainder     299761660

Found Block [hw64 1d6681286cb63ced] result = 1d6681286cb63ced Found Block  [0 0 0 0 18 0 32 0 0 2 17 221 255 252]
Total time  28h7m5.911036s
Random bytestring and modulusscan bytestring match  [0 0 0 0 18 0 32 0 0 2 17 221 255 252]   [0 0 0 0 18 0 32 0 0 2 17 221 255 252]  hex bytes  0000000012002000000211DDFFFC
Found block  thread 2 random bytestring and modulusscan bytestring match [0 0 0 0 18 0 32 0 0 2 17 221 255 252] = [0 0 0 0 18 0 32 0 0 2 17 221 255 252] 
```

## Example 6

This is the run on the new C++ test program with a ryzen 3950x using fasthash 64 and a 14 byte block with a 32 bit modulus.
It decompressed the 13 byte block to a 14 byte block (8 bytes fasthash 64 + 4 bytes modulus + 1 byte exponent) in 4.05 hours. 

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

Elapsed Time (s) 14601.1   = 4.05 hours
Modulus Scan thread 17 and Random byteblock match
0 0 0 0 18 17 33 17 17 17 17 34 255 252 
0 0 0 0 18 17 33 17 17 17 17 34 255 252 
```

# Donations

* BTC: `32L3gyJdkiFwgJ72rsSzbCBjTjVFxJLHPm`
* ETH: `0x137eB09cF2e199dA2Aa79607f96719be1491a312`
* LTC: `MJh8XpUQ5wm8x1HCoQ7dewNqhrLfZZc31x`

# Author
MDEncode MDzip and MDunzip was developed by Scott Ross on a Ryzen CPU.

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
