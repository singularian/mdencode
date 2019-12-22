# Signature list

This is the current numerical signature hash list.  
It corresponds to the binary or quaterniary argument position.  
These should be in sorted order for the context hashlist.  

Current mdencode signatures  

  1: blake2                
  2: blake2b               
  3: blake2s_128           
  4: blake2s_256           
  5: cube                  
  6: fnv                   
  7: fnv128                
  8: fnv128a               
  9: gost256               
 10: gost512               
 11: hmac256               
 12: hmac512               
 13: hw128                 
 14: hw256                 
 15: kekkak                
 16: murmur3               
 17: md4                   
 18: md5                   
 19: ripe128               
 20: ripe160               
 21: ripe256               
 22: sha1_128              
 23: sha1_1284             
 24: sha1                  
 25: sha224                
 26: sha256                
 27: sha512                
 28: sha512_224            
 29: sha512_256            
 30: sha3_224              
 31: sha3_256              
 32: sha3_384              
 33: sha3_512              
 34: siphash               
 35: skein_160             
 36: skein_256             
 37: skein_384             
 38: skein_512             
 39: skein_1024            
 40: tiger                 
 41: whirlpool 
 42: xxhash            

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



