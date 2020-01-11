# Command Line Usage of mdencode


[MD Encode Ascii Art Example]((https://asciinema.org/a/293097)

``` 
user@server:~/projects/src/github.com/singularian/mdencode/code/mdencode
Usage of md:                                                                                                                                     
                                                                                                                                                 
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
4300    - Mark Down
4500    - HTML
5000    - XML GO
5001    - XML
8000    - PNG Image Signature
8001    - PNG Image Signature
                                                                                                                                                 
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
md -uh=1122 -block=100 -mod=64 -file=Makefile
```


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

# Example decoderRandom  Usage

```
user@server:~/projects/src/github.com/singularian/mdencode/code/testdecode
$ ./decoderRandom
Usage  ./decoderRandom  -block=[BLOCKSIZE BYTES] -mod=[MODSIZE BITS] -thread=[THREADSIZE GOROUTINES] -start=[THREAD START] -end=[THREAD END] -bytes=[OPTIONAL JSON BYTESTRING] -hex=[OPTIONAL HEX BYTESTRING]

Usage  ./decoderRandom  -block=12 -mod=64 -thread=16
Usage  ./decoderRandom  -block=9 -mod=64 -thread=10 -bytes=[1,2,3,4,5]
Usage  ./decoderRandom  -block=8 -mod=64 -thread=10 -hex=FF0C3FDDAF
Usage  ./decoderRandom  -block=8 -mod=64 -thread=10 -bytes=[100,222,30,55,100]
Usage  ./decoderRandom  -mod=64 -thread=16 -start=2 -end=5 -bytes=[100,222,30,55,100,11,123]
Usage  ./decoderRandom  -mod=64 -thread=16 -start=2 -end=5 -hex=0F0F0F22CDFF
```

# Example Usage of mdbinlist

This is a test binary signature exporter. It will ultimately be used as a mdencode compression file.

```
$ mdbinlist

USAGE of mdbinlist:
Examples:
mdbinlist  [FORMAT]
mdbinlist  [FORMAT] [FILENAME]
mdbinlist  [FORMAT] [FILENAME] [OUTPUTFILE]
mdbinlist  4000 (opens default.mdbin)
mdbinlist  4000
mdbinlist  4000 mdbintest.md
```

# Example Usage of mdzip and mdunzip

                                                                                                    
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
  -log string                                                                                       
        Log Filename                                                                                
                                                                                                    
                                                                                                    
Build Time: 2018-06-16-0431 UTC                                                                     
Version:    1.0.0 復甦 復活                                                                             
```                                                                                                 
                                                                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./mdunzip                   
                                                                                                    
  -file string                                                                                      
        Input Filename                                                                              
  -out string                                                                                       
        Output Filename                                                                             
  -thread string                                                                                    
        Go Routine Threadsize   
  -val bool
        Run the File Hash List Post Validation
```                                                                                                 
                                                                                                    
mdlist can display the contents of an mdzip file                                                    
```                                                                                                 
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./mdlist
USAGE of ./mdlist:  
Examples:  
./mdlist  [FILENAME]   
```

# Example Usage of mdsig

```
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./mdsig

Current mdencode signatures                
                                           
1:  aes8        - AES Hash 8               
2:  ax          - XXHash 128               
3:  blake2      - Blake2                   
4:  blake2b     - Blake2b                  
5:  blake2s_128 - Blake2s 128              
6:  blake2s_256 - Blake2s 256              
7:  bmw         - BMW                      
8:  cube        - Cubehash                 
9:  echo        - Echo                     
10: fnv         - FNV                      
11: fnv128      - FNV 128                  
12: fnv128a     - FNV 128a                 
13: gost256     - Gost 256                 
14: gost512     - Gost 512                 
15: groest      - Groest                   
16: hmac256     - HMAC 256                 
17: hmac512     - HMAC 512                 
18: hw64        - Highway Hash 64          
19: hw128       - Highway Hash 128         
20: hw256       - Highway Hash 256         
21: kekkak      - Kekkak                   
22: luffa       - Luffa                    
23: murmur3     - Murmur3                  
24: md4         - MD4                      
25: md5         - MD5                      
26: ripe128     - Ripe 128                 
27: ripe160     - Ripe 160                 
28: ripe256     - Ripe 256                 
29: sea         - Seahash                  
30: sha1_128    - SHA1 128                 
31: sha1_1284   - SHA1 1284                
32: sha1        - SHA1                     
33: sha224      - SHA224                   
34: sha256      - SHA256                   
35: sha512      - SHA512                   
36: sha512_224  - SHA512 224               
37: sha512_256  - SHA512 256               
38: sha3_224    - SHA3 224                 
39: sha3_256    - SHA3 256                 
40: sha3_384    - SHA3 384                 
41: sha3_512    - SHA3 512                 
42: siphash     - Siphash                  
43: skein_160   - Skein 160                
44: skein_256   - Skein 256                
45: skein_384   - Skein 384                
46: skein_512   - Skein 512                
47: skein_1024  - Skein 1024               
48: tiger       - Tiger                    
49: whirlpool   - Whirlpool                
50: xxhash      - XXHash                   
```
