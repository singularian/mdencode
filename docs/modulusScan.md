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
