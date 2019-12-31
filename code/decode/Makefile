# This is how we want to name the binary output
BINARY=mdzip

# Project name
PROJECT="mdencode mdzip"

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# build directory
BUILD_DIR=${GOBIN}

# command path
CMD=cmd

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/nsross/mdencode/core.Version=${VERSION} -X github.com/nsross/mdencode/core.BuildTime=${BUILD_TIME}"

all:
	go build -o mdunzip ${CMD}/mdUnzip.go 
	go build -o mdzip ${CMD}/mdzip.go 
	go build -o mdlist ${CMD}/mdlist.go 
	go build -o mdlistTest ${CMD}/testBinReadMdFmt.go 
	go build -o mdsig ${CMD}/mdsig.go 
build: 
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdunzip ${CMD}/mdUnzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdzip ${CMD}/mdzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlist ${CMD}/mdlist.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlistTest ${CMD}/testBinReadMdFmt.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdsig ${CMD}/mdsig.go
goget:
	go get -u golang.org/x/crypto/blake2s
	go get -u golang.org/x/crypto/md4
	go get -u golang.org/x/crypto/sha3
	go get -u golang.org/x/crypto/ripemd160
	go get -u github.com/tildeleb/aeshash
	go get -u github.com/skeeto/cubehash
	go get -u github.com/martinlindhe/gogost
	go get -u github.com/minio/blake2b-simd
	go get -u github.com/minio/highwayhash
	go get -u github.com/maoxs2/go-ripemd
	go get -u github.com/steakknife/keccak
	go get -u github.com/asadmshah/murmur3
	go get -u github.com/dchest/siphash
	go get -u github.com/aead/skein
	go get -u github.com/cxmcc/tiger
	go get -u github.com/jzelinskie/whirlpool
	go get -u github.com/mattn/go-sqlite3
	go get -u github.com/fogleman/gg
	go get -u github.com/aead/siphash
	go get -u github.com/codahale/blake2
	go get -u github.com/enceve/crypto/skein
	go get -u github.com/mimoo/GoKangarooTwelve/K12
	go get -u github.com/phoreproject/go-x11
	go get -u github.com/OneOfOne/xxhash
	go get -u github.com/singularian/mdhash
getlibs:
	make getlibs
clean:
	rm -r mdzip mdunzip mdlist mdlistTest mdsig *.log
cleanlog:
	rm -r *.log
# Display general help about this command
help:
	@echo ""
	@echo "$(PROJECT) Makefile."
	@echo "GOPATH=$(GOPATH)"
	@echo "The following commands are available:"
	@echo "    make all         : Build the mdencode files"
	@echo "    make build       : Build the mdencode files to the build directory"
	@echo "    make getlibs     : Get the Golang $(PROJECT) dependencies"
	@echo "    make clean       : Remove any build artifact"
list:
	echo "all build clean cleanlog list"
