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
        File Hash List (default "01001") CSV or Binary                                                                                               
  -bh string                                                                                                                                     
        Block Hash List (default "01001") CSV or Binary                       
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

Build Time: 2020-04-09T00:05:17-0400
Version: 1.0.8 復甦 復活
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

Build Time: 2020-04-09T00:17:11-0400
Version: 1.0.8 復甦 復活

Examples:
mddbreport -file=md.db -fileid=1 -format=10
mddbreport -file=md.db -fileid=1 -format=10 -out=outputfile.txt -append=true
mddbreport -file=md.db -fileid=1 -format=5000 -out=outputfile.xml -append=false

```

# Example mdtest Usage

```
user@server:~/projects/src/github.com/singularian/mdencode/code/testdecode
$ ./mdtest
USAGE of mdtest:

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
    
Usage  mdtest  -block=12 -mod=64 -thread=16 -bh=1010101
Usage  mdtest  -block=9 -mod=64 -thread=10 -bh=11111 -bytes=[1,2,3,4,5]
Usage  mdtest  -block=8 -mod=64 -thread=10 -bh=1 -bytes=[100,222,30,55,100]
Usage  mdtest  -block=8 -mod=64 -thread=10 -bh=101 -hex=FF0C3FDDAF
Usage  mdtest  -block=20 -mod=128 -thread=16 -bh=0000000000000000001 -keylist=aes8:F01100119900112FF11
Usage  mdtest  -mod=64 -thread=16 -start=2 -end=5 -bytes=[100,222,30,55,100,11,123]
Usage  mdtest  -mod=64 -thread=16 -start=2 -end=5 -hex=0F0F0F22CDFF
Usage  mdtest  -block=11 -mod=64 -bh=1,5,15,16 -thread=16
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
user@server:~/projects/src/github.com/singularian/mdencode/code/mdzip_go$ ./mdzip                     
USAGE of ./mdzip:                                                                                   
                                                                                                    
  -file string                                                                                      
        Input Filename                                                                              
  -block string                                                                                     
        File Block Size Bytes (default "40")                                                        
  -fh string                                                                                        
        File Hash List (default "01001") CSV or Binary 
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

Build Time: 2020-03-10T13:42:02-0500    
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

mdcmd is a command line version of mdunzip. It is used as a potential distributed worker node for mdunzip.
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
  -thread string
        Go Routine Threadsize


Examples:
$ mdCmd -block=10 -mod=64 -h=sha1_64 -hex=17af4b9d3c68b6bb -exp=79 -rem=5324266269509948425 -thread=16

Starting mddecode
modsize           64
hashliststring    sha1_64
modexponent       79
modulus remainder 5324266269509948425

Found Block [sha1_64 17af4b9d3c68b6bb] result = 17af4b9d3c68b6bb Found Block buffer  [213 13 73 227 151 246 131 188 20 9]  thread  13
Total time  2.6133ms
buffer  [213 13 73 227 151 246 131 188 20 9]
Found Thread 13 Block d5 0d 49 e3 97 f6 83 bc 14 09
Found Match D50D49E397F683BC1409

$ mdcmd -block=11 -mod=64 -h=aes8:ax -hex=d72f0e3e362e899364aa114c88fa69d5bdd40ff776883374 -exp=87 -rem=8990898166113033087 -thread=16
Starting mddecode
modsize           64
hashliststring    aes8:ax
modexponent       87
modulus remainder 8990898166113033087

Found Block [aes8 d72f0e3e362e8993 ax 64aa114c88fa69d5bdd40ff776883374] result = d72f0e3e362e899364aa114c88fa69d5bdd40ff776883374 Found Block buffer  [154 141 237 124 198 22 63 10 224 219 127]  thread  13
Total time  64.9105ms
buffer  [154 141 237 124 198 22 63 10 224 219 127]
Found Thread 13 Block 9a 8d ed 7c c6 16 3f 0a e0 db 7f
Found Match 9A8DED7CC6163F0AE0DB7F
```

# Example Usage of mdsig

```
user@server:~/projects/src/github.com/singularian/mdencode/code/decode$ ./mdsig

Current mdencode signatures                

ID:    HashName      Description                              Block Size           Hash Key  
============================================================================================ 
1:     add32         Adler 32                                 4                    false     
2:     aes8          AES Hash 8                               8                    true      
3:     ax            XXHash 128                               16                   true      
4:     blake2        Blake2                                   64                   false     
5:     blake2b       Blake2b                                  32                   false     
6:     blake2s_128   Blake2s 128                              16                   true      
7:     blake2s_256   Blake2s 256                              32                   true      
8:     bmw           BMW                                      64                   false     
9:     crc32         CRC-32 IEEE                              4                    false     
10:    crc32c        CRC-32 Castagnoli                        4                    false     
11:    crc32k        CRC-32 Koopman                           4                    false     
12:    crc64ecma     CRC-64 ECMA                              8                    false     
13:    crc64iso      CRC-64 ISO                               8                    false     
14:    ct64          Cityhash 64                              8                    true      
15:    cube          Cubehash                                 64                   false     
16:    echo          Echo                                     64                   false     
17:    fh32          Farm Hash 32                             4                    true      
18:    fh64          Farm Hash 64                             8                    true      
19:    fnv32         FNV 32                                   4                    false     
20:    fnv32a        FNV 32a                                  4                    false     
21:    fnv64         FNV 64                                   8                    false     
22:    fnv64a        FNV 64a                                  8                    false     
23:    fnv128        FNV 128                                  16                   false     
24:    fnv128a       FNV 128a                                 16                   false     
25:    gost256       Gost 256                                 32                   false     
26:    gost512       Gost 512                                 64                   false     
27:    groest        Groest                                   64                   false     
28:    hmac256       HMAC 256                                 32                   true      
29:    hmac512       HMAC 512                                 64                   true      
30:    hw32          Highway Hash 32                          4                    true      
31:    hw64          Highway Hash 64                          8                    true      
32:    hw128         Highway Hash 128                         16                   true      
33:    hw256         Highway Hash 256                         32                   true      
34:    kekkak        Kekkak                                   32                   false     
35:    luffa         Luffa                                    64                   false     
36:    jn64          Jenkins 64                               8                    true      
37:    mar32         Marvin 32                                4                    true      
38:    me32          Meow 32                                  4                    true      
39:    me64          Meow 64                                  8                    true      
40:    me128         Meow 128                                 16                   true      
41:    met64         Metro Hash 64                            8                    true      
42:    mm32          Murmur3 32                               4                    true      
43:    murmur3       Murmur3                                  16                   true      
44:    md2           MD2                                      16                   false     
45:    md4           MD4                                      16                   false     
46:    md5           MD5                                      16                   false     
47:    poly1305      Poly1305                                 16                   true      
48:    ripe128       Ripe 128                                 16                   false     
49:    ripe160       Ripe 160                                 20                   false     
50:    ripe256       Ripe 256                                 32                   false     
51:    ripe320       Ripe 320                                 40                   false     
52:    sea           Seahash                                  8                    false     
53:    sha1_64       SHA1 64                                  8                    false     
54:    sha1_128      SHA1 128                                 16                   false     
55:    sha1_1284     SHA1 1284                                16                   false     
56:    sha1          SHA1                                     20                   false     
57:    sha224        SHA224                                   28                   false     
58:    sha256        SHA256                                   32                   false     
59:    sha512_224    SHA512 224                               28                   false     
60:    sha512_256    SHA512 256                               32                   false     
61:    sha512_384    SHA512 384                               48                   false     
62:    sha512        SHA512                                   64                   false     
63:    sha3_224      SHA3 224                                 28                   false     
64:    sha3_256      SHA3 256                                 32                   false     
65:    sha3_384      SHA3 384                                 48                   false     
66:    sha3_512      SHA3 512                                 64                   false     
67:    shavite       X11 SHAvite                              64                   false     
68:    sip64         Siphash 64                               8                    true      
69:    sip128        Siphash 128                              16                   true      
70:    skein_160     Skein 160                                20                   false     
71:    skein_256     Skein 256                                32                   false     
72:    skein_384     Skein 384                                48                   false     
73:    skein_512     Skein 512                                64                   false     
74:    skein_1024    Skein 1024                               128                  false     
75:    spk32         Spooky 32                                4                    true      
76:    spk64         Spooky 64                                8                    true      
77:    tiger         Tiger                                    24                   false     
78:    t1ha          T1ha                                     8                    true      
79:    whirlpool     Whirlpool                                64                   false     
80:    xxh32         XXHash 32                                4                    true      
81:    xxhash        XXHash 64                                8                    true      
82:    wy            WY Hash 64                               8                    true      
```
