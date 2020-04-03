# Signature list

This is the current numerical signature hash list.  
It corresponds to the binary or quaterniary argument position.  
These should be in sorted order for the context hashlist.  

```
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
37:    me32          Meow 32                                  4                    true       
38:    me64          Meow 64                                  8                    true       
39:    me128         Meow 128                                 16                   true       
40:    mm32          Murmur3 32                               4                    true       
41:    murmur3       Murmur3                                  16                   true       
42:    md2           MD2                                      16                   false      
43:    md4           MD4                                      16                   false      
44:    md5           MD5                                      16                   false      
45:    poly1305      Poly1305                                 16                   true       
46:    ripe128       Ripe 128                                 16                   false      
47:    ripe160       Ripe 160                                 20                   false      
48:    ripe256       Ripe 256                                 32                   false      
49:    ripe320       Ripe 320                                 40                   false      
50:    sea           Seahash                                  8                    false      
51:    sha1_64       SHA1 64                                  8                    false      
52:    sha1_128      SHA1 128                                 16                   false      
53:    sha1_1284     SHA1 1284                                16                   false      
54:    sha1          SHA1                                     20                   false      
55:    sha224        SHA224                                   28                   false      
56:    sha256        SHA256                                   32                   false      
57:    sha512_224    SHA512 224                               28                   false      
58:    sha512_256    SHA512 256                               32                   false      
59:    sha512_384    SHA512 384                               48                   false      
60:    sha512        SHA512                                   64                   false      
61:    sha3_224      SHA3 224                                 28                   false      
62:    sha3_256      SHA3 256                                 32                   false      
63:    sha3_384      SHA3 384                                 48                   false      
64:    sha3_512      SHA3 512                                 64                   false      
65:    shavite       X11 SHAvite                              64                   false      
66:    sip64         Siphash 64                               8                    true       
67:    sip128        Siphash 128                              16                   true       
68:    skein_160     Skein 160                                20                   false      
69:    skein_256     Skein 256                                32                   false      
70:    skein_384     Skein 384                                48                   false      
71:    skein_512     Skein 512                                64                   false      
72:    skein_1024    Skein 1024                               128                  false      
73:    spk32         Spooky 32                                4                    true       
74:    spk64         Spooky 64                                8                    true       
75:    tiger         Tiger                                    24                   false      
76:    whirlpool     Whirlpool                                64                   false      
77:    xxh32         XXHash 32                                4                    true       
78:    xxhash        XXHash 64                                8                    true       
79:    wy            WY Hash 64                               8                    true       
``` 

# Adding New Signatures

This is the step to add a new signature. It should be in sorted order

```
1: Add the signature name ID and description and block size to $GOPATH/$PROJECT/code/decode/mdSignatureList/mdSignatureList.go
2: Add the hash import to the hash context list to $GOPATH/$PROJECT/code/decode/mdHashContextList/mdHashContextList.go
3: Add the hash context new initialize function to CreateHashListMap()
   in decode/mdHashContextList/mdHashContextList.go
4: If the signature contains a key add a default key to keylist hash map and a initializer
5: Add the new signaturelist to docs/hashList.md and docs/usage.md
```

# Binary Signature Arguments

Binary signature parameters for mdencode include the following.  
Two lists are created for file hash lists and block hash lists.  

0 - Hash is not used  
1 - Hash is used  



# Quaterniary Mdencode Arguments

Quaterniary signature parameters for mdencode include 4 states.
One list is used for the file and block hash lists and possibly block group hash lists.

0 - No Hash  
1 - File Hash used  
2 - Block Hash used  
3 - File and Block Hash used  


# Quaterniary Mdencode Examples

```sh
user@server:~/projects/src/github.com/singularian/mdencode/code/mdencode$ md -file=Makefile -block=300 -mod=32 -uh=11 -format=4300
# Filename
Makefile

# Filepath
/home/george/projects/src/github.com/singularian/mdencode/code/mdencode/

# File Header
1792:4300:300:32:md4:md5-empty:1.0:2019-10-27 18:49:46.2424555 -0400 EDT m=+0.000958801

- File Size 1792 bytes
- Ecoding Format 4300
- Block Size 300 bytes
- Modulus Size 32 bits
- File Hash List md4:md5
- Block Hash List empty
- Version 1.0
- Time 2019-10-27 18:49:46.2428447 -0400 EDT m=+0.001347801

# Filehash
- md4 6444ab32cf86c8f252e72905ce52c14d
- md5 d41d8cd98f00b204e9800998ecf8427e

```

```sh
user@server:~/projects/src/github.com/singularian/mdencode/code/mdencode$ md -file=Makefile -block=300 -mod=32 -uh=110023 -format=4300
# Filename
Makefile

# Filepath
/home/george/projects/src/github.com/singularian/mdencode/code/mdencode/

# File Header
1792:4300:300:32:md4:md5:sha512_224-sha256:sha512_224:1.0:2019-10-27 18:51:43.8975134 -0400 EDT m=+0.001214601

- File Size 1792 bytes
- Ecoding Format 4300
- Block Size 300 bytes
- Modulus Size 32 bits
- File Hash List md4:md5:sha512_224
- Block Hash List sha256:sha512_224
- Version 1.0
- Time 2019-10-27 18:51:43.8980733 -0400 EDT m=+0.001774201

# Filehash
- md4 6444ab32cf86c8f252e72905ce52c14d
- md5 d41d8cd98f00b204e9800998ecf8427e
- sha512_224 6ed0dd02806fa89e25de060c19d3ac86cabb87d6a0ddd05c333b84f4

# blockhash 0
- 300 bytes
- Hash List 2f77560166dbaac4f146cfaa7b5ef49a2e37f0b5363310d8932089dcb12e85ba:8fcf7a40e4e58f52eebb25e965e052d718a6caf52b0a64e2199647f4
- Modulus Exponent 2397
- Modulus 1025801031

# blockhash 1
- 300 bytes
- Hash List a6b8412a9c8d1284297025ca6de92c48042a5536e61167a6f543ebabf1da875f:15ea37ffd34625eb3a3e01780e2e584a365d1e379c1de85579ff59ed
- Modulus Exponent 2398
- Modulus 1430867012

# blockhash 2
- 300 bytes
- Hash List d34c0eeb5ed4ba82ef30a8c853655802743998c6a28e703fa9710d542f0c4d0d:13f273aef5712b473e4e9cb1536f94e994441549e6414af7aad8fb75
- Modulus Exponent 2398
- Modulus 1950509422

# blockhash 3
- 300 bytes
- Hash List 53c4abac3f3a116407343a08d2c9f1451fb9b7be4152bd046625ba1454834484:f73ac621c2589dcc03b94907b2afd31553b6628366a967d2a5cb4b35
- Modulus Exponent 2398
- Modulus 1936990324

# blockhash 4
- 300 bytes
- Hash List 5f8c0a400c9e53a49085504d237ea1268006bd8715926689ed4ab2d86ca02163:6f910b12af644b6b12869467ba30a6ed5ea7e04d6c9e6a9bad856f18
- Modulus Exponent 2398
- Modulus 543648116

# blockhash 5
- 292 bytes
- Hash List b495d001c3711c7a39cd2d800a54082359987817c551838272bde05f173d42d5:8e0650ecd16bf5a3b1ac512fc7c7eca5d9f3b2d7ff3bf6538951a7e6
- Modulus Exponent 2334
- Modulus 1819025930
```

# Octal Signature Arguments

Octal signature parameters for mdencode include 8 states. This is a future enhancement.   
One list is used for the file and block hash lists and possibly block group hash lists.  

## States 
0 - No Hash  
1 - File Hash used  
2 - Block Group used  
3 - Block Hash used  
4 - File and Block Hash used  
5 - Block Group and Block Hash used  
6 - File and Block Group Used  
7 - File and Block Group and Block Hash used  

# HEX Encode Signature Arguments

Each parameter has a hex code. The first Hex Byte Nibble Half-Byte [0-F] delineates the hash type and the 
second Nibble Half-Byte [0-F] delineates the signature number 0 to 15 for the first byte.  
Each successive HEX byte increments the signature count by multiplying the 15 times the byte position.


Each byte increments the signature number in the second nibble.  
In this way 20 characters can encode (10 * 15) 150 signatures.  
A 200 character HEX Signature could encode 1500 signatures.  


## States
0 - No Hash  
1 - File Hash used  
2 - Block Group used  
3 - Block Hash used  
4 - File and Block Hash used  
5 - Block Group and Block Hash used  
6 - File and Block Group Used  
7 - File and Block Group and Block Hash used  

## Hex Encoding

The first Half-Byte is the Type [0-F]  
The second Half-Byte is the Number [0-F] or [0-Z] if there was a need for more signatures. 

[TYPE][NUMBER]  
[0-F Type][0-F Number]  

Alternatively, it can be 0-Z or 35 signatures per byte for the number.  

## Econding Example
1F227C42  

1F = File Hash Signature 15  
22 = Block Group Hash Signature 18  
7C = File and Block Group and Block Hash Signature 30  
42 = File and Block Hash Signature 33


# Hex Encoding 2

This format uses 3 to 5 bytes. One byte is the Signature type and the 2 to 4 following bytes are the signature number. Either 65k or 4 Billion in a 32 bit number.  

[TYPE][NUMBER][TYPE][NUMBER][TYPE][NUMBER]  

[0-255] - Signature Type Byte. There can be 255 Signature types in this example although it could use the 8 state model.
[0-255][0-255] - A 16 bit double byte that represents the signature number. This means you can reference 65+ k signatures in each group  
[0-255][0-255][0-255][0-255] - A alternative 32-bit integer that represents the signature number. This means you can reference 4 billion signatures

## States
0 - No Hash  
1 - File Hash used  
2 - Block Group used  
3 - Block Hash used  
4 - File and Block Hash used  
5 - Block Group and Block Hash used  
6 - File and Block Group Used  
7 - File and Block Group and Block Hash used  


# 16-bit Encoding Example

070E11020203031111

## Group 1

- 070E11 -  File and Block Group and Block Hash used - 0E11 - The 3601 Number  
- 020203 -  Block Group used - 0203 - The 515 Signature Number  
- 031111 -  Block Hash Used - 1111 - The 4369 Signature Number  



