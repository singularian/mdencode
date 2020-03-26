# Modulus Scan

A modulus scan allows for a block signature search with a modular floor

There are two types

- Incrementing Modulus Scans 
- Decrementing Modulus Scans

Currently, mdencode uses an incremental modulus scan.

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
The program `decoderRandom` is creating a random 11 byte array and then running a modulus scan to find the corresponding file block associated with a digital signature.  
It creates a random 11 byte array and a 64 bit modulus and a modulus exponent and then creates a sha1 hash and md5 hash.  
It calculated and found a hashed 11 byte block in 58 ms on a Ryzen 2700x.

`$GOPATH/github.com/singularian/mdencode/code/testdecode/decoderRandom `
```
user@server:~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=11 -mod=64 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  11  byteblock  [24 218 209 240 42 198 53 218 173 140 180]
Found Block  [24 218 209 240 42 198 53 218 173 140 180]
Total time  58.526ms
random bytestring and modulusscan bytestring match  [24 218 209 240 42 198 53 218 173 140 180]   [24 218 209 240 42 198 53 218 173 140 180]
Found block  thread 1 random bytestring and modulusscan bytestring match [24 218 209 240 42 198 53 218 173 140 180] [24 218 209 240 42 198 53 218 173 140 180]
```
This example of decoderRandom uses a 64-bit modulus to calculate a 12 byte block associated with an sha1 and md5 signature.
It uses 16 threads and a parallel modulus scan and was run on a Ryzen 2700x. 
```
user@server:~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=12 -mod=64 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  12  byteblock  [165 55 125 101 98 169 203 81 163 83 179 217]
Found Block  [165 55 125 101 98 169 203 81 163 83 179 217]
Total time  1m14.791082s
random bytestring and modulusscan bytestring match  [165 55 125 101 98 169 203 81 163 83 179 217]   [165 55 125 101 98 169 203 81 163 83 179 217]
Found block  thread 5 random bytestring and modulusscan bytestring match [165 55 125 101 98 169 203 81 163 83 179 217] [165 55 125 101 98 169 203 81 163 83 179 217]
```

[Other Decoder Examples](https://github.com/singularian/mdencode/blob/master/docs/EXAMPLES.md)

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

## Example 1

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

## Example 2

This examples shows a 14 byte block encoded in a 13 byte block. This is an example of modulus scan compression where the mdzip byte block is smaller than the input block.
It uses 2 32-bit signatures CRC-32 and FNV-32 and a 32-bit modulus and 1 byte for the modulus exponent.
```
user@server:~/projects/src/github.com/singularian/mdencode/build$ ./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=16 -hex=0000000012002000000000000000
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
user@instance-2:~/mdencode/mdencode-1.0.5/build$ ./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=120 -hex=000000001200200000000000FFFC
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
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=0 -end=32  -hex=0000000012002000000011DDFFFC 
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=32 -end=64  -hex=0000000012002000000011DDFFFC 
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=64 -end=96  -hex=0000000012002000000011DDFFFC 
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=96 -end=128  -hex=0000000012002000000011DDFFFC 
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=128 -end=160 -hex=0000000012002000000011DDFFFC 
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=160 -end=192 -hex=0000000012002000000011DDFFFC
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=192 -end=224 -hex=0000000012002000000011DDFFFC 
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=224 -end=256 -hex=0000000012002000000011DDFFFC 
./decoderRandomTestHC -block=14 -mod=32 -bh=0000000010000001 -thread=288 -start=256 -end=288 -hex=0000000012002000000011DDFFFC 

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

This is an example of a parallel modulus scan with 16 threads.
[Decoder Parallel Examples](https://github.com/singularian/mdencode/blob/master/examples/mdencodeParallelmodscan2.txt)
