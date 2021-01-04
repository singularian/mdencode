# Project MDencode MDzip Overview  
  
This is the MDzip C++ command line utility can compress a file into a MDzip binary byte block file.  
MDunzip will decompress the input signature binary block file to an output file.   
MDunzip and MDzip use a Hash Context list to encode and decode a file.  
MDZip uses a Prefix binary file with file and byte signatures with a modular floor. It can also use a signature keylist.  
MDzip takes an optional input key for the 32 byte Highway Hash 64 and Highway Hash 128 and Highway Hash 256. It can also take input keylist parameters for other signature keys.  
MDunzip will decompress the input signature binary block file to an output file.  
Block size is limited by the number of CPU threads and processors.   
MZunzip runs a mutlithreaded modulus scan on the MDzip file and writes the results to an unzipped output file.  
It currently doesn't use the same format as the GO version and has a different signature hash context list.

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

These are MDzip and MDunzip command line examples. It doesn't currently support the setting of the hash key for the C++ version.

```bash
mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4 
```

# MDunzip Examples

```bash

mdunzip --file=phone.txt.mdz --threads=32
mdunzip --file=filename.mdz --thread=16 
mdunzip --file=test.mdz --thread=16 
mdunzip --file=test.mdz --list=true
mdunzip --file=filename.mdz --list=true --unzip=false
```

# MDZip C++ Usage

[MD Encode Main Example Usage](https://github.com/singularian/mdencode/blob/master/docs/Usage.md)

This is the MDZip Example Usage

These are the 3 main programs.

- mdzip    - creates an mdzip binary file with a modular floor
- mdunzip  - unzips or displays the byte block and decodes an mdzip file to an output file
                                                                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_cpp$ ./mdzip                     
MDEncode MDzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDzip filename
  -b,--block INT:POSITIVE:INT in [1 - 100]
                              Blocksize number
  -m,--mod INT:POSITIVE       Modulus size number
  -t,--threads INT:POSITIVE   Thread count number
  --fhs INT:POSITIVE:INT in [1 - 34] ...
                              File Hashlist csv string
  --fh INT:POSITIVE:INT in [1 - 34] ...
                              File Hashlist integers list
  -r,--bhs INT:POSITIVE:INT in [1 - 34] ...
                              Block Hashlist csv string
  -s,--bh INT:POSITIVE:INT in [1 - 34] ...
                              Block Hashlist integers list
  -k,--keylist TEXT           Keylist csv string
  --rand BOOLEAN              Randomize Keylist
  -x,--list BOOLEAN           Display the Block list
  -l,--log BOOLEAN            Run Logging



Examples:
   ./mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
   ./mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
   ./mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4 


Hashlist:

ID          Hash Name   Description                   Key         Blocksize   
==============================================================================
1           cit64       Cityhash 64                   false       8           
2           crc32       CRC 32                        false       4           
3           crc64       CRC 64                        false       8           
4           fast32      Fasthash 32                   true        4           
5           fast64      Fasthash 64                   true        8           
6           fnv32       FNV-1  32                     false       4           
7           fnv32a      FNV-1a 32                     false       4           
8           fnv64       FNV-1  64                     false       8           
9           fnv64a      FNV-1a 64                     false       8           
10          hw64        Highway Hash 64               true        8           
11          md2         MD2                           false       16          
12          md4         MD4                           false       16          
13          md5         MD5                           false       16          
14          md6         MD6                           false       20          
15          md62        MD6 Quicker                   true        20          
16          met641      Metro Hash 64 v1              true        8           
17          met642      Metro Hash 64 v2              true        8           
18          mx3         MX3                           true        8           
19          png         Pengyhash 64                  true        8           
20          ripe160     Ripe MD 160                   false       20          
21          sea64       Seahash 64                    true        8           
22          sip64       Siphash 64                    true        8           
23          sha1_64     SHA1 64                       false       8           
24          sha1_128    SHA1 128                      false       16          
25          sha1        SHA1                          false       20          
26          sha256      SHA 256                       false       32          
27          sha384      SHA 384                       false       48          
28          sha512      SHA 512                       false       64          
29          spk32       Spooky 32                     true        4           
30          spk64       Spooky 64                     true        8           
31          xxh32       xxHash32                      true        4           
32          xxh64       xxHash64                      true        8           
33          whp         Whirlpool                     false       64          
34          wy64        WYhash 64                     true        8       
```                                                                                                 
                                                                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_cpp$ ./mdunzip                   
MDEncode MDunzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDunzip filename
  -t,--threads INT:POSITIVE   Thread count number
  -l,--list BOOLEAN           List the mdzip file
  -u,--unzip BOOLEAN          mdunzip a file
  -o,--over BOOLEAN           Overwriting an existing mdunzip output file
  --log BOOLEAN               Run Logging



Examples:
   ./mdunzip --file=filename.mdz --thread=16 
   ./mdunzip --file=test.mdz --thread=16 
   ./mdunzip --file=test.mdz --list=true
   ./mdunzip --file=filename.mdz --list=true --unzip=false


Hashlist:

ID          Hash Name   Description                   Key         Blocksize   
==============================================================================
1           cit64       Cityhash 64                   false       8           
2           crc32       CRC 32                        false       4           
3           crc64       CRC 64                        false       8           
4           fast32      Fasthash 32                   true        4           
5           fast64      Fasthash 64                   true        8           
6           fnv32       FNV-1  32                     false       4           
7           fnv32a      FNV-1a 32                     false       4           
8           fnv64       FNV-1  64                     false       8           
9           fnv64a      FNV-1a 64                     false       8           
10          hw64        Highway Hash 64               true        8           
11          md2         MD2                           false       16          
12          md4         MD4                           false       16          
13          md5         MD5                           false       16          
14          md6         MD6                           false       20          
15          md62        MD6 Quicker                   true        20          
16          met641      Metro Hash 64 v1              true        8           
17          met642      Metro Hash 64 v2              true        8           
18          mx3         MX3                           true        8           
19          png         Pengyhash 64                  true        8           
20          ripe160     Ripe MD 160                   false       20          
21          sea64       Seahash 64                    true        8           
22          sip64       Siphash 64                    true        8           
23          sha1_64     SHA1 64                       false       8           
24          sha1_128    SHA1 128                      false       16          
25          sha1        SHA1                          false       20          
26          sha256      SHA 256                       false       32          
27          sha384      SHA 384                       false       48          
28          sha512      SHA 512                       false       64          
29          spk32       Spooky 32                     true        4           
30          spk64       Spooky 64                     true        8           
31          xxh32       xxHash32                      true        4           
32          xxh64       xxHash64                      true        8           
33          whp         Whirlpool                     false       64          
34          wy64        WYhash 64                     true        8  
```                                                                                                 
                                                                                                    



# MDZip Video Example 

TODO for the C++ MDzip / MDunzip program  


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

The MDzip C++ engine provides a commandline modulus test scan with the hash context list.  It generates a random byte block n-bytes or can use a hex or decimal byte block of n-bytes.  
It also can use a N-bit modulus and a hash list and signature key as well as the thread number and then runs a modulus scan and compares results.  
If the byte blocks match the modulus scan result is a success.  It displays the hash signature list under the usage.

path: github.com/singularian/mdencode/code/testdecode_cpp/

This is the Usage for decoderRandomTestHC2.

```
MDEncode GMP C++ Test Program
Usage: ./decoderRandomTestHC2 [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -b,--block UINT:POSITIVE:INT in [1 - 100]
                              Blocksize number
  -m,--mod INT:POSITIVE       Modulus size number
  -t,--threads INT:POSITIVE   Thread count number
  -r,--bh INT:POSITIVE:INT in [1 - 34] ...
                              Block Hashlist csv string
  -s,--hl INT:POSITIVE:INT in [1 - 34] ...
                              Block Hashlist integers list
  -k,--keylist TEXT           Keylist csv string
  -x,--hex TEXT               Hex Byteblock string
  -l,--log BOOLEAN            Run Logging



Examples:
   ./decoderRandomTestHCthreads_gmp -b 12 -m 64 -t 16
   ./decoderRandomTestHCthreads_gmp --block=12 --mod=64    --threads=16
   ./decoderRandomTestHCthreads_gmp --block=12 --mod=128   --threads=16
   ./decoderRandomTestHCthreads_gmp --mod=64 --threads=16 --hex=0011
   ./decoderRandomTestHCthreads_gmp --mod=64 --threads=16 --hex=FFd033FF202020202011
   ./decoderRandomTestHCthreads_gmp --mod=64 --threads=16 --hex=FFd033FF202020202011 --log=true --hl 1 2 3 4 5
   ./decoderRandomTestHCthreads_gmp --mod=64 --threads=16 --hex=FFd033FF202020202011 --log=true --bh 1,5,7


Hashlist

ID          Hash Name   Description                   Key         Blocksize   
==============================================================================
1           cit64       Cityhash 64                   false       8           
2           crc32       CRC 32                        false       4           
3           crc64       CRC 64                        false       8           
4           fast32      Fasthash 32                   true        4           
5           fast64      Fasthash 64                   true        8           
6           fnv32       FNV-1  32                     false       4           
7           fnv32a      FNV-1a 32                     false       4           
8           fnv64       FNV-1  64                     false       8           
9           fnv64a      FNV-1a 64                     false       8           
10          hw64        Highway Hash 64               true        8           
11          md2         MD2                           false       16          
12          md4         MD4                           false       16          
13          md5         MD5                           false       16          
14          md6         MD6                           false       20          
15          md62        MD6 Quicker                   true        20          
16          met641      Metro Hash 64 v1              true        8           
17          met642      Metro Hash 64 v2              true        8           
18          mx3         MX3                           true        8           
19          png         Pengyhash 64                  true        8           
20          ripe160     Ripe MD 160                   false       20          
21          sea64       Seahash 64                    true        8           
22          sip64       Siphash 64                    true        8           
23          sha1_64     SHA1 64                       false       8           
24          sha1_128    SHA1 128                      false       16          
25          sha1        SHA1                          false       20          
26          sha256      SHA 256                       false       32          
27          sha384      SHA 384                       false       48          
28          sha512      SHA 512                       false       64          
29          spk32       Spooky 32                     true        4           
30          spk64       Spooky 64                     true        8           
31          xxh32       xxHash32                      true        4           
32          xxh64       xxHash64                      true        8           
33          whp         Whirlpool                     false       64          
34          wy64        WYhash 64                     true        8           
```

## Example 1: decoderRandomTestHC Tests  



# Donations

* BTC: `32L3gyJdkiFwgJ72rsSzbCBjTjVFxJLHPm`
* ETH: `0x137eB09cF2e199dA2Aa79607f96719be1491a312`
* LTC: `MJh8XpUQ5wm8x1HCoQ7dewNqhrLfZZc31x`

# Author
MDEncode C++ MDzip and MDunzip was developed by Scott Ross on a Ryzen CPU.

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
