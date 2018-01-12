# This is how we want to name the binary output
BINARY=mdencode

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# BUILD_DIR=../build
# BUILD_DIR=${GOPATH}src/github.com/singularian/mdencode_private/code/build
# BUILD_DIR=${GOPATH}/bin
BUILD_DIR=${GOBIN}

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/singularian/mdencode/core.Version=${VERSION} -X github.com/singularian/mdencode/core.BuildTime=${BUILD_TIME}"

all:
	go build -o ${BUILD_DIR}/md mdencode.go
	go build -o ${BUILD_DIR}/mdencode mdencode.go
	go build -o ${BUILD_DIR}/mdnewdb mdnewdb.go
	go build -o ${BUILD_DIR}/mddbreport mdencodeReport.go
build:
#	go install mdencode.go
#	go install mdencodeSQL.go
#	go install ${BUILD_DIR}/mdencodeSQL
#	go install -o mdencodeXML mdencode.go
#	go install -o mdencode mdencode.go
#	go install -o mdencodebyteblock mdencode.go
#	go install -o mdencodereport mdencodeSQL.go
# retrieve the go libraries
getlibs:
	go get -u github.com/steakknife/keccak
	go get -u github.com/mattn/go-sqlite3
	go get -u github.com/asadmshah/murmur3
	go get -u golang.org/x/crypto/ripemd160
	go get -u github.com/dchest/siphash
	go get -u github.com/aead/skein
	go get -u github.com/cxmcc/tiger
	go get -u github.com/jzelinskie/whirlpool
fmt:
	@gofmt -l -w $(SRC)
clean:
	rm -r ${BUILD_DIR}/mdencode ${BUILD_DIR}/md ${BUILD_DIR}/mdnewdb ${BUILD_DIR}/mddbreport
list:
	echo "all"
