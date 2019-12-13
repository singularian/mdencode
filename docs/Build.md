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

# Build mdencode

Steps to build mdencode on Linux

### Path
$GOPATH/github.com/singularian/mdencode/code/mdencode  

### Build Mdencode

A Makefile was provided for building Mdencode on Linux

### Make getlibs

make getlibs pulls the mdencode library dependencies

$GOPATH/github.com/singularian/mdencode/code/mdencode $ make getlibs

### Make all

make all builds mdencode to gobin by default  

$GOPATH/github.com/singularian/mdencode/code/mdencode $ make all  

### Make build

make build builds the mdencode to the mdBuild directory  

$GOPATH/github.com/singularian/mdencode/code/mdencode $ make build  

# Build mdZip and mdUnzip

mdzip and mdunzip and mdlist are the three main components for mdzip.
mdzip creates a binary signature file with a modulus or modular floor.
mdunzip will decode and mdzip file and write the output to a destination file.


### Path

$GOPATH/github.com/singularian/mdencode/code/decode

### Make all

This is the build steps with the Makefile on Linux.

$GOPATH/github.com/singularian/mdencode/code/decode $ make getlibs 
$GOPATH/github.com/singularian/mdencode/code/decode $ make all 

### Build

Make build installs mdzip and mdunzip and mdlist to the binary directory

$GOPATH/github.com/singularian/mdencode/code/decode $ make build 

# Build decoderRandom

decoderRandom is a test command line program which creates a random byte block. It uses  
a signature pair MD5 and SHA-1 and then creates a modulus floor and then runs a modulus scan.  
It will then try to find the corresponding byte block and check if the modulus scan found the correct byte block.  

### Path

$GOPATH/github.com/singularian/mdencode/code/testdecode  

### Build 

This example builds the mdencode decoderRandom files  

$GOPATH/github.com/singularian/mdencode/code/testdecode $ make all  
