# Project MDencode MDzip Overview  
  
MDzip is the golang command line utility can compress a file into a MDzip binary byte block file.  
MDunzip will decompress the input signature binary block file to an output file.   
MDunzip and MDzip use a Hash Context list to encode and decode a file.  
MDZip uses a Prefix binary file with file and byte signatures with a modular floor. It can also use a signature keylist.  
MDzip takes an optional input key for the 32 byte Highway Hash 64 and Highway Hash 128 and Highway Hash 256. It can also take input keylist parameters for other signature keys.  
MDunzip will decompress the input signature binary block file to an output file.  
Block size is limited by the number of CPU threads and processors.   
MZunzip runs a mutlithreaded modulus scan on the MDzip file and writes the results to an unzipped output file.  

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

The Modulus Scan is run in parallel within MDZip.  The modulus scan with configurable number of goroutine threads.
Each of the goroutines runs a modulus scan in sync and when a result is founding matching the original signature block it returns a byteblock.

It allows for the blocksize and modulus bitsize and thread count as well as the bytes either random or specified by the command line argument.

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

[MD Encode Main Example Usage](https://github.com/singularian/mdencode/blob/master/docs/Usage.md)

This is the MDZip Example Usage

These are the 3 main programs.

- mdzip    - creates an mdzip binary file with a modular floor
- mdunzip  - unzips and decodes an mdzip file to an output file
- mdlist   - displays the block signatures of an mdzip file
                                                                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./mdzip                     
USAGE of ./mdzip:                                                                                   
                                                                                                    
  -file string                                                                                      
        Input Filename                                                                              
  -block string                                                                                     
        File Block Size Bytes (default "40")                                                        
  -fh string                                                                                        
        File Hash Bit String List (default "01001")  
  -bh string                                                                                        
        Block Hash Bit String List (default "01001")  
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
hw64        - 1 to 64 hex characters
hw128       - 1 to 64 hex characters
hw256       - 1 to 64 hex characters
murmur3     - uint64 integer 1 to 18 numbers
sip64       - 1 to 32 hex characters
sip128      - 1 to 32 hex characters
xxhash      - uint64 integer 1 to 18 numbers

Keylist Examples:
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=1 -fh=110011 -keylist=aes8:12345
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000001 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000000000000000000111 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E,hw128:000FFFFFFF,hw256:0011
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=00000000000000000000000000000000000000000000001 -fh=110011 -keylist=sip64:FFF11CCAA09
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000000000000000000000000000000000000000000000001 -fh=110011 -keylist=sip128:FFF11CCAA09
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=00000000000000000000000000000000000000000000000000000001 -fh=110011 -keylist=xxhash:112222201
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000011 -fh=110011 -keylist=blake2s_128:123456789012345672222211000,blake2s_256:123456789A12345F
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=01 -fh=110011 -keylist=ax1:123456,ax2:789347

Build Time: 2020-01-18T13:42:02-0500    
Version:    1.0.4 復甦 復活
```                                                                                                 
                                                                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./mdunzip                   
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

# MDZip Highway Hash Example 

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

# MDzip Test

The MDzip provides a commandline modulus test scan with the hash context list.  It generates a random byte block n-bytes or can use a hex or decimal byte block of n-bytes.  
It also can use a N-bit modulus and a hash list and signature key as well as the thread number and then runs a modulus scan and compares results.  
If the byte blocks match the modulus scan result is a success.  

path: github.com/singularian/mdencode/code/testdecode/cmd/decoderRandomTestHC.go

This is the Usage for decoderRandomTestHC.

```
Usage  ./decoderRandomTestHC  -block=[BLOCKSIZE BYTES] -mod=[MODSIZE BITS] -thread=[THREADSIZE GOROUTINES] -start=[THREAD START] -end=[THREAD END] -bytes=[OPTIONAL JSON BYTESTRING] -hex=[OPTIONAL HEX BYTESTRING] keylist=[OPTIONAL HEX BYTESTRING]
Usage  ./decoderRandomTestHC  -block=12 -mod=64 -thread=16 -bh=1010101
Usage  ./decoderRandomTestHC  -block=9 -mod=64 -thread=10 -bh=11111 -bytes=[1,2,3,4,5]
Usage  ./decoderRandomTestHC  -block=8 -mod=64 -thread=10 -bh=1 -bytes=[100,222,30,55,100]
Usage  ./decoderRandomTestHC  -block=8 -mod=64 -thread=10 -bh=101 -hex=FF0C3FDDAF
Usage  ./decoderRandomTestHC  -block=20 -mod=128 -thread=16 -bh=0000000000000000001 -keylist=aes8:F01100119900112FF11
Usage  ./decoderRandomTestHC  -mod=64 -thread=16 -start=2 -end=5 -bytes=[100,222,30,55,100,11,123]
Usage  ./decoderRandomTestHC  -mod=64 -thread=16 -start=2 -end=5 -hex=0F0F0F22CDFF
```

Example decoderRandomTestHC Tests
```
./decoderRandomTestHC -block=12 -mod=64 -thread=16 -bh=1 -keylist=aes8:F0110033119900112FF11
hashlist  [aes8] 1
Found Block [aes8 777f8b3e50699983] result = 777f8b3e50699983 Found Block  [78 185 242 227 135 253 126 154 144 154 16 165]
Total time  9.5400239s
random bytestring and modulusscan bytestring match  [78 185 242 227 135 253 126 154 144 154 16 165]   [78 185 242 227 135 253 126 154 144 154 16 165]  hex bytes  4EB9F2E387FD7E9A909A10A5
Found block  thread 3 random bytestring and modulusscan bytestring match [78 185 242 227 135 253 126 154 144 154 16 165] = [78 185 242 227 135 253 126 154 144 154 16 165]

./decoderRandomTestHC -block=20 -mod=128 -thread=16 -bh=1 -keylist=aes8:F01100119900112FF11
hashlist  [aes8] 1
Found Block [aes8 81def47df5b10ba2] result = 81def47df5b10ba2 Found Block  [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244]
Total time  29.688835s
random bytestring and modulusscan bytestring match  [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244]   [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244]  hex bytes  3826CB1F1497B2B43A9ACCF5B3D89348B675D8F4
Found block  thread 15 random bytestring and modulusscan bytestring match [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244] = [56 38 203 31 20 151 178 180 58 154 204 245 179 216 147 72 182 117 216 244]


 ./decoderRandomTestHC -block=20 -mod=128 -thread=16 -bh=0000000000000000001 -keylist=aes8:F01100119900112FF11
hashlist  [hmac256] 0000000000000000001
Found Block [hmac256 baf64c746d51cc640faa447da3a4caecc6f35ca2dc42dd5f22bd94ed96f6697f] result = baf64c746d51cc640faa447da3a4caecc6f35ca2dc42dd5f22bd94ed96f6697f Found Block  [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51]
Total time  3m27.7438341s
random bytestring and modulusscan bytestring match  [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51]   [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51]  hex bytes  DEFB113AFD9A2D02C52755EF90B2BBACAB758733
Found block  thread 10 random bytestring and modulusscan bytestring match [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51] = [222 251 17 58 253 154 45 2 197 39 85 239 144 178 187 172 171 117 135 51]
```

# Donations

* BTC: `32L3gyJdkiFwgJ72rsSzbCBjTjVFxJLHPm`
* ETH: `0x137eB09cF2e199dA2Aa79607f96719be1491a312`
* LTC: `MJh8XpUQ5wm8x1HCoQ7dewNqhrLfZZc31x`

# Author
MDEncode MDzip and MDunzip was developed by Scott Ross on a Ryzen CPU.

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
