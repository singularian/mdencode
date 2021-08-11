

```bash
Starting the mdzip C++ sip40 increment key
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

mdzipsip40 --file=randomfile --randbh --fh=11 --inc

Filename Details:   randomfile
Version:            1.12
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
Blockhash key list: sip40 keys 7FF1B03CA2EF99800A081081210EB85F 
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
has160 17AB8DCAB71E820A48CA0086E39AAE4F00DC968F 

Zipping Block 1 Bytes Size 11/11
Block Bytes         00 00 01 7F DE B1 76 94 7A F5 45 
Signatures          sip40 51855EF4D5 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB85F 
Modulus Exponent    64
Modulus Remainder   341419709

Zipping Block 2 Bytes Size 11/11
Block Bytes         00 00 01 B6 CA E7 7D D8 15 D1 E6 
Signatures          sip40 67839BA95C 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB860 
Modulus Exponent    64
Modulus Remainder   2397092197

Zipping Block 3 Bytes Size 11/11
Block Bytes         00 00 01 E3 01 62 AD FF C4 30 58 
Signatures          sip40 3FE3EAA25E 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB862 
Modulus Exponent    64
Modulus Remainder   3804599047

Zipping Block 4 Bytes Size 11/11
Block Bytes         00 00 01 E0 B2 30 6D 51 BD 55 27 
Signatures          sip40 946189B72A 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB865 
Modulus Exponent    64
Modulus Remainder   846169494

Zipping Block 5 Bytes Size 11/11
Block Bytes         00 00 01 FB F2 3D 8E A6 50 92 78 
Signatures          sip40 4C5F229938 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB869 
Modulus Exponent    64
Modulus Remainder   2722287624

Zipping Block 6 Bytes Size 11/11
Block Bytes         00 00 01 A7 6D 90 DF 6C 86 2F 9B 
Signatures          sip40 A0949EC1EC 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB86E 
Modulus Exponent    64
Modulus Remainder   334741628

Zipping Block 7 Bytes Size 11/11
Block Bytes         00 00 01 2B 2F 8D 1B 2E 08 83 1D 
Signatures          sip40 1FA202B2D2 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB874 
Modulus Exponent    64
Modulus Remainder   1496846393

Zipping Block 8 Bytes Size 11/11
Block Bytes         00 00 01 4B 94 9A 17 EA E5 2A 1E 
Signatures          sip40 9E8401B371 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB87B 
Modulus Exponent    64
Modulus Remainder   913949751

Zipping Block 9 Bytes Size 11/11
Block Bytes         00 00 01 87 94 87 45 7B E8 B7 FC 
Signatures          sip40 DC97F0D560 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB883 
Modulus Exponent    64
Modulus Remainder   58539843

Zipping Block 10 Bytes Size 11/11
Block Bytes         00 00 01 68 38 1D 8F FF C6 53 AA 
Signatures          sip40 8233F21194 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB88C 
Modulus Exponent    64
Modulus Remainder   1744728379


Running mdlist randomfile.mdz

mdunzipsip40 --file=randomfile.mdz --list 

Zip Filename:       randomfile.mdz
Unzip Filename:     randomfile.mdz.out
Version:            1.12
Filesize:           110
Blocksize:          11
Blockcount:         10
Blockremainder:     11
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Blockkeylist:       sip40 keys 7FF1B03CA2EF99800A081081210EB85F 
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
has160 17AB8DCAB71E820A48CA0086E39AAE4F00DC968F 

Displaying Block 1 Bytes Size 11/11
Signatures          sip40 51855EF4D5 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB85F 
Modulus Exponent    64
Modulus Remainder   341419709

Displaying Block 2 Bytes Size 11/11
Signatures          sip40 67839BA95C 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB860 
Modulus Exponent    64
Modulus Remainder   2397092197

Displaying Block 3 Bytes Size 11/11
Signatures          sip40 3FE3EAA25E 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB862 
Modulus Exponent    64
Modulus Remainder   3804599047

Displaying Block 4 Bytes Size 11/11
Signatures          sip40 946189B72A 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB865 
Modulus Exponent    64
Modulus Remainder   846169494

Displaying Block 5 Bytes Size 11/11
Signatures          sip40 4C5F229938 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB869 
Modulus Exponent    64
Modulus Remainder   2722287624

Displaying Block 6 Bytes Size 11/11
Signatures          sip40 A0949EC1EC 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB86E 
Modulus Exponent    64
Modulus Remainder   334741628

Displaying Block 7 Bytes Size 11/11
Signatures          sip40 1FA202B2D2 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB874 
Modulus Exponent    64
Modulus Remainder   1496846393

Displaying Block 8 Bytes Size 11/11
Signatures          sip40 9E8401B371 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB87B 
Modulus Exponent    64
Modulus Remainder   913949751

Displaying Block 9 Bytes Size 11/11
Signatures          sip40 DC97F0D560 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB883 
Modulus Exponent    64
Modulus Remainder   58539843

Displaying Block 10 Bytes Size 11/11
Signatures          sip40 8233F21194 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB88C 
Modulus Exponent    64
Modulus Remainder   1744728379


Unzipping randomfile

mdunzipsip40 --file=randomfile.mdz --over --val=true

Zip Filename:       randomfile.mdz
Unzip Filename:     randomfile.mdz.out
Version:            1.12
Filesize:           110
Blocksize:          11
Blockcount:         10
Blockremainder:     11
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Blockkeylist:       sip40 keys 7FF1B03CA2EF99800A081081210EB85F 
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
has160 17AB8DCAB71E820A48CA0086E39AAE4F00DC968F 

Unzipping Block 1 Bytes Size 11/11
Signatures          sip40 51855EF4D5 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB85F 
Modulus Exponent    64
Modulus Remainder   341419709

Modulus Scan Thread 23 Match 

0   0   1   127 222 177 118 148 122 245 69  
00  00  01  7F  DE  B1  76  94  7A  F5  45  


Unzipping Block 2 Bytes Size 11/11
Signatures          sip40 67839BA95C 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB860 
Modulus Exponent    64
Modulus Remainder   2397092197

Modulus Scan Thread 31 Match 

0   0   1   182 202 231 125 216 21  209 230 
00  00  01  B6  CA  E7  7D  D8  15  D1  E6  


Unzipping Block 3 Bytes Size 11/11
Signatures          sip40 3FE3EAA25E 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB862 
Modulus Exponent    64
Modulus Remainder   3804599047

Modulus Scan Thread 15 Match 

0   0   1   227 1   98  173 255 196 48  88  
00  00  01  E3  01  62  AD  FF  C4  30  58  


Unzipping Block 4 Bytes Size 11/11
Signatures          sip40 946189B72A 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB865 
Modulus Exponent    64
Modulus Remainder   846169494

Modulus Scan Thread 15 Match 

0   0   1   224 178 48  109 81  189 85  39  
00  00  01  E0  B2  30  6D  51  BD  55  27  


Unzipping Block 5 Bytes Size 11/11
Signatures          sip40 4C5F229938 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB869 
Modulus Exponent    64
Modulus Remainder   2722287624

Modulus Scan Thread 16 Match 

0   0   1   251 242 61  142 166 80  146 120 
00  00  01  FB  F2  3D  8E  A6  50  92  78  


Unzipping Block 6 Bytes Size 11/11
Signatures          sip40 A0949EC1EC 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB86E 
Modulus Exponent    64
Modulus Remainder   334741628

Modulus Scan Thread 1 Match 

0   0   1   167 109 144 223 108 134 47  155 
00  00  01  A7  6D  90  DF  6C  86  2F  9B  


Unzipping Block 7 Bytes Size 11/11
Signatures          sip40 1FA202B2D2 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB874 
Modulus Exponent    64
Modulus Remainder   1496846393

Modulus Scan Thread 28 Match 

0   0   1   43  47  141 27  46  8   131 29  
00  00  01  2B  2F  8D  1B  2E  08  83  1D  


Unzipping Block 8 Bytes Size 11/11
Signatures          sip40 9E8401B371 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB87B 
Modulus Exponent    64
Modulus Remainder   913949751

Modulus Scan Thread 25 Match 

0   0   1   75  148 154 23  234 229 42  30  
00  00  01  4B  94  9A  17  EA  E5  2A  1E  


Unzipping Block 9 Bytes Size 11/11
Signatures          sip40 DC97F0D560 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB883 
Modulus Exponent    64
Modulus Remainder   58539843

Modulus Scan Thread 7 Match 

0   0   1   135 148 135 69  123 232 183 252 
00  00  01  87  94  87  45  7B  E8  B7  FC  


Unzipping Block 10 Bytes Size 11/11
Signatures          sip40 8233F21194 
Signatures keys     sip40 keys 7FF1B03CA2EF99800A081081210EB88C 
Modulus Exponent    64
Modulus Remainder   1744728379

Modulus Scan Thread 17 Match 

0   0   1   104 56  29  143 255 198 83  170 
00  00  01  68  38  1D  8F  FF  C6  53  AA  


The mdunzip output file validates
```		
