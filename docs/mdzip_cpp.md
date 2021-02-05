# Project MDencode MDzip C++ Overview  
  
This is the MDzip C++ command line utility can compress a file into a MDzip binary byte block file.  
MDunzip will decompress the input signature binary block file to an output file.   
MDunzip and MDzip use a Hash Context list to encode and decode a file.  
MDZip uses a header prefixed binary file with file and byte block signatures with a modular floor. It can also use a signature keylist.  
MDzip C++ takes an optional input key randomizer for the block hash list keys. It randomizes the supported signature keys and writes them to the mdzip file.    
MDunzip will decompress the input signature binary block file to an output file.  
Block size is limited by the number of CPU threads and processors.   
MZunzip runs a mutlithreaded modulus scan on the MDzip file and writes the results to an unzipped output file.  
It currently doesn't use the same format as the GO version and has a different signature hash context list.
MDunzip also supports a signature post validation and will check if the mdzip file signatures match the uncompressed output file.
Signature post validation can also be used to eliminate collisions if there were any.

**MDZip Features**
- User Specified Modulus Size
- User Specified Block Size
- File Hash Signatures
- Block Hash Signatures
- Optional Signature Keys
- 32 Byte Highway Hash Signature Keys which change the signature (With multiple signature key combinations)
- Lossless compression

**MDUnzip Features**
- Multithreaded Parallel Modulus Scan
- Decrypts an MDZip file to an output file
- Uses the MDZip Signature Keys to change the output file signature Modulus Scan
- File Signature post validation

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

# Modscan Iteration Calculator

This is the modscan iteration number calculator.  It can uses division to determine the number of partitions in a modulus scan iteration.  

Modscan_iterations = (Number / Modulus) + 1   

This is example for 100 with a modulus of 10. There should be 11 iterations.  

11 = (100 / 10) + 1

With 100 mod 20 there should be 6 iterations.

6 = (100 / 20) + 1


#### Modscan Iterations for 100 Mod 10  

This is a list of the modulus scan intervals for 100 Mod 10.  

0   
10  
20  
30  
40  
50  
60  
70  
80  
90  
100  

#### Modscan Iterations for 100 Mod 20

100 / mod 20 = 5 + 1 = 6

0  
20  
40  
60  
80  
100  

#### Modscan Iterations for 14 Bytes Mod 4 bytes

A 14 byte block has 2 ^ 112 (8 bits * 14 bytes) possible bits. 

2 ^ 112 / 2 ^ 32 = 2 ^ 80 + 1 

There are 2 ^ 80 + 1 iterations with 32 bit modulus.
There should be 12 bytes worth of signature and 8 for the fast hash 64 and 4 bytes for the 32 bit modulus to encode 10 bytes of data in the 13/14 encoding.  

# Parallel Modulus Scan

The Modulus Scan is run in parallel within MDunzip.  The modulus scan is configurable number of goroutine threads and the start and end thread.  
Each of the goroutines runs a modulus scan in sync and when a result is founding matching the original signature block it returns a byteblock.

It allows for the blocksize and modulus bitsize and thread count as well as the bytes either random or specified by the command line argument.

# MDZip Examples

These are MDzip and MDunzip command line examples. It doesn't currently support the setting of the hash key for the C++ version.

```bash
mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=true
mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=false
```

# MDunzip Examples

```bash

mdunzip --file=phone.txt.mdz --threads=32
mdunzip --file=filename.mdz --thread=16 
mdunzip --file=test.mdz --thread=16 
mdunzip --file=test.mdz --list=true --val=true
mdunzip --file=filename.mdz --list=true --unzip=false
```

# MDZip C++ Usage

This is the MDZip C++ Example Usage

These are the 5 main programs. 

- mdzip      - Creates an mdzip (mdz) binary file with a modular floor and multiple signatures.
- mdunzip    - Unzips and decodes an mdzip (mdz) file to an output file. It can also display the block list.
- mdzipnh    - Creates a light weight simplified mdzip (mdsz) binary file with a modular floor and one signature.
- mdunzipnh  - Unzips and decodes a simple mdzip (mdsz) file. It can also display the block list.  
- decoderRandomTestHC2 - Test modulus scan program for the C++ mdzip program.

path: [github.com/singularian/mdencode/code/mdzip_cpp/](https://github.com/singularian/mdencode/tree/master/code/mdzip_cpp)


[MD Encode Main Example Usage](https://github.com/singularian/mdencode/blob/master/docs/Usage.md)

### MDZip C++ Usage Example

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
  -t,--thread,--threads INT:POSITIVE
                              Thread count number
  --fhs INT:POSITIVE:INT in [1 - 34] ...
                              File Hashlist csv string
  --fh INT:POSITIVE:INT in [1 - 34] ...
                              File Hashlist integers list
  -r,--bhs INT:POSITIVE:INT in [1 - 34] ...
                              Block Hashlist csv string
  -s,--bh INT:POSITIVE:INT in [1 - 34] ...
                              Block Hashlist integers list
  -k,--keylist TEXT           Keylist csv string
  --randbh BOOLEAN            Randomize the Block Hash Keylist
  -l,--log BOOLEAN            Run Logging



MDzip Examples:
   mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=true
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=false
   mdzip --file=randfile --block=14 --mod=32 --fh 13     --bh 5        --randbh=true

MDunzip Examples:
   mdunzip --file=filename.mdz --thread=16 
   mdunzip --file=test.mdz --thread=16 
   mdunzip --file=test.mdz --list=true
   mdunzip --file=filename.mdz --list=true --unzip=false   


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

# MDunzip C++ Usage 

```     
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_cpp$ mdunzip
MDEncode MDunzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDunzip filename
  -t,--thread,--threads INT:POSITIVE
                              Thread count number
  -l,--list BOOLEAN           List the mdzip file
  -u,--unzip BOOLEAN          MDunzip a file
  -o,--over BOOLEAN           Overwrite an existing mdunzip output file
  --log BOOLEAN               Run Logging
  --val BOOLEAN               Run the File Signatures on the output file



MDunzip Examples:
   mdunzip --file=filename.mdz --thread=16 
   mdunzip --file=test.mdz --thread=32 
   mdunzip --file=test.mdz --list=true --val=true
   mdunzip --file=filename.mdz --list=true --unzip=false

MDzip Examples:
   mdzip --file=test.txt --block=12 --mod=64 --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 1 2 3  --bh 1 2 3 4 
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=true
   mdzip --file=test.txt --block=12 --mod=64 --fh 11     --bh 1 2 3 4  --randbh=false
   mdzip --file=randfile --block=14 --mod=32 --fh 13     --bh 5        --randbh=true



Hashlist:

ID          Hash Name   Description                   Key         Blocksize   
==============================================================================
1           cit64       Cityhash 64                   true        8           
2           crc32       CRC 32                        true        4           
3           crc64       CRC 64                        true        8           
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

# MDzip C++ No Header Simplified Usage 

This is the usage for a cut down simplified version of MDzip.

Features  

- One 64-bit fasthash 64 signature
- 14 byte signature block
- 32 bit modulus
- 64-bit fasthash 64 randomizable signature key
- A capability to use other signatures with 64 bits

```
MDEncode Minimized MDzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDzip filename
  -k,--key UINT:POSITIVE      Set fasthash key number
  -r,--rand BOOLEAN           Randomize the Key
  -l,--log BOOLEAN            Run Logging


 
Examples:
   mdzipnh --file=test.txt --key=1000 
   mdzipnh --file=test.txt --rand=true
   mdzipnh --file=test.txt  

   mdunzipnh --file=filename.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --list=true
   mdunzipnh --file=filename.mdsz --list=true --unzip=false
```

# MDunzip C++ No Header Simplified Usage 

This is the usage for a cut down simplified version of MDunzip with just One 64-bit fasthash 64 signature and a 14-byte block with a 32-bit modulus.

```
MDEncode Minimized MDunzip C++ Program
Usage: [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT:FILE REQUIRED
                              MDunzip filename
  -t,--thread,--threads INT:POSITIVE
                              Thread count number
  -l,--list BOOLEAN           List the mdzip file
  -u,--unzip BOOLEAN          MDunzip a file
  -o,--over BOOLEAN           Overwrite an existing mdunzip output file
  --log BOOLEAN               Run Logging



Examples:
   mdunzipnh --file=filename.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --thread=16 
   mdunzipnh --file=test.mdsz --list=true
   mdunzipnh --file=filename.mdsz --list=true --unzip=false

   mdzipnh --file=test.txt --key=1000 
   mdzipnh --file=test.txt --rand=true
   mdzipnh --file=test.txt  

```


# MDZip Video Example 

These are examples for the C++ MDzip / MDunzip program  

[MDzip Decompression Example](https://asciinema.org/a/rUzDEPcHuYzD5WnIo5ycTnkDg)  

[MDzip Decompression Example 2](https://asciinema.org/a/Bc3kmRoZMhv7AoArv6jL32SPj)


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

This is a guide for building MDencode MDzip C++ on Linux.

```
1) Install openssl
   Under ubuntu 
   apt-get install -y \
    openssl \
    libssl-dev \
    libgmp3-dev \
2) cd to the fnv directory
   cd ~/mdencode/code/testdecode_cpp/external/fnv
   RUN make install
3) cd to the mdzip_cpp code directory
   cd ~/mdencode/code/mdzip_cpp 
   make all
4) Optional build the test c++ program
   cd ~/mdencode/code/mdzip_cpp
   make test
4) Optional build path
   cd ~/mdencode/build 
   make buildcpp  
```

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
Usage: decoderRandomTestHC2 [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -b,--block UINT:POSITIVE:INT in [1 - 100]
                              Blocksize number
  -m,--mod INT:POSITIVE       Modulus size number
  -t,--thread,--threads INT:POSITIVE
                              Thread count number
  -r,--bh INT:POSITIVE:INT in [1 - 34] ...
                              Block Hashlist csv string
  -s,--hl INT:POSITIVE:INT in [1 - 34] ...
                              Block Hashlist integers list
  --randbh BOOLEAN            Randomize the Block Hash Keylist
  -x,--hex TEXT               Hex Byteblock string
  -l,--log BOOLEAN            Run Logging



Examples:
   decoderRandomTestHC2 -b 12 -m 64 -t 16
   decoderRandomTestHC2 --block=12 --mod=64    --threads=16
   decoderRandomTestHC2 --block=12 --mod=128   --threads=16
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=0011
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011 --log=true --hl 1 2 3 4 5
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011 --log=true --bh 1,5,7
   decoderRandomTestHC2 --mod=64 --threads=32  --hl 1 2 3 4 5 --randbh=true --block=12


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
  
This is an example of the modular floor using the C++ test program. 
It's differentiated from the GO Version by the number. 
The program `decoderRandomTestHC2` is using a random 14 byte array and then running a modulus scan to find the corresponding file block associated with a digital signature.  
It uses a 32 bit modulus and calculates modulus exponent floor or ceiling and then creates a fast64 hash. A modulus scan splits up the parallel search on 32 threads. 
It calculated and found the 14 byte block (13 bytes compressed) in 4 hours on a Ryzen 3950x. It compresses the 14 byte block to 13 bytes and decompresses it to 14 bytes.  

13/14 Modulus Encoding
- 8 bytes for the 64-bit fasthash 64 - (It can use any number of 64 bit hashes)
- 4 bytes for the 32-bit modulus 
- 1 byte for the modulus exponent 

C++ Test program
- ```$GOPATH/github.com/singularian/mdencode/code/testdecode_cpp/decoderRandomTestHC2```

```
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

Second modulus scan decompression example with a larger block running on a ryzen 3950x.

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

Running decode modscan
Found Match

Elapsed Time (s) 43036 = 12 hours
Modulus Scan thread 31 and Random byteblock match
0 0 0 0 22 65 33 97 18 63 24 34 255 252 
0 0 0 0 22 65 33 97 18 63 24 34 255 252 
```

Another modulus scan decompression example with a larger byte block 

```
./decoderRandomTestHC2 --mod=32 --threads=32 --hl 4  --hex=0000002020412161123F1822FFFC --log=true
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

# TODO List

- Add Bigendian / little-endian conversion support. 
- Make the go and C++ programs use the same format
- Add more signatures to the signature list

# Donations

* BTC: `32L3gyJdkiFwgJ72rsSzbCBjTjVFxJLHPm`
* ETH: `0x137eB09cF2e199dA2Aa79607f96719be1491a312`
* LTC: `MJh8XpUQ5wm8x1HCoQ7dewNqhrLfZZc31x`

# Author
MDEncode C++ MDzip and MDunzip was developed by Scott Ross on a Ryzen CPU.

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
