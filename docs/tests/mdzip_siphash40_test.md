

```bash
Starting the mdzip  C++ sip40 non incrementing key
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

mdzipsip40 --file=randomfile --randbh --fh=11

Filename Details:   randomfile
Version:            1.01
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
Blockhash key list: sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
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
has160 A39C093CC47610A1B173BC63AF6A37BB350DBB87 

Zipping Block 1 Bytes Size 11/11
Block Bytes         00 00 01 61 91 F4 F4 C8 61 41 2C 
Signatures          sip40 1806606884 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   703804962

Zipping Block 2 Bytes Size 11/11
Block Bytes         00 00 01 E3 8F 48 6F 70 9B 9C F1 
Signatures          sip40 FC784DCF52 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1412097378

Zipping Block 3 Bytes Size 11/11
Block Bytes         00 00 01 84 F3 94 72 66 7B 0F D8 
Signatures          sip40 C3DD4E8E04 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3949896779

Zipping Block 4 Bytes Size 11/11
Block Bytes         00 00 01 E3 C6 D6 57 46 75 56 BF 
Signatures          sip40 3F66ED7290 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   708586776

Zipping Block 5 Bytes Size 11/11
Block Bytes         00 00 01 89 98 2E 54 61 8C 1E 40 
Signatures          sip40 0E8028BAAE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3945024661

Zipping Block 6 Bytes Size 11/11
Block Bytes         00 00 01 20 44 FA 5D 70 29 63 F5 
Signatures          sip40 13EB87DEDE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   2423152211

Zipping Block 7 Bytes Size 11/11
Block Bytes         00 00 01 69 97 2F D1 F3 D9 5F F0 
Signatures          sip40 DA1C88B46B 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1567657923

Zipping Block 8 Bytes Size 11/11
Block Bytes         00 00 01 D4 F5 26 BC 85 AE 72 BE 
Signatures          sip40 693D403867 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1520671100

Zipping Block 9 Bytes Size 11/11
Block Bytes         00 00 01 C6 23 18 60 12 96 50 48 
Signatures          sip40 6BBBF7D301 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3636029609

Zipping Block 10 Bytes Size 11/11
Block Bytes         00 00 01 98 47 D5 4B AD C1 40 25 
Signatures          sip40 C7CCE9A9BE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1175000434


Running mdlist randomfile.mdz

mdunzipsip40 --file=randomfile.mdz --list 

Zip Filename:       randomfile.mdz
Unzip Filename:     randomfile.mdz.out
Version:            1.01
Filesize:           110
Blocksize:          11
Blockcount:         10
Blockremainder:     11
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Blockkeylist:       sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
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
has160 A39C093CC47610A1B173BC63AF6A37BB350DBB87 

Displaying Block 1 Bytes Size 11/11
Signatures          sip40 1806606884 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   703804962

Displaying Block 2 Bytes Size 11/11
Signatures          sip40 FC784DCF52 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1412097378

Displaying Block 3 Bytes Size 11/11
Signatures          sip40 C3DD4E8E04 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3949896779

Displaying Block 4 Bytes Size 11/11
Signatures          sip40 3F66ED7290 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   708586776

Displaying Block 5 Bytes Size 11/11
Signatures          sip40 0E8028BAAE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3945024661

Displaying Block 6 Bytes Size 11/11
Signatures          sip40 13EB87DEDE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   2423152211

Displaying Block 7 Bytes Size 11/11
Signatures          sip40 DA1C88B46B 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1567657923

Displaying Block 8 Bytes Size 11/11
Signatures          sip40 693D403867 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1520671100

Displaying Block 9 Bytes Size 11/11
Signatures          sip40 6BBBF7D301 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3636029609

Displaying Block 10 Bytes Size 11/11
Signatures          sip40 C7CCE9A9BE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1175000434


Unzipping randomfile

mdunzipsip40 --file=randomfile.mdz --over --val=true

Zip Filename:       randomfile.mdz
Unzip Filename:     randomfile.mdz.out
Version:            1.01
Filesize:           110
Blocksize:          11
Blockcount:         10
Blockremainder:     11
Modsize:            32
Modsize Bytes:      4
Filehashlist:       has160
Blockhashlist:      sip40
Blockkeylist:       sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
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
has160 A39C093CC47610A1B173BC63AF6A37BB350DBB87 

Unzipping Block 1 Bytes Size 11/11
Signatures          sip40 1806606884 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   703804962

Modulus Scan Thread 21 Match 

0   0   1   97  145 244 244 200 97  65  44  
00  00  01  61  91  F4  F4  C8  61  41  2C  


Unzipping Block 2 Bytes Size 11/11
Signatures          sip40 FC784DCF52 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1412097378

Modulus Scan Thread 17 Match 

0   0   1   227 143 72  111 112 155 156 241 
00  00  01  E3  8F  48  6F  70  9B  9C  F1  


Unzipping Block 3 Bytes Size 11/11
Signatures          sip40 C3DD4E8E04 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3949896779

Modulus Scan Thread 19 Match 

0   0   1   132 243 148 114 102 123 15  216 
00  00  01  84  F3  94  72  66  7B  0F  D8  


Unzipping Block 4 Bytes Size 11/11
Signatures          sip40 3F66ED7290 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   708586776

Modulus Scan Thread 25 Match 

0   0   1   227 198 214 87  70  117 86  191 
00  00  01  E3  C6  D6  57  46  75  56  BF  


Unzipping Block 5 Bytes Size 11/11
Signatures          sip40 0E8028BAAE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3945024661

Modulus Scan Thread 21 Match 

0   0   1   137 152 46  84  97  140 30  64  
00  00  01  89  98  2E  54  61  8C  1E  40  


Unzipping Block 6 Bytes Size 11/11
Signatures          sip40 13EB87DEDE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   2423152211

Modulus Scan Thread 30 Match 

0   0   1   32  68  250 93  112 41  99  245 
00  00  01  20  44  FA  5D  70  29  63  F5  


Unzipping Block 7 Bytes Size 11/11
Signatures          sip40 DA1C88B46B 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1567657923

Modulus Scan Thread 19 Match 

0   0   1   105 151 47  209 243 217 95  240 
00  00  01  69  97  2F  D1  F3  D9  5F  F0  


Unzipping Block 8 Bytes Size 11/11
Signatures          sip40 693D403867 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1520671100

Modulus Scan Thread 30 Match 

0   0   1   212 245 38  188 133 174 114 190 
00  00  01  D4  F5  26  BC  85  AE  72  BE  


Unzipping Block 9 Bytes Size 11/11
Signatures          sip40 6BBBF7D301 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   3636029609

Modulus Scan Thread 1 Match 

0   0   1   198 35  24  96  18  150 80  72  
00  00  01  C6  23  18  60  12  96  50  48  


Unzipping Block 10 Bytes Size 11/11
Signatures          sip40 C7CCE9A9BE 
Signatures keys     sip40 keys 0F8D659D91C3E7DBDAE0DA0337C318A3 
Modulus Exponent    64
Modulus Remainder   1175000434

Modulus Scan Thread 13 Match 

0   0   1   152 71  213 75  173 193 64  37  
00  00  01  98  47  D5  4B  AD  C1  40  25  


The mdunzip output file validates
```
