# Clone mdencode

This is the basic Linux / Unix Go mdencode build

git clone https://github.com/singularian/mdencode.git  

git clone ssh://git@github.com/singularian/mdencode.git  

# mdencode Go Library Dependancies

```
Go Library Dependencies
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
go get -u github.com/fogleman/gg
```

# Linux Build mdencode

Steps to build mdencode on Linux

### Path
$GOPATH/github.com/singularian/mdencode/build  

### Build Mdencode

A Makefile was provided for building Mdencode on Linux

### Make getlibs

make getlibs pulls the mdencode library dependencies

$GOPATH/github.com/singularian/mdencode/build $ make all

### Make all

make all builds mdencode to gobin by default  

$GOPATH/github.com/singularian/mdencode/build $ make all  

### Make build

make build builds the mdencode to the mdBuild directory  

$GOPATH/github.com/singularian/mdencode/build $ make build  

# Build mdZip and mdUnzip

mdzip and mdunzip and mdlist are the three main components for mdzip.
mdzip creates a binary signature file with a modulus or modular floor.
mdunzip will decode and mdzip file and write the output to a destination file.


### Path

$GOPATH/github.com/singularian/mdencode/build

### Make all

This is the build steps with the Makefile on Linux with GO Modules.

$GOPATH/github.com/singularian/mdencode/build $ make all  
$GOPATH/github.com/singularian/mdencode/build $ make build   

### Build

Make build installs mdzip and mdunzip and mdlist to the binary directory  

$GOPATH/github.com/singularian/mdencode/build $ make mdzip   
$GOPATH/github.com/singularian/mdencode/build $ make build   

# Build decoderRandom

decoderRandom is a test command line program which creates a random byte block. It uses  
a signature pair MD5 and SHA-1 and then creates a modulus floor and then runs a modulus scan.  
It will then try to find the corresponding byte block and check if the modulus scan found the correct byte block.  

### Paths

$GOPATH/github.com/singularian/mdencode/build   (main build directory)  

$GOPATH/github.com/singularian/mdencode/code/testdecode  
$GOPATH/github.com/singularian/mdencode/code/decode  
$GOPATH/github.com/singularian/mdencode/code/mdencode  

### Build 

This example builds the mdencode decoderRandom files   

$GOPATH/github.com/singularian/mdencode/code/testdecode $ make all  
$GOPATH/github.com/singularian/mdencode/build $ make test  


# Windows Build

1) Download and install git windows  
2) Add git to the windows environment path  
3) Install gcc with mingw (http://mingw-w64.org/) use the windows 64 bit install option  
4) Add mingw-w64 to the windows environment path for gcc.exe  
5) Build the go files  
