# Siphash 47 encoding

This is an example of the siphash47 decompression.  
It currently uses a 7 bit bitstream to encode the modulus exponent.  
This means that an 80 bit and 10 byte file block can be encoded as 79 bits.  

10 byte file block encoding
- 5 bytes siphash40
- 4 bytes 32 bit modulus
- 7 bits for the modulus exponent (this could be 2 to 7 bits and could use RLE)

```
Starting the mdzip  C++ sip47 modulus bitstream
Creating randomfile

rm -f randomfile

# randfile2 -blocknum=10 -bsize=10 -zsize=2 -first=14 -out=randomfile
# this test uses a random block in this format 
# it could also use a full non padded block 
# 0   1   127 231 57  115 27  53  37  74
# 00  14  7F  E7  39  73  1B  35  25  4A
randfile2 -blocknum=10 -bsize=10 -zsize=2 -first=1 -out=randomfile
Generating block  1
Generating block  2
Generating block  3
Generating block  4
Generating block  5
Generating block  6
Generating block  7
Generating block  8
Generating block  9
Generating block  10
Randomfile  randomfile Created

Zipping the file randomfile

mdzipsip47 --file=randomfile --randbh --fh=11

Filename Details:   randomfile
Version:            1.01
Filesize:           100
Blocksize:          10
Blockcount:         10
Blockremainder:     10
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Filehashlist size:  6
Blockhashlist size: 5
Blockhash key list: sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
File Hash Bytes:    20
Block Hash Bytes:   5
Platform:           Little Endian

File hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           11           has160      20          0          
                         Total       20          0

File block hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           30           sip40       5           16         
                         Total       5           16

File Signatures 
has160 A202B65206937E7F572C8011C608274C220B4AB3 

Zipping Block 1 Bytes Size 10/10
Block Bytes         00 01 F8 CB BC B5 82 41 28 1A 
Signatures          sip40 BF68C674C2 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   2064442577

Zipping Block 2 Bytes Size 10/10
Block Bytes         00 01 87 C0 4A CD CD 5E 9D F0 
Signatures          sip40 7253338C5C 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1428089023

Zipping Block 3 Bytes Size 10/10
Block Bytes         00 01 CA B2 ED FE 01 80 21 CC 
Signatures          sip40 87DA8B9654 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   3425898443

Zipping Block 4 Bytes Size 10/10
Block Bytes         00 01 9E 8B 2D EB 13 AE 88 F1 
Signatures          sip40 94A7B253BC 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   2990126813

Zipping Block 5 Bytes Size 10/10
Block Bytes         00 01 90 86 29 D4 46 CF 32 C1 
Signatures          sip40 70E340400B 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   3612695702

Zipping Block 6 Bytes Size 10/10
Block Bytes         00 01 BD 44 9C E2 9A 5F 34 6C 
Signatures          sip40 BA2E3AE2A0 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1470353744

Zipping Block 7 Bytes Size 10/10
Block Bytes         00 01 06 A9 53 00 FB 64 BC 44 
Signatures          sip40 6DF6C6C370 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   34475846

Zipping Block 8 Bytes Size 10/10
Block Bytes         00 01 F9 12 CE 39 74 22 86 95 
Signatures          sip40 641FFE613F 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1832211664

Zipping Block 9 Bytes Size 10/10
Block Bytes         00 01 0F 5A 0C 56 05 6C 7E 48 
Signatures          sip40 EA5D623CD6 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   348555935

Zipping Block 10 Bytes Size 10/10
Block Bytes         00 01 C5 C1 DA 5A 5A 1E 62 7F 
Signatures          sip40 BDB08537F0 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   534789339

Validating the randomfile.mdz

mdunzipsip47 --file=randomfile.mdz --valmdzip

MDzip File randomfile.mdz validates 
MDzip File 186 = 186

Running mdlist randomfile.mdz

mdunzipsip47 --file=randomfile.mdz --list 

Zip Filename:       randomfile.mdz
Unzip Filename:     randomfile.mdz.out
Version:            1.01
Filesize:           100
Blocksize:          10
Blockcount:         10
Blockremainder:     10
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Blockkeylist:       sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
File Hash Bytes:    20
Block Hash Bytes:   5
Platform:           Little Endian

File hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           11           has160      20          0          
                         Total       20          0

File block hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           30           sip40       5           16         
                         Total       5           16

File Signatures 
has160 A202B65206937E7F572C8011C608274C220B4AB3 

Displaying Block 1 Bytes Size 10/10
Signatures          sip40 BF68C674C2 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   2064442577

Displaying Block 2 Bytes Size 10/10
Signatures          sip40 7253338C5C 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1428089023

Displaying Block 3 Bytes Size 10/10
Signatures          sip40 87DA8B9654 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   3425898443

Displaying Block 4 Bytes Size 10/10
Signatures          sip40 94A7B253BC 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   2990126813

Displaying Block 5 Bytes Size 10/10
Signatures          sip40 70E340400B 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   3612695702

Displaying Block 6 Bytes Size 10/10
Signatures          sip40 BA2E3AE2A0 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1470353744

Displaying Block 7 Bytes Size 10/10
Signatures          sip40 6DF6C6C370 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   34475846

Displaying Block 8 Bytes Size 10/10
Signatures          sip40 641FFE613F 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1832211664

Displaying Block 9 Bytes Size 10/10
Signatures          sip40 EA5D623CD6 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   348555935

Displaying Block 10 Bytes Size 10/10
Signatures          sip40 BDB08537F0 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   534789339


Unzipping randomfile

mdunzipsip47 --file=randomfile.mdz --over --val=true

Zip Filename:       randomfile.mdz
Unzip Filename:     randomfile.mdz.out
Version:            1.01
Filesize:           100
Blocksize:          10
Blockcount:         10
Blockremainder:     10
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Blockkeylist:       sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
File Hash Bytes:    20
Block Hash Bytes:   5
Threadcount:        32
Platform:           Little Endian

File hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           11           has160      20          0          
                         Total       20          0

File block hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           30           sip40       5           16         
                         Total       5           16

File Signatures 
has160 A202B65206937E7F572C8011C608274C220B4AB3 

Unzipping Block 1 Bytes Size 10/10
Signatures          sip40 BF68C674C2 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   2064442577

Modulus Scan Thread 22 Match 

0   1   248 203 188 181 130 65  40  26  
00  01  F8  CB  BC  B5  82  41  28  1A  


Unzipping Block 2 Bytes Size 10/10
Signatures          sip40 7253338C5C 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1428089023

Modulus Scan Thread 15 Match 

0   1   135 192 74  205 205 94  157 240 
00  01  87  C0  4A  CD  CD  5E  9D  F0  


Unzipping Block 3 Bytes Size 10/10
Signatures          sip40 87DA8B9654 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   3425898443

Modulus Scan Thread 31 Match 

0   1   202 178 237 254 1   128 33  204 
00  01  CA  B2  ED  FE  01  80  21  CC  


Unzipping Block 4 Bytes Size 10/10
Signatures          sip40 94A7B253BC 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   2990126813

Modulus Scan Thread 12 Match 

0   1   158 139 45  235 19  174 136 241 
00  01  9E  8B  2D  EB  13  AE  88  F1  


Unzipping Block 5 Bytes Size 10/10
Signatures          sip40 70E340400B 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   3612695702

Modulus Scan Thread 21 Match 

0   1   144 134 41  212 70  207 50  193 
00  01  90  86  29  D4  46  CF  32  C1  


Unzipping Block 6 Bytes Size 10/10
Signatures          sip40 BA2E3AE2A0 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1470353744

Modulus Scan Thread 4 Match 

0   1   189 68  156 226 154 95  52  108 
00  01  BD  44  9C  E2  9A  5F  34  6C  


Unzipping Block 7 Bytes Size 10/10
Signatures          sip40 6DF6C6C370 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   34475846

Modulus Scan Thread 2 Match 

0   1   6   169 83  0   251 100 188 68  
00  01  06  A9  53  00  FB  64  BC  44  


Unzipping Block 8 Bytes Size 10/10
Signatures          sip40 641FFE613F 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   1832211664

Modulus Scan Thread 27 Match 

0   1   249 18  206 57  116 34  134 149 
00  01  F9  12  CE  39  74  22  86  95  


Unzipping Block 9 Bytes Size 10/10
Signatures          sip40 EA5D623CD6 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   348555935

Modulus Scan Thread 23 Match 

0   1   15  90  12  86  5   108 126 72  
00  01  0F  5A  0C  56  05  6C  7E  48  


Unzipping Block 10 Bytes Size 10/10
Signatures          sip40 BDB08537F0 
Signatures keys     sip40 keys 8FD7542D47BAC03C623C4F7D5A022C8F 
Modulus Exponent    64
Modulus Remainder   534789339

Modulus Scan Thread 28 Match 

0   1   197 193 218 90  90  30  98  127 
00  01  C5  C1  DA  5A  5A  1E  62  7F  


The mdunzip output file validates
```
