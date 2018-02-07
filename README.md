# Project mdencode :octocat:
mdencode is a golang command line file digital signature generator with modular floors.  
mdencode is written in golang and uses an interface for pluggin formatters.  
mdencode allows for the creation of sqlite3 db file signatures and the first random file signatures. 

This program can generate a file signature at the file or block level and format and write the output in multiple formats.  
It can also write to an sqlite db. The mdencode sqlite db files contain a table schema and can be exported into multiple formats.  

# Features

- Versioned Cryptographic File Signatures
- Optional File Crypto Signatures
- Optional N-Bit Sized File Block Modular Floors
- Optional N-Byte Block Crypto Signatures
- SQLite3 DB Crypto Signature Files
- Plugin Formatters
- Multiple Selectable Crypto Hash Signatures for Files and File Blocks
- The "First" Random File or Block signatures 

# Output Formats

- TEXT
- INFORM
- XML
- JSON
- BINARY
- CSV
- SQLITE3 DB
- BUILD YOUR OWN

# Digital Signature List

This is a partial list of the digital signatures. More can be added to the signature chain and are selectable via a binary flag argument for file blocks and files

- fnv
- md4
- md5
- ripe160
- sha
- sha3
- skein
- Tiger
- Whirlpool



# Mdencode usage
This is the mdencode usage.
mdencode has three main golang programs. 

- md or mdencode - generates file signatures
- mddbreport - generates formatted signatures dumps of file signatures stored in a mdencode sqlite3 db
- mdnewdb  - creates new sqlite3 mdencode signatures

mdencode or md can generate a signature for a file or directory and a modular floor

mdencode can generate random file signatures and block signatures.
mdencode is split into encode and decode parts.

Random file signatures are file signatures that can change block or file hash lists and block size randomly from program executions.
In one run the mdencode program can run ripe160, sha1, sha224 and whirlpool on a file while in another run it might run just md4 and sha1.
This allows for a file signature to have the lists randomized and improves security by making signatures unpredictable. The signature lists
for mdencode are bit strings which can be random bit strings. This allows for the first random file signatures.

```
Usage of md:

  -fh string
     File Hash Boolean String List (default "011")     
  -bh string  
     Block Hash Boolean String List (default "011")   
  -fr
     Generate A Random File Hash Boolean String List
  -br
     Generate A Random Block Hash Boolean String List
  -fbr
     Generate A Random File and Block Hash Boolean String List
  -block string  
     File Block Size Bytes (default "40")  
  -mod string  
     Modulus Size in Bits (default "32")       
  -append  
     Append To Output File     
  -file string  
     Input filename  
  -dir string
     Input Directory
  -format int  
     Output Format (default 10)
  -initdb string
     Create SQLite3 Empty DB File      
  -key string  
     Signature Key (Minimum 16 bytes for siphash) (default "LomaLindaSanSerento9000")  
  -line  
     File Hash as one line  
  -log string  
     Log Filename  
  -byte  
     Append the File Byteblock to the Hash List  
  -blockint  
     Append the File Byteblock Bigint to the Hash List  
  -out string  
     Output Filename  

Version: 1.0 復甦 復活

Formats:
0 .. 98 - Text
101     - CSV
102     - CSV
1000    - Binary
2000    - SQL Lite 3 DB File
3000    - Inform
4000    - JSON
5000    - GO XML
5001    - XML Formatter

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
```

# Output Formats

This is a list of the current numerical format assignment.  
This is subject to change.  
```sh
0 .. 98 - Text
101     - CSV
102     - CSV
1000    - Binary
2000    - SQL Lite 3 DB File
3000    - Inform
4000    - JSON
5000    - GO XML
5001    - XML Formatter
```

# Command Line Examples

```sh
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
```

# Eample usage

This is a file signature for Makefile with a block size of 100 bytes and a 128 bit modulus and the fileblock signature bit string and file bit string and a format specified by number 10. It is formatted by go flags args.

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=10 -file=america.txt -out=america.txt_b100_mod128_f9.md
```

[Text Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_f9.md)

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=5000 -file=america.txt -out=america.txt_b100_mod128_xml.md
```

[XML Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_xml.md)

```
md -block=100 -mod=128 -fh=10000111 -bh=10001111 -format=4000 -file=america.txt -out=america.txt_b100_mod128_xml.md
```

[JSON Example](https://raw.githubusercontent.com/singularian/mdencode/master/examples/america.txt_b100_mod128_json.md)


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

# Using mddbreport 

mddbreport is a utility to display the singatures in an sqlite3 db signature file.
This example shows how to generate an sqlite3 file db signature

```
md -file=Makefile -format=2000 -line=true -out=sqlite3_example
```

It can also generate a formatted signature from the sqlite3_example.db sqlite 3 file

This example shows how to list the files in a sqlite3_example.db file sqlite3 signature db

The columns are:

file id, filepath, filename, version

```
$ mddbreport -file=sqlite3_example.db

file 1: /projects/src/github.com/singularian/mdencode/code/encode/ Makefile 1.0  

$ mddbreport -file=filesig.db  

file 1: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ america.txt 1.0  
file 2: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ twelve.doc 1.0  
file 3: /home/nsross/projects/src/github.com/singularian/mdencode/examples/ century.xml 1.0  
```

# Example mddbreport formats

This example shows how to use the mddbreport utility to display the file sqlite3 db signatures in a numerical format. 

```
$ mddbreport -file=example.db -fileid=1
```

[example.db.txt](https://raw.githubusercontent.com/singularian/mdencode/master/examples/example.db.txt)

```
$ mddbreport -file=example.db -fileid=1 -format=4000

```
[example.db.json](https://raw.githubusercontent.com/singularian/mdencode/master/examples/example.db.json)

```
$ mddbreport -file=example.db -fileid=1 -format=5001
```
[example.db.xml](https://raw.githubusercontent.com/singularian/mdencode/master/examples/example.db.xml)

If zero is specified for the argument it will format and output all the signature db files in the numeric format
```
$ mddbreport -file=example.db -fileid=0 -format=100
```

# Modular Floor

This is my block chopper.  

![This is my block chopper](https://github.com/singularian/mdencode/blob/master/diagrams/blockchopper.jpg "Coyau's Cleaver")

This program uses a modular floor paired with a package of digital signatures. This makes the digital signature more unique. 
A modular floor consists of a polynomial which is the modular exponent plus n times the modulus plus the remainder. 
It is possible to use a modular scan to find matching byte blocks associated with the digital signature package.
A digital signature package or group can consist of 1 or more block and file signatures. 
A modular floor is generaly computed at the block level for a file byte block and utilizes a big integer.

The modulus scan polynomial is calculated as follows.

```
modremainder    = fileblockbigint % modulus  
fileblockbigint = modulus * n + modremainder where n equals 0 to m  
fileblockbigint = modulus ^ (exponent p) + modulus * n + modremainder where p is an optional power and n equals 0 to m    
```

# Modular Floor Equation

This is the modular floor equation for two forms. One is the exponential floor while the other contains a multiple of the modular floor plus the remainder.
A modulus should be considered part of the file signature subset. A modulus is a digital signature.

![](https://github.com/singularian/mdencode/blob/master/diagrams/modularfloor.png)  
![](https://github.com/singularian/mdencode/blob/master/diagrams/modularfloorexponent.png)  

# Modular Floor Examples
  
This is an example of the modular floor.  
The program `decoderRandom` is creating a random 6 byte array and then running a modulus scan to find the corresponding file block associated with a digital signature.  
It creates a random 6 byte array and a 32 bit modulus and a modulus exponent and then creates a sha1 hash and md5 hash.  
It calculated and found a hashed 6 byte block in two seconds on a Pentium n3700.  

`$GOPATH/github.com/singularian/mdencode/code/decode/decoderRandom`
```
$ ./decoderRandom
random  6  bytes  [175 149 193 192 35 31]
modulus size bits  32
byte modulus  4294967296
block modulus  3250594591
modulus exponent  47
shasum  dc483a03987b1e24cc6e235671767b2c6ce17dd1
md5sum  a09f627e7dc71be69f7d0f20a214864f
modulo bigint  4294967296
modulo floor  140737488355328
modulo ceil  281474976710656
Found Block  [175 149 193 192 35 31]
total time  71.219466ms
random bytestring and modulusscan bytestring match  [175 149 193 192 35 31]   [175 149 193 192 35 31]
```
This example of decoderRandom uses a 32-bit and 24-bit modulus to calculate a 6 byte block associated with an sha1 and md5 signature.
```
$ ./decoderRandom
random  6  bytes  [139 111 156 34 193 144]
modulus size bits  32
byte modulus  4294967296
block modulus  2619523472
modulus exponent  47
shasum  af2757f99333d0f693cb70d5ff0e1caaf90cdb50
md5sum  448497bb2b4327b46de2d7cfc6b351c7
modulo bigint  4294967296
modulo floor  140737488355328
modulo ceil  281474976710656
Found Block  [139 111 156 34 193 144]
total time  58.27042ms
random bytestring and modulusscan bytestring match  [139 111 156 34 193 144]   [139 111 156 34 193 144]

random  6  bytes   [221 166 184 36 29 8]
modulus size bits  24
byte modulus  16777216
block modulus  2366728
modulus exponent  47
shasum  c3a9edb837e4d3fbc7a16f24f4f789f2ca758e32
md5sum  7f5dacfd281a1732880232315e12d3b4
modulo bigint  16777216
modulo floor  140737488355328
modulo ceil  281474976710656
Found Block  [221 166 184 36 29 8]
total time  15.123651922s
random bytestring and modulusscan bytestring match  [221 166 184 36 29 8]   [221 166 184 36 29 8]
```

This is a modulus scan on a 7 byte random block [21 209 197 154 67 38 91] with a 32-bit modulus.

```
$ ./decoderRandom 7 32 

random  7  bytes  [21 209 197 154 67 38 91]   6141621137778267
modulus size bits  32
byte modulus  4294967296
block modulus  2588091995
modulus exponent  52
shasum  150bb90a2f5cc13d9ce4b9ca7a97cf9272becf6b
md5sum  cbaa0f63b037ce8c45b6827462905188
modulo bigint  4294967296
modulo floor  4503599627370496  ceil  9007199254740992   2
modulo floor UUUU  52   4503599627370496  ceil  9007199254740992  mod  4294967296
modulo floor  4503599627370496
mfloor  4503599627370496   0
modulo floor equals zero  0   0
modulo floor equals zero setting  4503599627370496   4503599627370496
modremainder  4503599627370496   4503599627370496
modstart test result floor  4503602215462491  initial remainder  4503599627370496
Found Block  [21 209 197 154 67 38 91]
total time  453.257177ms
random bytestring and modulusscan bytestring match  [21 209 197 154 67 38 91]   [21 209 197 154 67 38 91]
```

This is a modulus scan test run of the decoderRandom and an 8-byte block with a 48-bit modulus

```
$ ./decoderRandom 8 48

random  8  bytes  [177 64 44 204 68 188 251 64]   12772257799060912960
modulus size bits  48
byte modulus  281474976710656
block modulus  49255838186304
modulus exponent  63
shasum  6b044398c783f714cdde112bdfa54b3e3fc27478
md5sum  2927e31d7947053d296ec4c66b740f43
modulo bigint  281474976710656
modulo floor  9223372036854775808  ceil  18446744073709551616   2
modulo floor UUUU  63   9223372036854775808  ceil  18446744073709551616  mod  281474976710656
modulo floor  9223372036854775808
mfloor  9223372036854775808   0
modulo floor equals zero  0   0
modulo floor equals zero setting  9223372036854775808   9223372036854775808
modremainder  9223372036854775808   9223372036854775808
modstart test result floor  9223421292692962112  initial remainder  9223372036854775808
Found Block  [177 64 44 204 68 188 251 64]
total time  22.83303ms
random bytestring and modulusscan bytestring match  [177 64 44 204 68 188 251 64]   [177 64 44 204 68 188 251 64]
```

This is a modulus scan on a 24 byte random block with a 160-bit modulus.

```
$ ./decoderRandom 24 160

random  24  bytes  [97 18 163 169 250 131 29 233 141 20 233 130 167 50 111 11 170 249 117 163 189 132 162 246]   2380218370871604582033952342137589732174749121711727485686
modulus size bits  160
byte modulus  1461501637330902918203684832716283019655932542976
block modulus  1430171696056580635811329467257220397964567290614
modulus exponent  190
shasum  b771dd3bca98fd0c4500f80ff5b83163422d7a78
md5sum  24377890c89f8d0fd7f6759a1f781b3f
modulo bigint  1461501637330902918203684832716283019655932542976
modulo floor  1569275433846670190958947355801916604025588861116008628224  ceil  3138550867693340381917894711603833208051177722232017256448   2
modulo floor UUUU  190   1569275433846670190958947355801916604025588861116008628224  ceil  3138550867693340381917894711603833208051177722232017256448  mod  1461501637330902918203684832716283019655932542976
modulo floor  1569275433846670190958947355801916604025588861116008628224
mfloor  1569275433846670190958947355801916604025588861116008628224   0
modulo floor equals zero  0   0
modulo floor equals zero setting  1569275433846670190958947355801916604025588861116008628224   1569275433846670190958947355801916604025588861116008628224
modremainder  1569275433846670190958947355801916604025588861116008628224   1569275433846670190958947355801916604025588861116008628224
modstart test result floor  1569275435276841887015527991613246071282809259080575918838  initial remainder  1569275433846670190958947355801916604025588861116008628224
Found Block  [97 18 163 169 250 131 29 233 141 20 233 130 167 50 111 11 170 249 117 163 189 132 162 246]
total time  10m13.626655508s
random bytestring and modulusscan bytestring match  [97 18 163 169 250 131 29 233 141 20 233 130 167 50 111 11 170 249 117 163 189 132 162 246]   [97 18 163 169 250 131 29 233 141 20 233 130 167 50 111 11 170 249 117 163 189 132 162 246]
```

This is a test modulus scan on a 30 byte random block and a 208-bit modulus. It computed the file block in 30 minutes.

```
$ ./decoderRandom 30 208
random  30  bytes  [224 32 77 208 119 140 50 111 148 112 51 6 119 78 217 169 127 226 139 12 18 102 11 24 198 47 34 138 150 8]   1546862094779236226658336428465289893654427126483713938810659946210432520
modulus size bits  208
byte modulus  411376139330301510538742295639337626245683966408394965837152256
block modulus  192105658193431839085922428518307514071415196907139728478475784
modulus exponent  239
shasum  d587f1226783a54d812b8b5bb866eccbff7a9fc7
md5sum  fb5050193ac2605e54acd7538743b73b
modulo bigint  411376139330301510538742295639337626245683966408394965837152256
modulo floor  883423532389192164791648750371459257913741948437809479060803100646309888  ceil  1766847064778384329583297500742918515827483896875618958121606201292619776   2
modulo floor UUUU  239   883423532389192164791648750371459257913741948437809479060803100646309888  ceil  1766847064778384329583297500742918515827483896875618958121606201292619776  mod  411376139330301510538742295639337626245683966408394965837152256
modulo floor  883423532389192164791648750371459257913741948437809479060803100646309888
mfloor  883423532389192164791648750371459257913741948437809479060803100646309888   0
modulo floor equals zero  0   0
modulo floor equals zero setting  883423532389192164791648750371459257913741948437809479060803100646309888   883423532389192164791648750371459257913741948437809479060803100646309888
modremainder  883423532389192164791648750371459257913741948437809479060803100646309888   883423532389192164791648750371459257913741948437809479060803100646309888
modstart test result floor  883423532581297822985080589457381686432049462509224675967942829124785672  initial remainder  883423532389192164791648750371459257913741948437809479060803100646309888
Found Block  [224 32 77 208 119 140 50 111 148 112 51 6 119 78 217 169 127 226 139 12 18 102 11 24 198 47 34 138 150 8]
total time  30m40.336738171s
random bytestring and modulusscan bytestring match  [224 32 77 208 119 140 50 111 148 112 51 6 119 78 217 169 127 226 139 12 18 102 11 24 198 47 34 138 150 8]   [224 32 77 208 119 140 50 111 148 112 51 6 119 78 217 169 127 226 139 12 18 102 11 24 198 47 34 138 150 8]
```

This is a test modulus scan on a 40 byte random block and a 288-bit modulus. It computed the file block in under 2 minutes.

```
random  40  bytes  [36 70 9 158 250 39 41 175 153 109 249 79 24 40 173 31 178 205 163 246 213 57 62 47 80 202 73 3 115 151 222 203 79 64 44 122 214 8 239 55]   302655882115645057855398278526862402649221125066903900403798877130708027364550200878646484856631
modulus size bits  288
byte modulus  497323236409786642155382248146820840100456150797347717440463976893159497012533375533056
block modulus  485964412207264705374655695519929829686550492959203335305120360412231041563571473936183
modulus exponent  317
shasum  9ee342f8be609f69a6eb6d2697f0670840e60c0d
md5sum  f5c4ebbdfb3ee5ee40039d47cfd3b504
modulo bigint  497323236409786642155382248146820840100456150797347717440463976893159497012533375533056
modulo floor  266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072  ceil  533996758980227520598755426542388028650676130589163192486760401955554931445160137505740521734144   2
modulo floor UUUU  317   266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072  ceil  533996758980227520598755426542388028650676130589163192486760401955554931445160137505740521734144  mod  497323236409786642155382248146820840100456150797347717440463976893159497012533375533056
modulo floor  266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072
mfloor  266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072   0
modulo floor equals zero  0   0
modulo floor equals zero setting  266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072   266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072
modremainder  266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072   266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072
modstart test result floor  266998379976078172506642418645849709845267894981132089202583536282897826134811110316441734803255  initial remainder  266998379490113760299377713271194014325338065294581596243380200977777465722580068752870260867072
Found Block  [36 70 9 158 250 39 41 175 153 109 249 79 24 40 173 31 178 205 163 246 213 57 62 47 80 202 73 3 115 151 222 203 79 64 44 122 214 8 239 55]
total time  1m24.594897512s
random bytestring and modulusscan bytestring match  [36 70 9 158 250 39 41 175 153 109 249 79 24 40 173 31 178 205 163 246 213 57 62 47 80 202 73 3 115 151 222 203 79 64 44 122 214 8 239 55]   [36 70 9 158 250 39 41 175 153 109 249 79 24 40 173 31 178 205 163 246 213 57 62 47 80 202 73 3 115 151 222 203 79 64 44 122 214 8 239 55]
```

This is a test modulus scan on a 1000 byte block with a 7968 byte modulus  
[mddecode1000.txt](https://raw.githubusercontent.com/singularian/mdencode/master/examples/mddecode1000.txt)

# Go Library Dependencies

go get -u golang.org/x/crypto/blake2s  
go get -u golang.org/x/crypto/md4  
go get -u golang.org/x/crypto/sha3  
go get -u golang.org/x/crypto/ripemd160  
go get -u github.com/steakknife/keccak  
go get -u github.com/asadmshah/murmur3  
go get -u github.com/dchest/siphash  
go get -u github.com/aead/skein  
go get -u github.com/cxmcc/tiger  
go get -u github.com/jzelinskie/whirlpool  
go get -u github.com/mattn/go-sqlite3  

# Build

##### Basic Linux / Unix Go Build

```
git clone https://github.com/singularian/mdencode.git
```
make all builds mdencode to gobin by default
```
$GOPATH/github.com/singularian/mdencode/code/encode $ make all
```

make build builds the mdencode to the mdBuild directory
```
$GOPATH/github.com/singularian/mdencode/code/encode $ make build
```

This example builds the mdencode decoder files
```
$GOPATH/github.com/singularian/mdencode/code/decode $ make build
```

# Donations

* BTC: `31ixWCPg2Ax5uhYk418C4KBFacnnri9Axb`
* ETH: `0x68907c4ac25e2041A797Ce8c639994cf9EbFdc8C`

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
