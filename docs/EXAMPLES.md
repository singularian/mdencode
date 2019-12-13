# Project mdencode :octocat:
mdencode is a golang command line file digital signature generator with modular floors.  
mdencode is written in golang and uses an interface for pluggin formatters.  
mdencode allows for the creation of sqlite3 db file signatures and the first random file signatures. 

This program can generate a file signature at the file or block level and format and write the output in multiple formats.  
It can also write to an sqlite db. The mdencode sqlite db files contain a table schema and can be exported into multiple formats.  

# Modular Floor Equation

This is the modular floor equation for two forms. One is the exponential floor while the other contains a multiple of the modular floor plus the remainder.
A modulus should be considered part of the file signature subset. A modulus is a digital signature.

![](https://github.com/singularian/mdencode/blob/master/diagrams/modularfloor.png)  
![](https://github.com/singularian/mdencode/blob/master/diagrams/modularfloorexponent.png)  

# Modular Floor Examples
  
This is an example of the modular floor.  
The program `decoderRandom` is creating a random 6 byte array and then running a modulus scan to find the corresponding file block associated with a digital signature.  
It creates a random 10 byte array and a 64 bit modulus and a modulus exponent and then creates a sha1 hash and md5 hash.  
A modulus scan can use any number of signatures including ripe160 or sha3.
It calculated and found a hashed 10 byte block in two seconds on a Ryzen 2700x with 16 threads.  

`$GOPATH/github.com/singularian/mdencode/code/testdecode/decoderRandom`
```
$ ~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=10 -mod=64 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  10  byteblock  [170 182 97 211 89 129 255 229 35 197]
Not Found Block  [255 241 97 211 89 129 255 229 35 197]
Total time  2.733ms
Not Found Block  [255 243 97 211 89 129 255 229 35 197]
Not Found Block  [255 249 97 211 89 129 255 229 35 197]
Total time  2.909ms
Total time  3.495ms
Not Found Block  [255 242 97 211 89 129 255 229 35 197]
Total time  2.606ms
Not Found Block  [255 253 97 211 89 129 255 229 35 197]
Total time  2.036ms
Found Block  [170 182 97 211 89 129 255 229 35 197]
Total time  848µs
random bytestring and modulusscan bytestring match  [170 182 97 211 89 129 255 229 35 197]   [170 182 97 211 89 129 255 229 35 197]
Found block  thread 6 random bytestring and modulusscan bytestring match [170 182 97 211 89 129 255 229 35 197] [170 182 97 211 89 129 255 229 35 197]
```
This example of decoderRandom uses a 32-bit modulus to calculate a 9 byte block associated with an sha1 and md5 signature.
Total computation time on a Ryzen 2700x with 16 threads was 32 minutes 5 seconds.
```
$ :~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=9 -mod=32 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  9  byteblock  [36 228 92 238 39 36 109 236 20]
Found Block  [36 228 92 238 39 36 109 236 20]
Total time  32m5.668088s
random bytestring and modulusscan bytestring match  [36 228 92 238 39 36 109 236 20]   [36 228 92 238 39 36 109 236 20]
Found block  thread 7 random bytestring and modulusscan bytestring match [36 228 92 238 39 36 109 236 20] [36 228 92 238 39 36 109 236 20]

```

This is a modulus scan on a 8 byte random block [53 132 218 83 11 69 48 238] with a 32-bit modulus and 16 goroutine threads.

```
$ ./decoderRandom -block=8 -mod=32 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  8  byteblock  [53 132 218 83 11 69 48 238]
Found Block  [53 132 218 83 11 69 48 238]
Total time  27.344133s
random bytestring and modulusscan bytestring match  [53 132 218 83 11 69 48 238]   [53 132 218 83 11 69 48 238]
Found block  thread 3 random bytestring and modulusscan bytestring match [53 132 218 83 11 69 48 238] [53 132 218 83 11 69 48 238]
```

This is a modulus scan test run with an 9-byte block with a 48-bit modulus.
Total computation time was 149 ms.

```
$ :~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=9 -mod=48 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  9  byteblock  [86 70 2 131 209 24 39 111 115]
Found Block  [86 70 2 131 209 24 39 111 115]
Total time  149.567ms
random bytestring and modulusscan bytestring match  [86 70 2 131 209 24 39 111 115]   [86 70 2 131 209 24 39 111 115]
Found block  thread 2 random bytestring and modulusscan bytestring match [86 70 2 131 209 24 39 111 115] [86 70 2 131 209 24 39 111 115]

```

This is a modulus scan on a 24 byte random block with a 160-bit modulus.
Total computation time was 13.48 seconds.
```
$ :~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=24 -mod=160 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  24  byteblock  [136 153 119 145 195 82 220 142 182 125 229 102 13 132 97 240 51 1 248 203 77 213 29 61]
Found Block  [136 153 119 145 195 82 220 142 182 125 229 102 13 132 97 240 51 1 248 203 77 213 29 61]
Total time  13.485776s
random bytestring and modulusscan bytestring match  [136 153 119 145 195 82 220 142 182 125 229 102 13 132 97 240 51 1 248 203 77 213 29 61]   [136 153 119 145 195 82 220 142 182 125 229 102 13 132 97 240 51 1 248 203 77 213 29 61]
Found block  thread 1 random bytestring and modulusscan bytestring match [136 153 119 145 195 82 220 142 182 125 229 102 13 132 97 240 51 1 248 203 77 213 29 61] [136 153 119 145 195 82 220 142 182 125 229 102 13 132 97 240 51 1 248 203 77 213 29 61]

```

This is a test modulus scan on a 30 byte random block and a 208-bit modulus. It computed the file block in 30 minutes on a Pentium 3700 and in 3 minutes on a Ryzen 2700x with 16 threads.
It compares the parallel modulus scan with the single thread modulus scan.
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

$ :~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=30 -mod=208 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  30  byteblock  [253 214 4 230 246 172 128 179 202 193 176 53 21 98 95 206 221 138 138 23 180 211 145 52 140 183 54 233 178 251]
Found Block  [253 214 4 230 246 172 128 179 202 193 176 53 21 98 95 206 221 138 138 23 180 211 145 52 140 183 54 233 178 251]
Total time  3m28.61374s
random bytestring and modulusscan bytestring match  [253 214 4 230 246 172 128 179 202 193 176 53 21 98 95 206 221 138 138 23 180 211 145 52 140 183 54 233 178 251]   [253 214 4 230 246 172 128 179 202 193 176 53 21 98 95 206 221 138 138 23 180 211 145 52 140 183 54 233 178 251]
Found block  thread 6 random bytestring and modulusscan bytestring match [253 214 4 230 246 172 128 179 202 193 176 53 21 98 95 206 221 138 138 23 180 211 145 52 140 183 54 233 178 251] [253 214 4 230 246 172 128 179 202 193 176 53 21 98 95 206 221 138 138 23 180 211 145 52 140 183 54 233 178 251]

```

The first test is a modulus scan on a 40 byte random block and a 288-bit modulus. It computed the file block in under 2 minutes.
The second test is a parallel modulus scan in decoderRandom ran in 38.3 seconds with a Ryzen 2700x with a larger byte block and 16 threads.
```
$ ./decoderRandom 40 288 
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

$ :~/projects/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=40 -mod=288 -thread=16
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  40  byteblock  [149 247 202 99 4 91 124 229 111 84 253 171 95 11 146 130 30 236 57 235 10 25 36 69 152 78 196 42 108 227 131 57 181 106 213 148 204 95 164 172]
Found Block  [149 247 202 99 4 91 124 229 111 84 253 171 95 11 146 130 30 236 57 235 10 25 36 69 152 78 196 42 108 227 131 57 181 106 213 148 204 95 164 172]
Total time  38.306676s
random bytestring and modulusscan bytestring match  [149 247 202 99 4 91 124 229 111 84 253 171 95 11 146 130 30 236 57 235 10 25 36 69 152 78 196 42 108 227 131 57 181 106 213 148 204 95 164 172]   [149 247 202 99 4 91 124 229 111 84 253 171 95 11 146 130 30 236 57 235 10 25 36 69 152 78 196 42 108 227 131 57 181 106 213 148 204 95 164 172]
Found block  thread 3 random bytestring and modulusscan bytestring match [149 247 202 99 4 91 124 229 111 84 253 171 95 11 146 130 30 236 57 235 10 25 36 69 152 78 196 42 108 227 131 57 181 106 213 148 204 95 164 172] [149 247 202 99 4 91 124 229 111 84 253 171 95 11 146 130 30 236 57 235 10 25 36 69 152 78 196 42 108 227 131 57 181 106 213 148 204 95 164 172]


```

This is a 55 byte block example of the modulus scan with a 64 bit modulus. Total execution time was 4.3 seconds.
```
$ ./decoderRandom -mod=64 -thread=16 -bytes=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,1,20,0,0,0,0,0,0,0,0,0,2]
buffer  [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 8 1 20 0 0 0 0 0 0 0 0 0 2]
starting modulus scan threads  16  start thread  0  end thread  16  byteblock size  55  byteblock  [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 8 1 20 0 0 0 0 0 0 0 0 0 2]
Found Block  [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 8 1 20 0 0 0 0 0 0 0 0 0 2]
Total time  4.335839s
random bytestring and modulusscan bytestring match  [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 8 1 20 0 0 0 0 0 0 0 0 0 2]   [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 8 1 20 0 0 0 0 0 0 0 0 0 2]
Found block  thread 0 random bytestring and modulusscan bytestring match [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 8 1 20 0 0 0 0 0 0 0 0 0 2] [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 8 1 20 0 0 0 0 0 0 0 0 0 2]
```


```
user@serverx:~/go/src/github.com/singularian/mdencode/code/testdecode$ ./decoderRandom -block=13 -mod=64 -thread=24
starting modulus scan threads  24  start thread  0  end thread  24  byteblock size  13  byteblock  [114 245 178 128 14 118 213 7 141 176 89 26 79]
Found Block  [114 245 178 128 14 118 213 7 141 176 89 26 79]
Total time  2h43m5.46892182s
random bytestring and modulusscan bytestring match  [114 245 178 128 14 118 213 7 141 176 89 26 79]   [114 245 178 128 14 118 213 7 141 176 89 26 79]
Found block  thread 6 random bytestring and modulusscan bytestring match [114 245 178 128 14 118 213 7 141 176 89 26 79] [114 245 178 128 14 118 213 7 141 176 89 26 79]
```

This is a test modulus scan on a 1000 byte block with a 7968 byte modulus  
[mddecode1000.txt](https://raw.githubusercontent.com/singularian/mdencode/master/examples/mddecode1000.txt)


This next example shows the mdzip and mdlist and mdunzip used in sequence on a 100 byte file.
A script random.sh has been created to randomly generate a n-byte file and mdzip and mdunzip it.
Two different signatures are used for the blocks sha1 and sha_1284 a modified sliced 128-bit SHA-1 signature.
```
morpheus:~/projects/src/github.com/singularian/mdencode/code/decode$ ./random.sh 100 00011 13 64 16
args 5
File Size 100
Block Size 13
Modulus Bit Size 64
Thread Size 16
Creating a random file randomfile

Zipping the file randomfile
filename length  10   10
file Hashlist size  23   md5:sha1-sha1:sha1_1284  attribute 5  [100 13 64 10 70 23]
block number  1 13 [8c8b79d61c8887d59080f9d432bd19d0e76b3a2a 1c8887d59080f9d432bd19d0e76b3a2a] 98 11096481055620584782 64
block number  2 13 [8f759778d907750abf104172a758d76832030059 d907750abf104172a758d76832030059] 99 8330882157427409141 64
block number  3 13 [fa403764b69a3933afd0a4eb9d9460c556d8b975 b69a3933afd0a4eb9d9460c556d8b975] 103 482012241523938924 64
block number  4 13 [f7d5654759fd6b9eb77262e7c25b2fdd48beb7fc 59fd6b9eb77262e7c25b2fdd48beb7fc] 102 15706132613119944451 64
block number  5 13 [49704901b24a15954264337428a16d9b60882a70 b24a15954264337428a16d9b60882a70] 102 2632971523708479270 64
block number  6 13 [bb3bd84d1346b192c76a7dfd1250ee7dbb9f2807 1346b192c76a7dfd1250ee7dbb9f2807] 101 6687011917356290731 64
block number  7 13 [62c8c40821420d274d2a6deafad8889d433eb9b2 21420d274d2a6deafad8889d433eb9b2] 103 13679595928796667994 64
block number  8 9 [39732ba47d3043158b43d33d87ac2014a5d337ec 7d3043158b43d33d87ac2014a5d337ec] 71 13835176885161869058 64

mdlist randomfile
fileSize  100
blockSize  13
modSize  64
fileNameLength  10
filePathLength  70
fileHashLength  23
filePath  /home/nsross/projects/src/github.com/singularian/mdencode/code/decode/ 48 118
filePath  /home/nsross/projects/src/github.com/singularian/mdencode/code/decode/
filename:  randomfile
hashlistname    md5:sha1-sha1:sha1_1284
hashlist        md5:sha1 sha1:sha1_1284
file hashlist   md5:sha1
block hashlist  sha1:sha1_1284

file hashlistname  md5  hex  34bdefc2bae31ee077c350881381aa46
file hashlistname  sha1  hex  da39a3ee5e6b4b0d3255bfef95601890afd80709
block hash 0 13 [8c8b79d61c8887d59080f9d432bd19d0e76b3a2a 1c8887d59080f9d432bd19d0e76b3a2a] 98 11096481055620584782
block hash 1 13 [8f759778d907750abf104172a758d76832030059 d907750abf104172a758d76832030059] 99 8330882157427409141
block hash 2 13 [fa403764b69a3933afd0a4eb9d9460c556d8b975 b69a3933afd0a4eb9d9460c556d8b975] 103 482012241523938924
block hash 3 13 [f7d5654759fd6b9eb77262e7c25b2fdd48beb7fc 59fd6b9eb77262e7c25b2fdd48beb7fc] 102 15706132613119944451
block hash 4 13 [49704901b24a15954264337428a16d9b60882a70 b24a15954264337428a16d9b60882a70] 102 2632971523708479270
block hash 5 13 [bb3bd84d1346b192c76a7dfd1250ee7dbb9f2807 1346b192c76a7dfd1250ee7dbb9f2807] 101 6687011917356290731
block hash 6 13 [62c8c40821420d274d2a6deafad8889d433eb9b2 21420d274d2a6deafad8889d433eb9b2] 103 13679595928796667994
block hash 7 9 [39732ba47d3043158b43d33d87ac2014a5d337ec 7d3043158b43d33d87ac2014a5d337ec] 71 13835176885161869058

mdunzip randomfile
fileSize  100
blockSize  13
modSize  64
fileNameLength  10
filePathLength  70
fileHashLength  23
filePath  /home/nsross/projects/src/github.com/singularian/mdencode/code/decode/ 48 118
filePath  /home/nsross/projects/src/github.com/singularian/mdencode/code/decode/
filename:  randomfile
hashlistname  md5:sha1-sha1:sha1_1284
hashlist  md5:sha1 sha1:sha1_1284
file hashlistname  md5  hex  34bdefc2bae31ee077c350881381aa46
file hashlistname  sha1  hex  da39a3ee5e6b4b0d3255bfef95601890afd80709
Hash ByteBlock 8c8b79d61c8887d59080f9d432bd19d0e76b3a2a1c8887d59080f9d432bd19d0e76b3a2a
Processing block hash  0  blocksize  13 [8c8b79d61c8887d59080f9d432bd19d0e76b3a2a 1c8887d59080f9d432bd19d0e76b3a2a] 98 11096481055620584782
Found Block buffer  [6 132 247 148 178 153 254 158 186 121 29 213 78]  thread  2
Total time  9m40.8054422s
buffer  [6 132 247 148 178 153 254 158 186 121 29 213 78]
Found Thread 2 Block 06 84 f7 94 b2 99 fe 9e ba 79 1d d5 4e
testing mutex
Processing  true
Found block modscan  0  thread  2  block  06 84 f7 94 b2 99 fe 9e ba 79 1d d5 4e
Found block Mutex  0  thread  2  block  06 84 f7 94 b2 99 fe 9e ba 79 1d d5 4e
end testing mutex  true
Hash ByteBlock 8f759778d907750abf104172a758d76832030059d907750abf104172a758d76832030059
Processing block hash  1  blocksize  13 [8f759778d907750abf104172a758d76832030059 d907750abf104172a758d76832030059] 99 8330882157427409141
Found Block buffer  [13 98 8 71 127 115 157 61 46 235 25 192 245]  thread  15
Total time  21m15.1774166s
buffer  [13 98 8 71 127 115 157 61 46 235 25 192 245]
Found Thread 15 Block 0d 62 08 47 7f 73 9d 3d 2e eb 19 c0 f5
testing mutex
Processing  true
Found block modscan  1  thread  15  block  0d 62 08 47 7f 73 9d 3d 2e eb 19 c0 f5
Found block Mutex  1  thread  15  block  0d 62 08 47 7f 73 9d 3d 2e eb 19 c0 f5
end testing mutex  true
Hash ByteBlock fa403764b69a3933afd0a4eb9d9460c556d8b975b69a3933afd0a4eb9d9460c556d8b975
Processing block hash  2  blocksize  13 [fa403764b69a3933afd0a4eb9d9460c556d8b975 b69a3933afd0a4eb9d9460c556d8b975] 103 482012241523938924
Found Block buffer  [229 228 206 4 200 6 176 115 148 113 108 150 108]  thread  8
Total time  6h39m44.4062959s
buffer  [229 228 206 4 200 6 176 115 148 113 108 150 108]
Found Thread 8 Block e5 e4 ce 04 c8 06 b0 73 94 71 6c 96 6c
testing mutex
Processing  true
Found block modscan  2  thread  8  block  e5 e4 ce 04 c8 06 b0 73 94 71 6c 96 6c
Found block Mutex  2  thread  8  block  e5 e4 ce 04 c8 06 b0 73 94 71 6c 96 6c
end testing mutex  true
Hash ByteBlock f7d5654759fd6b9eb77262e7c25b2fdd48beb7fc59fd6b9eb77262e7c25b2fdd48beb7fc
Processing block hash  3  blocksize  13 [f7d5654759fd6b9eb77262e7c25b2fdd48beb7fc 59fd6b9eb77262e7c25b2fdd48beb7fc] 102 15706132613119944451
Found Block buffer  [82 203 203 128 180 217 247 100 101 160 49 39 3]  thread  4
Total time  1h13m34.251126s
buffer  [82 203 203 128 180 217 247 100 101 160 49 39 3]
Found Thread 4 Block 52 cb cb 80 b4 d9 f7 64 65 a0 31 27 03
testing mutex
Processing  true
Found block modscan  3  thread  4  block  52 cb cb 80 b4 d9 f7 64 65 a0 31 27 03
Found block Mutex  3  thread  4  block  52 cb cb 80 b4 d9 f7 64 65 a0 31 27 03
end testing mutex  true
Hash ByteBlock 49704901b24a15954264337428a16d9b60882a70b24a15954264337428a16d9b60882a70
Processing block hash  4  blocksize  13 [49704901b24a15954264337428a16d9b60882a70 b24a15954264337428a16d9b60882a70] 102 2632971523708479270
Found Block buffer  [120 151 45 76 75 36 138 49 166 150 92 231 38]  thread  11
Total time  3h38m0.523192s
buffer  [120 151 45 76 75 36 138 49 166 150 92 231 38]
Found Thread 11 Block 78 97 2d 4c 4b 24 8a 31 a6 96 5c e7 26
testing mutex
Processing  true
Found block modscan  4  thread  11  block  78 97 2d 4c 4b 24 8a 31 a6 96 5c e7 26
Found block Mutex  4  thread  11  block  78 97 2d 4c 4b 24 8a 31 a6 96 5c e7 26
end testing mutex  true
Hash ByteBlock bb3bd84d1346b192c76a7dfd1250ee7dbb9f28071346b192c76a7dfd1250ee7dbb9f2807
Processing block hash  5  blocksize  13 [bb3bd84d1346b192c76a7dfd1250ee7dbb9f2807 1346b192c76a7dfd1250ee7dbb9f2807] 101 6687011917356290731
Found Block buffer  [34 99 247 82 83 92 205 9 232 214 211 82 171]  thread  3
Total time  9m16.1408615s
buffer  [34 99 247 82 83 92 205 9 232 214 211 82 171]
Found Thread 3 Block 22 63 f7 52 53 5c cd 09 e8 d6 d3 52 ab
testing mutex
Processing  true
Found block modscan  5  thread  3  block  22 63 f7 52 53 5c cd 09 e8 d6 d3 52 ab
Found block Mutex  5  thread  3  block  22 63 f7 52 53 5c cd 09 e8 d6 d3 52 ab
end testing mutex  true
Hash ByteBlock 62c8c40821420d274d2a6deafad8889d433eb9b221420d274d2a6deafad8889d433eb9b2
Processing block hash  6  blocksize  13 [62c8c40821420d274d2a6deafad8889d433eb9b2 21420d274d2a6deafad8889d433eb9b2] 103 13679595928796667994
Found Block buffer  [155 63 100 48 124 189 215 176 5 6 206 32 90]  thread  12
Total time  1h46m11.7865889s
buffer  [155 63 100 48 124 189 215 176 5 6 206 32 90]
Found Thread 12 Block 9b 3f 64 30 7c bd d7 b0 05 06 ce 20 5a
testing mutex
Processing  true
Found block modscan  6  thread  12  block  9b 3f 64 30 7c bd d7 b0 05 06 ce 20 5a
Found block Mutex  6  thread  12  block  9b 3f 64 30 7c bd d7 b0 05 06 ce 20 5a
end testing mutex  true
Hash ByteBlock 39732ba47d3043158b43d33d87ac2014a5d337ec7d3043158b43d33d87ac2014a5d337ec
Proccesing block hash  7  blocksize  9 [39732ba47d3043158b43d33d87ac2014a5d337ec 7d3043158b43d33d87ac2014a5d337ec] 71 13835176885161869058
Found Block buffer  [213 192 0 108 19 60 196 191 2]  thread  5
Total time  1.2902ms
buffer  [213 192 0 108 19 60 196 191 2]
Found Thread 5 Block d5 c0 00 6c 13 3c c4 bf 02
testing mutex
Processing  true
Found block modscan  7  thread  5  block  d5 c0 00 6c 13 3c c4 bf 02
Found block Mutex  7  thread  5  block  d5 c0 00 6c 13 3c c4 bf 02
end testing mutex  true
morpheus:~/projects/src/github.com/singularian/mdencode/code/decode$
```


# Donations

* BTC: `32L3gyJdkiFwgJ72rsSzbCBjTjVFxJLHPm`
* ETH: `0x137eB09cF2e199dA2Aa79607f96719be1491a312`
* LTC: `MJh8XpUQ5wm8x1HCoQ7dewNqhrLfZZc31x`

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
