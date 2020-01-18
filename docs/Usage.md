# Command Line Usage of mdencode


[MD Encode Ascii Art Example](https://asciinema.org/a/293097)

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
        Signature Hash Keylist
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
md -file=Makefile -block=400 -mod=33 -fh=111 -bh=0000000000000000001 -format=4300 -keylist=hw64:FFFFFCC
md -file=randomfile.mdz -block=100 -mod=64 -bh=100000000000000000111 -keylist=aes8:95511,hw64:FF0CC001,hw128:FFCCc00c,hw256:ffcc000 -format=4300
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
  -keylist string
        Signature Hash Keylist
  -log string                                                                                       
        Log Filename  


Examples:  
mdzip -mod=64 -block=12 -file=randomfile -out=randomfile.mdz -bh=000000000001001 -fh=111  
mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000001 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E  

Signature Keys Format:
A signature Key parameter "-keylist" is delineatead with a keyname colon hexvalue comma

  -keylist=keyname:hexvalue  
  -keylist=keyname:hexvalue,key2:hexvalue  
  -keylist=keyname:hexvalue,key2:hexvalue  
  -keylist=keyname:hexvalue,key2:hexvalue,key3:keyvalue,...  

Keylist:
aes8        - uint64 integer 1 to 18 numbers
blake2s_128 - 16+ hex characters
blake2s_256 - 16+ hex characters
hw64        - 1 to 64 hex characters
hw128       - 1 to 64 hex characters
hw256       - 1 to 64 hex characters
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

Build Time: 2020-01-18T13:42:02-0500    
Version:    1.0.0 復甦 復活
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
10: fnv         - FNV 64          
11: fnva        - FNV 64a         
12: fnv128      - FNV 128         
13: fnv128a     - FNV 128a        
14: gost256     - Gost 256        
15: gost512     - Gost 512        
16: groest      - Groest          
17: hmac256     - HMAC 256        
18: hmac512     - HMAC 512        
19: hw64        - Highway Hash 64 
20: hw128       - Highway Hash 128
21: hw256       - Highway Hash 256
22: kekkak      - Kekkak          
23: luffa       - Luffa           
24: murmur3     - Murmur3         
25: md2         - MD2             
26: md4         - MD4             
27: md5         - MD5             
28: poly1305    - Poly1305        
29: ripe128     - Ripe 128        
30: ripe160     - Ripe 160        
31: ripe256     - Ripe 256        
32: ripe320     - Ripe 320        
33: sea         - Seahash         
34: sha1_128    - SHA1 128        
35: sha1_1284   - SHA1 1284       
36: sha1        - SHA1            
37: sha224      - SHA224          
38: sha256      - SHA256          
39: sha512      - SHA512          
40: sha512_224  - SHA512 224      
41: sha512_256  - SHA512 256      
42: sha3_224    - SHA3 224        
43: sha3_256    - SHA3 256        
44: sha3_384    - SHA3 384        
45: sha3_512    - SHA3 512        
46: shavite     - X11 SHAvite     
47: sip64       - Siphash 64      
48: sip128      - Siphash 128     
49: skein_160   - Skein 160       
50: skein_256   - Skein 256       
51: skein_384   - Skein 384       
52: skein_512   - Skein 512       
53: skein_1024  - Skein 1024      
54: tiger       - Tiger           
55: whirlpool   - Whirlpool       
56: xxhash      - XXHash                                                     
```
