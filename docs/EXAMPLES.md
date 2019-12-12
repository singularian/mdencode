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

# Donations

* BTC: `32L3gyJdkiFwgJ72rsSzbCBjTjVFxJLHPm`
* ETH: `0x137eB09cF2e199dA2Aa79607f96719be1491a312`
* LTC: `MJh8XpUQ5wm8x1HCoQ7dewNqhrLfZZc31x`

# License 

[GNU General Public License v3.0](https://github.com/singularian/mdencode/blob/master/LICENSE)
