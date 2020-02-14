# Signature list

This is the current numerical signature hash list.  
It corresponds to the binary or quaterniary argument position.  
These should be in sorted order for the context hashlist.  

# Adding New Signatures

This is the step to add a new signature. It should be in sorted order  

```
1: Add the signature name ID and description and block size to $GOPATH/$PROJECT/code/decode/mdBlockSize/mdBinaryList.go  
2: Add the hash import to the hash context list to $GOPATH/$PROJECT/code/decode/mdHashContextList/mdHashContextList.go  
3: Add the hash context new initialize function to CreateHashListMap()  
   in decode/mdHashContextList/mdHashContextList.go  
```

### Current mdencode signatures  

These are the current mdencode signatures

```
1:  aes8        - AES Hash 8           
2:  ax          - XXHash 128           
3:  blake2      - Blake2               
4:  blake2b     - Blake2b              
5:  blake2s_128 - Blake2s 128          
6:  blake2s_256 - Blake2s 256          
7:  bmw         - BMW                  
8:  crc64ecma   - CRC64 ECMA           
9:  crc64iso    - CRC64 ISO            
10: cube        - Cubehash             
11: echo        - Echo                 
12: fnv         - FNV 64               
13: fnva        - FNV 64a              
14: fnv128      - FNV 128              
15: fnv128a     - FNV 128a             
16: gost256     - Gost 256             
17: gost512     - Gost 512             
18: groest      - Groest               
19: hmac256     - HMAC 256             
20: hmac512     - HMAC 512             
21: hw64        - Highway Hash 64      
22: hw128       - Highway Hash 128     
23: hw256       - Highway Hash 256     
24: kekkak      - Kekkak               
25: luffa       - Luffa                
26: murmur3     - Murmur3              
27: md2         - MD2                  
28: md4         - MD4                  
29: md5         - MD5                  
30: poly1305    - Poly1305             
31: ripe128     - Ripe 128             
32: ripe160     - Ripe 160             
33: ripe256     - Ripe 256             
34: ripe320     - Ripe 320             
35: sea         - Seahash              
36: sha1_128    - SHA1 128             
37: sha1_1284   - SHA1 1284            
38: sha1        - SHA1                 
39: sha224      - SHA224               
40: sha256      - SHA256               
41: sha512_224  - SHA512 224           
42: sha512_256  - SHA512 256           
43: sha512_384  - SHA512 384           
44: sha512      - SHA512               
45: sha3_224    - SHA3 224             
46: sha3_256    - SHA3 256             
47: sha3_384    - SHA3 384             
48: sha3_512    - SHA3 512             
49: shavite     - X11 SHAvite          
50: sip64       - Siphash 64           
51: sip128      - Siphash 128          
52: skein_160   - Skein 160            
53: skein_256   - Skein 256            
54: skein_384   - Skein 384            
55: skein_512   - Skein 512            
56: skein_1024  - Skein 1024           
57: tiger       - Tiger                
58: whirlpool   - Whirlpool            
59: xxhash      - XXHash               
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



