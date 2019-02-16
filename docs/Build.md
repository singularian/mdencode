# Build mdencode

Basic Linux / Unix Go mdencode Build  

git clone https://github.com/singularian/mdencode.git

make all builds mdencode to gobin by default
$GOPATH/github.com/singularian/mdencode/code/encode $ make all

make build builds the mdencode to the mdBuild directory
$GOPATH/github.com/singularian/mdencode/code/mdencode $ make build

# mdencode Go Library Dependancies

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


# Build decoderRandom

$GOPATH/github.com/singularian/mdencode/code/decode

This example builds the mdencode decoderRandom files
$GOPATH/github.com/singularian/mdencode/code/decode $ make build
