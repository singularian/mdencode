

```bash
Starting the mdzip  C++ sip40 decrement key
Creating randomfile

rm -f randomfile

randfile2 -blocknum=10 -bsize=11 -zsize=3 -first=1 -out=randomfile

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

mdzipsip40 --file=randomfile --randbh --fh=11 --dec

Filename Details:   randomfile
Version:            1.11
Filesize:           110
Blocksize:          11
Blockcount:         10
Blockremainder:     11
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Filehashlist size:  6
Blockhashlist size: 5
Blockhash key list: sip40 keys 94A1D9893ED8F990FC2DB347DB8995B6 
File Hash Bytes:    20
Block Hash Bytes:   5
Platform:           Little Endian

File hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           11           has160      20          0          
                         Total       20          0

File block hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           29           sip40       5           16         
                         Total       5           16

File Signatures 
has160 8EB821717D5165B4705275538E5DD02852F01ECC 

Zipping Block 1 Bytes Size 11/11
Block Bytes         00 00 01 BD C7 10 CC CA 25 F5 43 
Signatures          sip40 E2C17C5DE1 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B6 
Modulus Exponent    64
Modulus Remainder   2280457745

Zipping Block 2 Bytes Size 11/11
Block Bytes         00 00 01 58 EE 44 7C 22 4B 21 B0 
Signatures          sip40 7C0B290B9C 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B5 
Modulus Exponent    64
Modulus Remainder   2067359277

Zipping Block 3 Bytes Size 11/11
Block Bytes         00 00 01 06 A7 0B 39 C2 4C 1D 7C 
Signatures          sip40 DB6F0E5DC6 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B3 
Modulus Exponent    64
Modulus Remainder   3371378870

Zipping Block 4 Bytes Size 11/11
Block Bytes         00 00 01 5D 66 8D D3 8C EE 5A 54 
Signatures          sip40 C6F6373939 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B0 
Modulus Exponent    64
Modulus Remainder   3931433000

Zipping Block 5 Bytes Size 11/11
Block Bytes         00 00 01 23 EB FF 31 83 BC C2 2C 
Signatures          sip40 DF7FB787BD 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995AC 
Modulus Exponent    64
Modulus Remainder   2812854622

Zipping Block 6 Bytes Size 11/11
Block Bytes         00 00 01 5E A8 ED 39 B9 CA E6 31 
Signatures          sip40 59E3938EDC 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995A7 
Modulus Exponent    64
Modulus Remainder   410243948

Zipping Block 7 Bytes Size 11/11
Block Bytes         00 00 01 31 85 BB 6B B4 A4 A5 93 
Signatures          sip40 18286590CC 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995A1 
Modulus Exponent    64
Modulus Remainder   3861537023

Zipping Block 8 Bytes Size 11/11
Block Bytes         00 00 01 CF B4 93 D4 1F 79 A7 24 
Signatures          sip40 87022539E2 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB89959A 
Modulus Exponent    64
Modulus Remainder   4012784377

Zipping Block 9 Bytes Size 11/11
Block Bytes         00 00 01 D9 66 D2 A5 75 51 69 E0 
Signatures          sip40 0554DBE544 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB899592 
Modulus Exponent    64
Modulus Remainder   1320696967

Zipping Block 10 Bytes Size 11/11
Block Bytes         00 00 01 B1 B1 3E F5 87 88 62 BE 
Signatures          sip40 B502404543 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB899589 
Modulus Exponent    64
Modulus Remainder   960078261


Running mdlist randomfile.mdz

mdunzipsip40 --file=randomfile.mdz --list 

Zip Filename:       randomfile.mdz
Unzip Filename:     randomfile.mdz.out
Version:            1.11
Filesize:           110
Blocksize:          11
Blockcount:         10
Blockremainder:     11
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Blockkeylist:       sip40 keys 94A1D9893ED8F990FC2DB347DB8995B6 
File Hash Bytes:    20
Block Hash Bytes:   5
Platform:           Little Endian

File hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           11           has160      20          0          
                         Total       20          0

File block hashlist 
Number      Hash ID      Hash Name   Blocksize   Blockkeysize
1           29           sip40       5           16         
                         Total       5           16

File Signatures 
has160 8EB821717D5165B4705275538E5DD02852F01ECC 

Displaying Block 1 Bytes Size 11/11
Signatures          sip40 E2C17C5DE1 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B6 
Modulus Exponent    64
Modulus Remainder   2280457745

Displaying Block 2 Bytes Size 11/11
Signatures          sip40 7C0B290B9C 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B5 
Modulus Exponent    64
Modulus Remainder   2067359277

Displaying Block 3 Bytes Size 11/11
Signatures          sip40 DB6F0E5DC6 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B3 
Modulus Exponent    64
Modulus Remainder   3371378870

Displaying Block 4 Bytes Size 11/11
Signatures          sip40 C6F6373939 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B0 
Modulus Exponent    64
Modulus Remainder   3931433000

Displaying Block 5 Bytes Size 11/11
Signatures          sip40 DF7FB787BD 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995AC 
Modulus Exponent    64
Modulus Remainder   2812854622

Displaying Block 6 Bytes Size 11/11
Signatures          sip40 59E3938EDC 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995A7 
Modulus Exponent    64
Modulus Remainder   410243948

Displaying Block 7 Bytes Size 11/11
Signatures          sip40 18286590CC 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995A1 
Modulus Exponent    64
Modulus Remainder   3861537023

Displaying Block 8 Bytes Size 11/11
Signatures          sip40 87022539E2 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB89959A 
Modulus Exponent    64
Modulus Remainder   4012784377

Displaying Block 9 Bytes Size 11/11
Signatures          sip40 0554DBE544 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB899592 
Modulus Exponent    64
Modulus Remainder   1320696967

Displaying Block 10 Bytes Size 11/11
Signatures          sip40 B502404543 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB899589 
Modulus Exponent    64
Modulus Remainder   960078261


Unzipping randomfile

mdunzipsip40 --file=randomfile.mdz --over --val=true

Zip Filename:       randomfile.mdz
Unzip Filename:     randomfile.mdz.out
Version:            1.11
Filesize:           110
Blocksize:          11
Blockcount:         10
Blockremainder:     11
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Blockkeylist:       sip40 keys 94A1D9893ED8F990FC2DB347DB8995B6 
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
1           29           sip40       5           16         
                         Total       5           16

File Signatures 
has160 8EB821717D5165B4705275538E5DD02852F01ECC 

Unzipping Block 1 Bytes Size 11/11
Signatures          sip40 E2C17C5DE1 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B6 
Modulus Exponent    64
Modulus Remainder   2280457745

Modulus Scan Thread 13 Match 

0   0   1   189 199 16  204 202 37  245 67  
00  00  01  BD  C7  10  CC  CA  25  F5  43  


Unzipping Block 2 Bytes Size 11/11
Signatures          sip40 7C0B290B9C 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B5 
Modulus Exponent    64
Modulus Remainder   2067359277

Modulus Scan Thread 29 Match 

0   0   1   88  238 68  124 34  75  33  176 
00  00  01  58  EE  44  7C  22  4B  21  B0  


Unzipping Block 3 Bytes Size 11/11
Signatures          sip40 DB6F0E5DC6 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B3 
Modulus Exponent    64
Modulus Remainder   3371378870

Modulus Scan Thread 26 Match 

0   0   1   6   167 11  57  194 76  29  124 
00  00  01  06  A7  0B  39  C2  4C  1D  7C  


Unzipping Block 4 Bytes Size 11/11
Signatures          sip40 C6F6373939 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995B0 
Modulus Exponent    64
Modulus Remainder   3931433000

Modulus Scan Thread 20 Match 

0   0   1   93  102 141 211 140 238 90  84  
00  00  01  5D  66  8D  D3  8C  EE  5A  54  


Unzipping Block 5 Bytes Size 11/11
Signatures          sip40 DF7FB787BD 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995AC 
Modulus Exponent    64
Modulus Remainder   2812854622

Modulus Scan Thread 18 Match 

0   0   1   35  235 255 49  131 188 194 44  
00  00  01  23  EB  FF  31  83  BC  C2  2C  


Unzipping Block 6 Bytes Size 11/11
Signatures          sip40 59E3938EDC 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995A7 
Modulus Exponent    64
Modulus Remainder   410243948

Modulus Scan Thread 27 Match 

0   0   1   94  168 237 57  185 202 230 49  
00  00  01  5E  A8  ED  39  B9  CA  E6  31  


Unzipping Block 7 Bytes Size 11/11
Signatures          sip40 18286590CC 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB8995A1 
Modulus Exponent    64
Modulus Remainder   3861537023

Modulus Scan Thread 12 Match 

0   0   1   49  133 187 107 180 164 165 147 
00  00  01  31  85  BB  6B  B4  A4  A5  93  


Unzipping Block 8 Bytes Size 11/11
Signatures          sip40 87022539E2 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB89959A 
Modulus Exponent    64
Modulus Remainder   4012784377

Modulus Scan Thread 21 Match 

0   0   1   207 180 147 212 31  121 167 36  
00  00  01  CF  B4  93  D4  1F  79  A7  24  


Unzipping Block 9 Bytes Size 11/11
Signatures          sip40 0554DBE544 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB899592 
Modulus Exponent    64
Modulus Remainder   1320696967

Modulus Scan Thread 7 Match 

0   0   1   217 102 210 165 117 81  105 224 
00  00  01  D9  66  D2  A5  75  51  69  E0  


Unzipping Block 10 Bytes Size 11/11
Signatures          sip40 B502404543 
Signatures keys     sip40 keys 94A1D9893ED8F990FC2DB347DB899589 
Modulus Exponent    64
Modulus Remainder   960078261

Modulus Scan Thread 23 Match 

0   0   1   177 177 62  245 135 136 98  190 
00  00  01  B1  B1  3E  F5  87  88  62  BE  


The mdunzip output file validates
```