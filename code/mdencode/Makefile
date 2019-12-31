# This is how we want to name the binary output
BINARY=mdencode

# Project name
PROJECT=mdencode

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# BUILD_DIR=../build
# BUILD_DIR=${GOPATH}src/github.com/singularian/mdencode/code/build
# BUILD_DIR=${GOPATH}/bin
BUILD_DIR=${GOBIN}

# command path
CMD=cmd

# Setup the -ldflags option for go build here, interpolate the variable values
# LDFLAGS=-ldflags "-X github.com/singularian/mdencode/core.Version=${VERSION} -X github.com/singularian/mdencode/main.BuildTime=${BUILD_TIME}"
LDFLAGS=-ldflags "-X main.Version=${VERSION} -X main.BuildTime=${BUILD_TIME}"

all:
	go build $(LDFLAGS) -o ${BUILD_DIR}/md ${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdencode ${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdnewdb ${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mddbreport ${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdbinlist ${CMD}/testBinReadMdFmt.go
build:
	go build $(LDFLAGS) -o ${BUILD_DIR}/md ${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdencode ${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdnewdb ${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mddbreport ${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdbinlist ${CMD}/testBinReadMdFmt.go
# retrieve the go libraries
getlibs:
	go get -u golang.org/x/crypto/blake2s
	go get -u golang.org/x/crypto/md4
	go get -u golang.org/x/crypto/sha3
	go get -u golang.org/x/crypto/ripemd160
	go get -u github.com/tildeleb/aeshash
	go get -u github.com/skeeto/cubehash
	go get -u github.com/martinlindhe/gogost
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
fmt:
	# gofmt -l -w $(SRC)
lint:
	GOPATH=$(GOPATH) PATH=$(GOPATH)/bin:$(PATH) golint .
clean:
	rm -r ${BUILD_DIR}/mdencode ${BUILD_DIR}/md ${BUILD_DIR}/mdnewdb ${BUILD_DIR}/mddbreport
	rm -r mdBuild/md mdBuild/mdencode mdBuild/mdnewdb mdBuild/mddbreport mdBuild/mdbinlist
cleanlogs:
	rm *.log
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
	echo "all build getlibs clean cleanlogs list"
