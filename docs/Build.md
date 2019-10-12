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

## Path
$GOPATH/github.com/singularian/mdencode/code/mdencode  

## Build Mdencode

make all builds mdencode to gobin by default  

$GOPATH/github.com/singularian/mdencode/code/mdencode $ make all  

make build builds the mdencode to the mdBuild directory  

$GOPATH/github.com/singularian/mdencode/code/mdencode $ make build  

# Build mdZip and mdUnzip

$GOPATH/github.com/singularian/mdencode/code/decode  

# Build decoderRandom

## Path

$GOPATH/github.com/singularian/mdencode/code/testdecode  

## Build 

This example builds the mdencode decoderRandom files  
$GOPATH/github.com/singularian/mdencode/code/decode $ make all  
